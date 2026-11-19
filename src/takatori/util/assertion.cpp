#include <takatori/util/assertion.h>

#include <iostream>
#include <takatori/util/stacktrace.h>

namespace boost {

[[noreturn]] static inline void assertion_failed_with_trace(
        char const* expr,
        char const* msg,
        char const* function,
        ::boost::stacktrace::stacktrace const& trace = {}) {
    std::cerr << "Assertion '" << expr << "'"
              << " was failed in '" << function << "'";
    if (msg != nullptr) {
        std::cerr << " : " << msg;
    }
    // NOTE: printed on this thread directly, not through takatori::util::stacktrace:
    // the process aborts right after this, so the per-thread state does not matter,
    // and not depending on another thread is more reliable here
    std::cerr << '\n'
              << trace
              << '\n'
              << std::flush;
    std::abort();
}

void assertion_failed(char const* expr, char const* function, char const*, long) { // NOLINT
    assertion_failed_with_trace(expr, nullptr, function);
}

void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const*, long) { // NOLINT
    assertion_failed_with_trace(expr, msg, function);
}

} // namespace boost
