#include <takatori/relation/details/union_element.h>

#include <utility>

#include <takatori/util/hash.h>
#include <takatori/util/optional_print_support.h>

namespace takatori::relation::details {

union_element::union_element(
        std::optional<source_type> left,
        std::optional<source_type> right,
        destination_type destination) noexcept
    : left_(std::move(left))
    , right_(std::move(right))
    , destination_(std::move(destination))
{}

std::optional<union_element::source_type>& union_element::left() noexcept {
    return left_;
}

std::optional<union_element::source_type> const& union_element::left() const noexcept {
    return left_;
}

std::optional<union_element::source_type>& union_element::right() noexcept {
    return right_;
}

std::optional<union_element::source_type> const& union_element::right() const noexcept {
    return right_;
}

union_element::destination_type& union_element::destination() noexcept {
    return destination_;
}

union_element::destination_type const& union_element::destination() const noexcept {
    return destination_;
}

bool operator==(union_element const& a, union_element const& b) noexcept {
    return a.left() == b.left()
        && a.right() == b.right()
        && a.destination() == b.destination();
}

bool operator!=(union_element const& a, union_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, union_element const& value) {
    return out << "mapping("
               << "(" << util::print_support { value.left() } << ", "
                      << util::print_support { value.right() } << ")"
               << " => "
               << value.destination() << ")";
}

} // namespace takatori::relation::details

std::size_t std::hash<takatori::relation::details::union_element>::operator()(
        takatori::relation::details::union_element const& value) const noexcept {
    return takatori::util::hash(value.left(), value.right(), value.destination());
}
