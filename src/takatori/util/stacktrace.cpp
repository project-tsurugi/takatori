#include <takatori/util/stacktrace.h>

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <utility>

#include <dlfcn.h>

#include "symbolizer.h"

namespace takatori::util {

namespace {

/**
 * @brief returns a string of the given stacktrace made from the addresses of its frames, without function names.
 * @details This does not read debug information, and does not create any per-thread state.
 *      Each frame is printed as "<module path>+0x<offset>", which can be symbolized later by addr2line.
 *      The offset is relative to the load address of the module, which is what addr2line expects
 *      for shared libraries and position independent executables (not for non-PIE executables).
 *      Like Boost's own output, the address of each frame is its return address, used as is.
 */
std::string to_string_by_address(::boost::stacktrace::stacktrace const& trace) {
    std::ostringstream buf {};
    buf << "(stacktrace symbolization skipped: symbolizer is not available or did not respond)\n";
    std::size_t index = 0;
    for (auto const& frame : trace) {
        auto const* address = frame.address();
        buf << (index < 10 ? " " : "") << index << "# "; // NOLINT(*-magic-numbers)
        ::Dl_info info {};
        if (::dladdr(address, &info) != 0 && info.dli_fname != nullptr) {
            auto offset = reinterpret_cast<std::uintptr_t>(address) // NOLINT(*-reinterpret-cast)
                    - reinterpret_cast<std::uintptr_t>(info.dli_fbase); // NOLINT(*-reinterpret-cast)
            buf << info.dli_fname << "+0x" << std::hex << offset << std::dec;
        } else {
            buf << address;
        }
        buf << '\n';
        ++index;
    }
    return buf.str();
}

} // namespace

stacktrace::stacktrace(::boost::stacktrace::stacktrace native) noexcept :
    native_ { std::move(native) }
{}

::boost::stacktrace::stacktrace const& stacktrace::native() const noexcept {
    return native_;
}

bool stacktrace::empty() const noexcept {
    return native_.empty();
}

std::string stacktrace::to_string() const {
    if (native_.empty()) {
        return {};
    }
    if (auto* worker = symbolizer::find()) {
        if (auto result = (*worker)(native_)) {
            return std::move(*result);
        }
    }
    return to_string_by_address(native_);
}

std::ostream& operator<<(std::ostream& out, stacktrace const& value) {
    return out << value.to_string();
}

stacktrace current_stacktrace() {
    // skip this function itself
    return stacktrace { ::boost::stacktrace::stacktrace { 1, static_cast<std::size_t>(-1) } };
}

} // namespace takatori::util
