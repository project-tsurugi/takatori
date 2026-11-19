#pragma once

// FIXME: move to common core module

#if defined(BOOST_STACKTRACE_USE_BACKTRACE) \
        && !defined(BOOST_STACKTRACE_BACKTRACE_INCLUDE_FILE) \
        && !__has_include(<backtrace.h>)

// NOTE: clang-tidy cannot detect <backtrace.h>
#undef BOOST_STACKTRACE_USE_BACKTRACE

#endif

#include <ostream>
#include <string>

#include <boost/stacktrace/stacktrace.hpp>

namespace takatori::util {

/**
 * @brief a captured stacktrace, which is symbolized on a dedicated thread.
 * @details Boost.Stacktrace (libbacktrace) keeps its symbolization state per thread and never releases it,
 *      so converting stacktraces on many short-lived threads leaks memory for each thread.
 *      This class symbolizes on a single long-lived thread to keep the number of states to one
 *      (see to_string() for when it prints a stacktrace made from addresses only instead).
 *
 *      The state remembers the modules loaded when it is first used, so frames in modules loaded later
 *      (e.g. by dlopen()) may be printed with fewer details (e.g. without names of non-exported functions
 *      or line numbers).
 */
class stacktrace {
public:
    /**
     * @brief creates a new empty instance.
     */
    stacktrace() = default;

    /**
     * @brief creates a new instance.
     * @param native the captured stacktrace
     */
    explicit stacktrace(::boost::stacktrace::stacktrace native) noexcept;

    /**
     * @brief returns the captured stacktrace.
     * @return the captured stacktrace
     * @warning Do not write the returned object into output streams directly:
     *      it is converted to a string on the calling thread, and leaks memory for each thread.
     *      Use to_string() or operator<<() of this class instead.
     */
    [[nodiscard]] ::boost::stacktrace::stacktrace const& native() const noexcept;

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief returns a human readable string of this stacktrace.
     * @details This can be called from any threads.
     *      This asks the dedicated thread to convert this stacktrace, and waits for the result.
     *      If the dedicated thread is not available (disabled in this build, in a child process after fork()
     *      without exec, or if the thread could not be started), does not respond within 10 seconds,
     *      or the conversion fails with an exception other than std::bad_alloc,
     *      this returns a stacktrace made from addresses only instead: its first line tells so,
     *      and each frame is shown as "<module path>+0x<offset>", which can be symbolized later
     *      (e.g. `addr2line -f -C -i -e <module path> <offset>`).
     *      Once a request timed out, later calls return that form without waiting,
     *      until the dedicated thread catches up (it never does if a conversion never returns).
     * @return the string
     * @return empty string if this is empty
     * @throws std::bad_alloc if a C++ memory allocation failed
     *      (allocation failures inside libbacktrace are not reported; they only give fewer details)
     * @warning This is not async-signal-safe: do not call this (or operator<<()) from signal handlers.
     *      It allocates memory, and may wait for a lock which the interrupted code holds.
     */
    [[nodiscard]] std::string to_string() const;

private:
    // NOTE: { 0, 0 } (skip 0, max depth 0) makes it empty; `{}` would capture the current stack
    ::boost::stacktrace::stacktrace native_ { 0, 0 };
};

/**
 * @brief appends a human readable string of the given stacktrace into the output stream.
 * @details This writes the result of stacktrace::to_string(): see it for how the string is made.
 * @param out the output stream
 * @param value the target stacktrace
 * @return the output stream
 * @throws std::bad_alloc if a C++ memory allocation failed (see stacktrace::to_string())
 * @warning This is not async-signal-safe: do not call this from signal handlers.
 *      It allocates memory, and may wait for a lock which the interrupted code holds.
 */
std::ostream& operator<<(std::ostream& out, stacktrace const& value);

/**
 * @brief captures the current stacktrace.
 * @return the captured stacktrace
 */
[[nodiscard]] stacktrace current_stacktrace();

} // namespace takatori::util
