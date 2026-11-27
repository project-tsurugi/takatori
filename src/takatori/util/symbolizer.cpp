#include "symbolizer.h"

#include <csignal>
#include <exception>
#include <future>
#include <memory>
#include <new>
#include <system_error>
#include <utility>

#include <pthread.h>

#include <boost/version.hpp>

namespace takatori::util {

namespace {

// whether the process-wide instance is available; if not, stacktraces are printed without function names
// NOTE: BOOST_STACKTRACE_USE_BACKTRACE must be checked after including stacktrace.h (via symbolizer.h),
// which undefines it if <backtrace.h> is not available, so that this matches the backend actually used
#if defined(BOOST_STACKTRACE_USE_BACKTRACE) && BOOST_VERSION < 106900
// Boost.Stacktrace older than 1.69 (libbacktrace) creates a new state for each conversion and never releases it,
// so a single dedicated thread does not bound the memory: disabled regardless of the build option
constexpr bool process_wide_enabled_v = false;
#elif defined(TAKATORI_ENABLE_STACKTRACE_SYMBOLIZATION)
// enabled by the build option ENABLE_STACKTRACE_SYMBOLIZATION (ON by default)
constexpr bool process_wide_enabled_v = true;
#else
// disabled by the build option ENABLE_STACKTRACE_SYMBOLIZATION=OFF,
// or built without the CMake scripts of this project (which define the macro above)
constexpr bool process_wide_enabled_v = false;
#endif

// trivially destructible: safe to read even during or after static destruction
std::atomic_bool forked_child { false }; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

// Registered when this library is loaded (before main(), while the process has only one thread),
// not in find(): if fork() were called while find() is lazily creating the instance,
// the child could block in find() forever, because the initialization looks still in progress there.
// With this, the child always sees forked_child and never touches the instance.
// NOTE: this does not detect a child process created without running pthread_atfork handlers
// (e.g. by calling the clone or fork system call directly, not through fork()).
// Such a child would use the inherited instance, whose thread does not exist there: its first request waits
// until the timeout, or forever if another thread held the queue lock at the fork.
// Comparing getpid() with the process ID recorded at load time would also guard against it,
// but such children are not expected in our configurations, so it is not done for now.
[[maybe_unused]] int const atfork_registered = ::pthread_atfork(nullptr, nullptr, +[] { forked_child.store(true); });

symbolizer* create_process_wide() {
    auto result = std::make_unique<symbolizer>();
    // intentionally never deleted, so that its destructor is never called, in the parent or in a child process
    return result.release();
}

/**
 * @brief starts a thread with all asynchronous signals blocked.
 * @details The new thread inherits the signal mask of the calling thread, so blocking them here
 *      leaves no window in which a signal handler could run on the new thread.
 *      The signal mask of the calling thread is restored before this returns.
 *
 *      Synchronous signals (raised by a fault of the thread itself) are not blocked,
 *      so that the crash handler of the process (e.g. glog's failure signal handler) still runs
 *      if the new thread crashes; blocking them would make the kernel kill the process without it.
 */
template<class F>
std::thread start_with_asynchronous_signals_blocked(F&& function) {
    ::sigset_t blocked {};
    ::sigfillset(&blocked);
    for (int synchronous : { SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT, SIGTRAP }) {
        ::sigdelset(&blocked, synchronous);
    }
    ::sigset_t saved {};
    ::pthread_sigmask(SIG_SETMASK, &blocked, &saved);
    try {
        std::thread result { std::forward<F>(function) };
        ::pthread_sigmask(SIG_SETMASK, &saved, nullptr);
        return result;
    } catch (...) {
        ::pthread_sigmask(SIG_SETMASK, &saved, nullptr);
        throw;
    }
}

} // namespace

std::string symbolizer::convert_by_boost(::boost::stacktrace::stacktrace const& trace) {
    return ::boost::stacktrace::to_string(trace);
}

symbolizer::symbolizer(std::chrono::milliseconds timeout, std::string thread_name, converter_type converter) :
    timeout_ { timeout },
    converter_ { std::move(converter) },
    thread_ { start_with_asynchronous_signals_blocked([this, thread_name = std::move(thread_name)] {
        // NOTE: the CPU affinity is intentionally left as inherited from the thread that created this thread
        // (the process-wide instance is created lazily by whichever thread first needs it).
        // If threads are pinned to cores or NUMA nodes (e.g. jogasaki's dev_core_affinity, disabled by default),
        // this thread may be pinned together with such a thread; set the affinity of this thread separately
        // in such environments.
        // best effort: the name is not set if it is too long
        ::pthread_setname_np(::pthread_self(), thread_name.c_str());
        run();
    }) }
{}

symbolizer::~symbolizer() {
    {
        std::lock_guard lock { mutex_ };
        stopping_ = true;
    }
    cv_.notify_all();
    thread_.join();
}

bool symbolizer::process_wide_enabled() noexcept {
    return process_wide_enabled_v;
}

symbolizer* symbolizer::find() {
    if constexpr (!process_wide_enabled_v) {
        return nullptr;
    }
    // check fork before touching the instance: its lock may be held by a thread which does not exist in the child
    if (forked_child.load()) {
        return nullptr;
    }
    try {
        // if this throws, the initialization is retried on the next call
        static symbolizer* instance = create_process_wide();
        return instance;
    } catch (std::system_error const&) {
        // the thread could not be started; std::bad_alloc is not caught here
        return nullptr;
    }
}

namespace {

/// @brief the state of a request (see symbolizer::operator()).
enum class request_state : int {
    pending,
    running,
    done,
    orphaned,
};

/// @brief calls the function when this goes out of scope.
template<class F>
class finally {
public:
    explicit finally(F function) noexcept : function_ { std::move(function) } {}
    ~finally() { function_(); }
    finally(finally const&) = delete;
    finally(finally&&) = delete;
    finally& operator=(finally const&) = delete;
    finally& operator=(finally&&) = delete;
private:
    F function_;
};

} // namespace

std::optional<std::string> symbolizer::operator()(::boost::stacktrace::stacktrace const& trace) {
    if (std::this_thread::get_id() == thread_.get_id()) {
        try {
            return converter_(trace);
        } catch (std::bad_alloc const&) {
            throw;
        } catch (std::exception const&) {
            // other failures degrade to the address-only form
            return std::nullopt;
        }
    }
    // while an orphaned request is still in flight, the thread is considered stalled: do not wait
    if (stalled()) {
        return std::nullopt;
    }
    auto deadline = clock_type::now() + timeout_;
    auto result = std::make_shared<std::promise<std::string>>();
    auto future = result->get_future();
    // Each request has its own state, and the caller and the thread move it with exchange/compare-exchange,
    // so that exactly one of them sees the other's transition:
    //   pending -> running -> done      (normal)
    //   pending -> orphaned             (the caller stopped waiting before the thread started it; the thread skips it)
    //   running -> orphaned -> done     (the caller stopped waiting while the thread was working on it)
    //   done -> orphaned                (the caller timed out just as the thread finished it; not counted)
    // orphaned_count_ is the number of orphaned requests which the thread has not finished or skipped yet.
    auto state = std::make_shared<std::atomic<request_state>>(request_state::pending);
    // the promise is owned by the task only: if the task ends (or is discarded) without setting it,
    // the caller gets std::future_error (broken promise) instead of waiting forever
    submit([this, result = std::move(result), state, trace] {
        auto expected = request_state::pending;
        if (!state->compare_exchange_strong(expected, request_state::running)) {
            // orphaned before it started: skip it (nobody waits for the result)
            orphaned_count_.fetch_sub(1);
            return;
        }
        // runs after the result is set, so that a caller who sees `done` can take the result at once
        finally on_exit { [this, &state] {
            if (state->exchange(request_state::done) == request_state::orphaned) {
                orphaned_count_.fetch_sub(1);
            }
        } };
        try {
            result->set_value(converter_(trace));
        } catch (...) {
            // forward to the waiting thread (e.g. std::bad_alloc), which rethrows it in future.get()
            try {
                result->set_exception(std::current_exception());
            } catch (...) { // NOLINT(bugprone-empty-catch)
                // the promise is left unset; the caller gets a broken promise when this task is destroyed
            }
        }
    });
    if (future.wait_until(deadline) != std::future_status::ready) {
        if (state->exchange(request_state::orphaned) != request_state::done) {
            // still pending or running: count it until the thread finishes or skips it.
            // The thread may decrement before this increment; the count is signed and only compared with > 0.
            orphaned_count_.fetch_add(1);
            if (future.wait_for(std::chrono::seconds { 0 }) != std::future_status::ready) {
                return std::nullopt;
            }
            // the result was set just before the thread marked it done: take it
            // (the count stays incremented; the thread decrements it when it marks the request done)
        }
        // finished just in time: the result is set (or about to be visible), take it
    }
    try {
        return future.get();
    } catch (std::bad_alloc const&) {
        throw;
    } catch (std::exception const&) {
        // the conversion failed on the thread, or the task was discarded before it ran (std::future_error);
        // they degrade to the address-only form
        // NOTE: catch std::exception, not (...), so that forced unwinding (thread cancellation) is not swallowed
        return std::nullopt;
    }
}

void symbolizer::submit(std::function<void()> task) {
    {
        std::lock_guard lock { mutex_ };
        queue_.emplace_back(std::move(task));
    }
    cv_.notify_one();
}

void symbolizer::run() {
    while (true) {
        std::function<void()> task {};
        {
            std::unique_lock lock { mutex_ };
            cv_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
            if (stopping_) {
                return;
            }
            task = std::move(queue_.front());
            queue_.pop_front();
        }
        try {
            task();
        } catch (...) { // NOLINT(bugprone-empty-catch)
            // tasks report their own results and are not expected to throw;
            // this only keeps the thread alive (an exception escaping here would call std::terminate)
        }
    }
}

bool symbolizer::stalled() const noexcept {
    return orphaned_count_.load() > 0;
}

} // namespace takatori::util
