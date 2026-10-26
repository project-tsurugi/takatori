#include <takatori/relation/details/row_slice_compat.h>

#include <stdexcept>

#include <takatori/util/exception.h>

namespace takatori::relation::details {

using ::takatori::util::throw_exception;

std::optional<std::size_t> get_compatible_row_count(row_slice const& slice) {
    if (auto&& start = slice.start(); !start.is_absent()) {
        if (auto value = start.immediate_if(); !value || *value != 0) {
            throw_exception(std::domain_error("row slice with start position is not supported in the compatibility API"));
        }
    }
    auto&& count = slice.count();
    if (count.is_absent()) {
        return std::nullopt;
    }
    if (auto value = count.immediate_if()) {
        return *value;
    }
    throw_exception(std::domain_error("row slice with variable count is not supported in the compatibility API"));
}

void set_compatible_row_count(row_slice& slice, std::optional<std::size_t> count) {
    if (count) {
        slice.count() = *count;
    } else {
        slice.count() = std::nullopt;
    }
}

} // namespace takatori::relation::details
