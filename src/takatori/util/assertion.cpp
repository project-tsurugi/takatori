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
    std::cerr << std::endl
              << trace
              << std::endl;
    std::abort();
}

void assertion_failed(char const* expr, char const* function, char const*, long) { // NOLINT
    assertion_failed_with_trace(expr, nullptr, function);
}

void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const*, long) { // NOLINT
    assertion_failed_with_trace(expr, msg, function);
}

} // namespace boost
