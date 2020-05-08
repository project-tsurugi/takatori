#include <takatori/util/exception.h>

#include <iostream>

namespace takatori::util {

::boost::stacktrace::stacktrace const* find_trace(std::exception const& exception) {
    return ::boost::get_error_info<stacktrace_info>(exception);
}

void print_trace(std::ostream& out, std::exception const& exception) {
    if (auto const* st = find_trace(exception); st != nullptr) {
        out << *st;
    }
}

} // namespace takatori::util
