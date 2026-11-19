#include <takatori/util/symbolizer.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <future>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <pthread.h>

#include <gtest/gtest.h>

namespace takatori::util {

using namespace std::chrono_literals;

// NOTE: these tests do not pin the contents of human readable strings, nor timings.

class symbolizer_test : public ::testing::Test {
public:
    ::boost::stacktrace::stacktrace trace_ {};

    /**
     * @brief calls the symbolizer repeatedly until it returns a value.
     * @return the value, or std::nullopt if it did not within the limit
     */
    static std::optional<std::string> wait_for_value(symbolizer& target, ::boost::stacktrace::stacktrace const& trace) {
        auto limit = std::chrono::steady_clock::now() + 10s;
        while (std::chrono::steady_clock::now() < limit) {
            if (auto result = target(trace)) {
                return result;
            }
            std::this_thread::sleep_for(10ms);
        }
        return std::nullopt;
    }
};

/**
 * @brief a gate which blocks tasks until it is opened.
 * @details This opens itself when destroyed. If a symbolizer is declared after this gate,
 *      declare the result of open_on_exit() after the symbolizer, so that the gate is opened
 *      before the destructor of the symbolizer waits for its thread, even if the test is left early.
 */
class gate {
public:
    gate() = default;
    ~gate() { open(); }
    gate(gate const&) = delete;
    gate(gate&&) = delete;
    gate& operator=(gate const&) = delete;
    gate& operator=(gate&&) = delete;

    void open() {
        std::call_once(once_, [this] { promise_.set_value(); });
    }

    /// @brief returns a future which becomes ready when this is opened (it can outlive this gate).
    [[nodiscard]] std::shared_future<void> waiter() const {
        return opened_;
    }

    /// @brief opens the gate when destroyed.
    class opener {
    public:
        explicit opener(gate& target) noexcept : target_ { target } {}
        ~opener() { target_.open(); }
        opener(opener const&) = delete;
        opener(opener&&) = delete;
        opener& operator=(opener const&) = delete;
        opener& operator=(opener&&) = delete;
    private:
        gate& target_;
    };

