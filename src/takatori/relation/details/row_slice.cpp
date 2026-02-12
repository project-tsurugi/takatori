#include <takatori/relation/details/row_slice.h>

#include <takatori/util/optional_print_support.h>

namespace takatori::relation::details {

row_slice::row_slice(
        position_type start,
        size_type count) noexcept:
    start_ { std::move(start) },
    count_ { std::move(count) }
{}

row_slice::operator bool() const noexcept {
    return static_cast<bool>(start_) || static_cast<bool>(count_);
}

row_slice::position_type& row_slice::start() noexcept {
    return start_;
}

const row_slice::position_type& row_slice::start() const noexcept {
    return start_;
}

row_slice::size_type& row_slice::count() noexcept {
    return count_;
}

const row_slice::size_type& row_slice::count() const noexcept {
    return count_;
}

bool operator==(row_slice const& lhs, row_slice const& rhs) noexcept {
    return lhs.start_ == rhs.start_
            && lhs.count_ == rhs.count_;
}

bool operator!=(row_slice const& lhs, row_slice const& rhs) noexcept {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& out, row_slice const& slice) {
    return out << "row_slice("
               << "start=" << slice.start_ << ", "
               << "count=" << slice.count_ << ")";
}

} // namespace takatori::relation::details
