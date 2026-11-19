#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

#include <takatori/util/stacktrace.h>

namespace takatori::util {

/**
 * @brief a dedicated thread that converts stacktraces into strings.
 * @details Boost.Stacktrace (libbacktrace) keeps its symbolization state per thread and never releases it.
 *      Converting all stacktraces on this single thread keeps the number of the states to one.
 *
 *      The process-wide instance is available via find(), and is never destroyed,
 *      so that it does not conflict with static destruction on process exit.
 *      Other instances are for testing, and their destructor stops the thread.
 *
 *      The thread starts with all asynchronous signals blocked, regardless of the thread that happened to create it
 *      (so that no signal handler for them runs on it; synchronous signals such as SIGSEGV stay unblocked,
 *      so that a crash handler still runs on a fault). Its CPU affinity is inherited from that thread as is
 *      (see the constructor).
 */
class symbolizer {
public:
    /// @brief the clock type.
    using clock_type = std::chrono::steady_clock;

    /// @brief the default maximum time to wait for the result.
    /// @note also written in the documentation of stacktrace::to_string(): keep them consistent.
    static constexpr std::chrono::milliseconds default_timeout { 10'000 };

    /// @brief the default name of the thread.
    static constexpr char const* default_thread_name = "stacktrace";

    /// @brief the type of functions that convert stacktraces into strings.
    using converter_type = std::function<std::string(::boost::stacktrace::stacktrace const&)>;

    /**
     * @brief converts the given stacktrace by Boost.Stacktrace (the default converter).
     * @param trace the target stacktrace
     * @return the converted string
     */
    [[nodiscard]] static std::string convert_by_boost(::boost::stacktrace::stacktrace const& trace);

    /**
     * @brief creates a new instance, and starts its thread.
     * @param timeout the maximum time to wait for the result
     * @param thread_name the name of the thread, at most 15 bytes on Linux
     *      (if it is longer, the name is not set, and nothing else changes)
     * @param converter the function that converts stacktraces, called on the thread of this object;
     *      other than the default is only for testing
     * @throws std::system_error if the thread could not be started
     */
    explicit symbolizer(
            std::chrono::milliseconds timeout = default_timeout,
            std::string thread_name = default_thread_name,
            converter_type converter = convert_by_boost);

    /**
     * @brief stops the thread and waits for it.
     * @details The tasks remaining in the queue are discarded.
     */
    ~symbolizer();

    symbolizer(symbolizer const&) = delete;
    symbolizer(symbolizer&&) = delete;
    symbolizer& operator=(symbolizer const&) = delete;
    symbolizer& operator=(symbolizer&&) = delete;

    /**
     * @brief returns the process-wide instance.
     * @details This creates the instance on the first call.
     *      A child process is detected by a pthread_atfork handler, so a child created without running
     *      such handlers (e.g. by a raw clone system call) is not detected, and gets the inherited instance,
     *      whose thread does not exist there.
     * @return the process-wide instance
     * @return nullptr if it is not available: disabled in this build (see process_wide_enabled()),
     *      in a child process after fork(), or if its thread could not be started
     * @throws std::bad_alloc if memory allocation failed
     */
    [[nodiscard]] static symbolizer* find();

    /**
     * @brief returns whether or not the process-wide instance is enabled in this build.
     * @details It is disabled by the build option ENABLE_STACKTRACE_SYMBOLIZATION=OFF,
     *      or with Boost.Stacktrace older than 1.69 on libbacktrace, which leaks memory on each conversion.
     *      If disabled, find() always returns nullptr, and stacktraces are printed without function names.
     *      Instances created directly (for testing) work regardless of this.
     * @return true if it is enabled
     */
    [[nodiscard]] static bool process_wide_enabled() noexcept;

    /**
     * @brief converts the given stacktrace into a human readable string.
     * @details This asks the thread of this object, and waits for the result.
     *      On the thread of this object itself, this converts it directly.
     *      Once a request timed out, this returns std::nullopt immediately (without asking the thread)
     *      until the thread has finished or skipped every orphaned request (whose caller stopped waiting on timeout):
     *      an orphaned request is skipped if the thread has not started it yet.
     * @param trace the target stacktrace
     * @return the converted string
     * @return std::nullopt if the thread did not respond in time, the conversion failed with an exception
     *      other than std::bad_alloc, or the request was discarded before it ran
     * @throws std::bad_alloc if memory allocation failed, including on the thread of this object
     */
    [[nodiscard]] std::optional<std::string> operator()(::boost::stacktrace::stacktrace const& trace);

    /**
     * @brief enqueues a task to run on the thread of this object.
     * @details The queue is locked only briefly. A child process after fork() never reaches here
     *      (see find()), so the lock is never left held by a thread which does not exist.
     * @param task the task
     * @throws std::bad_alloc if memory allocation failed
     */
    void submit(std::function<void()> task);

private:
    std::chrono::milliseconds timeout_;
    converter_type converter_;
    std::mutex mutex_ {};
    std::condition_variable cv_ {};
    std::deque<std::function<void()>> queue_ {};
    bool stopping_ { false };
    // the number of orphaned requests (whose caller stopped waiting on timeout) which the thread has not
    // finished or skipped yet (see operator());
    // signed, because the thread may decrement it just before the caller increments it
    std::atomic<std::int64_t> orphaned_count_ { 0 };
    // must be the last member: the thread starts in the constructor and uses the members above
    std::thread thread_;

    void run();

    /**
     * @brief returns whether or not the thread is considered stalled.
     * @return true if some orphaned requests are still in flight: callers should not wait then
     */
    [[nodiscard]] bool stalled() const noexcept;
};

} // namespace takatori::util
