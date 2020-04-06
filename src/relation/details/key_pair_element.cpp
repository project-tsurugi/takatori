#include <takatori/relation/details/key_pair_element.h>

#include <utility>

#include <takatori/util/hash.h>

namespace takatori::relation::details {

key_pair_element::key_pair_element(
        descriptor::variable left,
        descriptor::variable right) noexcept
    : left_(std::move(left))
    , right_(std::move(right))
{}

descriptor::variable& key_pair_element::left() noexcept {
    return left_;
}

descriptor::variable const& key_pair_element::left() const noexcept {
    return left_;
}

descriptor::variable& key_pair_element::right() noexcept {
    return right_;
}

descriptor::variable const& key_pair_element::right() const noexcept {
    return right_;
}

bool operator==(key_pair_element const& a, key_pair_element const& b) noexcept {
    return a.left() == b.left()
        && a.right() == b.right();
}

bool operator!=(key_pair_element const& a, key_pair_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, key_pair_element const& value) {
    return out << "key_pair("
               << value.left() << ", "
               << value.right() << ")";
}

} // namespace takatori::relation::details

std::size_t std::hash<takatori::relation::details::key_pair_element>::operator()(
        takatori::relation::details::key_pair_element const& value) const noexcept {
    return takatori::util::hash(value.left(), value.right());
}
