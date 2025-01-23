#include <takatori/util/fail.h>

#include <iostream>
#include <takatori/util/stacktrace.h>

namespace takatori::util {

void fail(std::string_view message) noexcept {
    if (!message.empty()) {
        std::cerr << message << '\n';
    }
    std::cerr << ::boost::stacktrace::stacktrace {}
              << '\n'
              << std::flush;
    std::abort();
}

} // namespace takatori::util
