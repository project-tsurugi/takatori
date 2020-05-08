#include <takatori/util/fail.h>

#include <iostream>
#include <takatori/util/stacktrace.h>

namespace takatori::util {

void fail(std::string_view message) noexcept {
    if (!message.empty()) {
        std::cerr << message << std::endl;
    }
    std::cerr << ::boost::stacktrace::stacktrace {}
              << std::endl;
    std::abort();
}

} // namespace takatori::util
