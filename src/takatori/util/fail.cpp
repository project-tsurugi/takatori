#include <takatori/util/fail.h>

#include <iostream>
#include <takatori/util/stacktrace.h>

namespace takatori::util {

void fail(std::string_view message) noexcept {
    if (!message.empty()) {
        std::cerr << message << '\n';
    }
    // NOTE: printed on this thread directly, not through takatori::util::stacktrace:
    // the process aborts right after this, so the per-thread state does not matter,
    // and not depending on another thread is more reliable here
    std::cerr << ::boost::stacktrace::stacktrace {}
              << '\n'
              << std::flush;
    std::abort();
}

} // namespace takatori::util