    [[nodiscard]] opener open_on_exit() {
        return opener { *this };
    }

private:
    std::promise<void> promise_ {};
    std::shared_future<void> opened_ { promise_.get_future().share() };
    std::once_flag once_ {};
};

TEST_F(symbolizer_test, simple) {
    symbolizer target {};
    auto result = target(trace_);
    ASSERT_TRUE(result);
    EXPECT_FALSE(result->empty());
}

TEST_F(symbolizer_test, empty_trace) {
    symbolizer target {};
    auto result = target(::boost::stacktrace::stacktrace { 0, 0 });
    ASSERT_TRUE(result);
    EXPECT_TRUE(result->empty());
}

TEST_F(symbolizer_test, process_wide) {
    if (!symbolizer::process_wide_enabled()) {
        GTEST_SKIP() << "the process-wide symbolizer is disabled in this build";
    }
    auto* first = symbolizer::find();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(symbolizer::find(), first);
}

TEST_F(symbolizer_test, process_wide_disabled) {
    if (symbolizer::process_wide_enabled()) {
        GTEST_SKIP() << "the process-wide symbolizer is enabled in this build";
    }
    EXPECT_EQ(symbolizer::find(), nullptr);
    // stacktraces are still printed, without function names
    EXPECT_FALSE(current_stacktrace().to_string().empty());
    // instances created directly still work
    symbolizer target {};
    EXPECT_TRUE(target(trace_));
}

TEST_F(symbolizer_test, on_its_own_thread) {
    // a request from the thread of the symbolizer itself must not wait for itself
    // declared before target: target is destroyed (and its thread is joined) first
    std::promise<std::optional<std::string>> inner {};
    auto inner_result = inner.get_future();
    symbolizer target { 1s };
    target.submit([&] {
        inner.set_value(target(trace_));
    });
    ASSERT_EQ(inner_result.wait_for(10s), std::future_status::ready);
    auto result = inner_result.get();
    ASSERT_TRUE(result);
    EXPECT_FALSE(result->empty());
}

TEST_F(symbolizer_test, timeout_and_recovery) {
    symbolizer target { 100ms };
    // the first conversion of a new instance reads debug information and may take longer than the timeout
    ASSERT_TRUE(wait_for_value(target, trace_));

    // keep the thread busy until the gate opens
    std::promise<void> gate {};
    auto opened = gate.get_future().share();
    target.submit([opened] {
        opened.wait();
    });

    // the request times out, and is orphaned
    EXPECT_FALSE(target(trace_));
    // while the orphaned request is in flight, requests return std::nullopt (without waiting)
    EXPECT_FALSE(target(trace_));

    // after the thread becomes free, it returns values again
    gate.set_value();
    auto result = wait_for_value(target, trace_);
    ASSERT_TRUE(result);
    EXPECT_FALSE(result->empty());
}

TEST_F(symbolizer_test, destroy_with_pending_tasks) {
    // the destructor stops the thread even if tasks remain in the queue
    {
        symbolizer target {};
        // declared after target: if this scope is left early, the gate is destroyed first,
        // which releases the blocked task, so that the destructor of target does not wait forever
        std::promise<void> gate {};
        auto opened = gate.get_future().share();
        target.submit([opened] {
            opened.wait();
        });
        target.submit([] {});
        gate.set_value();
    }
    // the second task may or may not remain in the queue when the destructor runs; it just must not hang
    SUCCEED();
}

TEST_F(symbolizer_test, timeout_while_converting) {
    // running -> orphaned -> done: the caller stops waiting while the thread is converting its request
    gate converting {};
    std::promise<void> entered {};
    auto entered_future = entered.get_future();
    std::atomic_bool first { true };
    symbolizer target {
            1s,
            symbolizer::default_thread_name,
            [opened = converting.waiter(), &entered, &first](::boost::stacktrace::stacktrace const& trace) {
                if (first.exchange(false)) {
                    entered.set_value();
                    opened.wait();
                }
                return symbolizer::convert_by_boost(trace);
            } };
    auto release = converting.open_on_exit();

    // the request times out while its conversion is blocked
    EXPECT_FALSE(target(trace_));
    // the conversion had started (otherwise the thread would have skipped the request)
    EXPECT_EQ(entered_future.wait_for(0s), std::future_status::ready);
    // while the orphaned request is in flight, requests return std::nullopt
    EXPECT_FALSE(target(trace_));

    // after the conversion finishes, it returns values again
    converting.open();
    auto result = wait_for_value(target, trace_);
    ASSERT_TRUE(result);
    EXPECT_FALSE(result->empty());
}

TEST_F(symbolizer_test, many_orphaned_requests) {
    symbolizer target { 100ms };
    // the first conversion of a new instance reads debug information and may take longer than the timeout
    ASSERT_TRUE(wait_for_value(target, trace_));

    // declared after target: opened before the destructor of target waits for its thread
    gate busy {};
    target.submit([opened = busy.waiter()] {
        opened.wait();
    });
    constexpr std::size_t callers = 4;
    std::atomic_size_t values { 0 };
    std::vector<std::thread> threads {};
    threads.reserve(callers);
    for (std::size_t i = 0; i < callers; ++i) {
        threads.emplace_back([&] {
            if (target(trace_)) {
                values.fetch_add(1);
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    // every request timed out, or returned at once because others were orphaned
    EXPECT_EQ(values.load(), 0);

    // after the thread skips all orphaned requests, it returns values again
    busy.open();
    auto result = wait_for_value(target, trace_);
    ASSERT_TRUE(result);
    EXPECT_FALSE(result->empty());
}

TEST_F(symbolizer_test, conversion_failure) {
    symbolizer target {
            symbolizer::default_timeout,
            symbolizer::default_thread_name,
            [](::boost::stacktrace::stacktrace const&) -> std::string {
                throw std::runtime_error { "testing" };
            } };
    EXPECT_FALSE(target(trace_));
    // a failure is not a timeout: the next request is still asked to the thread
    EXPECT_FALSE(target(trace_));
}

TEST_F(symbolizer_test, conversion_bad_alloc) {
    symbolizer target {
            symbolizer::default_timeout,
            symbolizer::default_thread_name,
            [](::boost::stacktrace::stacktrace const&) -> std::string {
                throw std::bad_alloc {};
            } };
    EXPECT_THROW((void) target(trace_), std::bad_alloc);
}

TEST_F(symbolizer_test, failures_on_its_own_thread) {
    // declared before target: target is destroyed (and its thread is joined) first
    std::promise<bool> runtime_error_result {};
    std::promise<bool> bad_alloc_result {};
    auto runtime_error_future = runtime_error_result.get_future();
    auto bad_alloc_future = bad_alloc_result.get_future();
    std::atomic_bool throw_bad_alloc { false };
    symbolizer target {
            symbolizer::default_timeout,
            symbolizer::default_thread_name,
            [&throw_bad_alloc](::boost::stacktrace::stacktrace const&) -> std::string {
                if (throw_bad_alloc.load()) {
                    throw std::bad_alloc {};
                }
                throw std::runtime_error { "testing" };
            } };
    target.submit([&] {
        // degrades to std::nullopt
        runtime_error_result.set_value(!target(trace_).has_value());
        throw_bad_alloc.store(true);
        try {
            (void) target(trace_);
            bad_alloc_result.set_value(false);
        } catch (std::bad_alloc const&) {
            // propagates to the caller
            bad_alloc_result.set_value(true);
        }
    });
    ASSERT_EQ(runtime_error_future.wait_for(10s), std::future_status::ready);
    EXPECT_TRUE(runtime_error_future.get());
    ASSERT_EQ(bad_alloc_future.wait_for(10s), std::future_status::ready);
    EXPECT_TRUE(bad_alloc_future.get());
}

TEST_F(symbolizer_test, signal_mask) {
    ::sigset_t before {};
    ::pthread_sigmask(SIG_SETMASK, nullptr, &before);
    // declared before target: target is destroyed (and its thread is joined) first
    std::promise<::sigset_t> inner {};
    auto inner_mask = inner.get_future();
    symbolizer target {};
    ::sigset_t after {};
    ::pthread_sigmask(SIG_SETMASK, nullptr, &after);
    target.submit([&inner] {
        ::sigset_t mask {};
        ::pthread_sigmask(SIG_SETMASK, nullptr, &mask);
        inner.set_value(mask);
    });
    ASSERT_EQ(inner_mask.wait_for(10s), std::future_status::ready);
    auto mask = inner_mask.get();

    // asynchronous signals are blocked on the thread
    for (int signal : { SIGINT, SIGTERM, SIGHUP, SIGQUIT, SIGUSR1, SIGUSR2, SIGPIPE, SIGALRM, SIGCHLD }) {
        EXPECT_EQ(::sigismember(&mask, signal), 1) << signal;
    }
    // synchronous signals are not, so that a crash handler still runs on a fault
    for (int signal : { SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT, SIGTRAP }) {
        EXPECT_EQ(::sigismember(&mask, signal), 0) << signal;
    }
    // the signal mask of the creating thread is restored
    for (int signal = 1; signal < NSIG; ++signal) {
        EXPECT_EQ(::sigismember(&before, signal), ::sigismember(&after, signal)) << signal;
    }
}

} // namespace takatori::util
