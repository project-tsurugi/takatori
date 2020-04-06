#include <takatori/relation/details/mapping_element.h>

#include <utility>

#include <takatori/util/hash.h>

namespace takatori::relation::details {

mapping_element::mapping_element(source_type source, destination_type destination) noexcept
    : source_(std::move(source))
    , destination_(std::move(destination))
{}

mapping_element::source_type& mapping_element::source() noexcept {
    return source_;
}

mapping_element::source_type const& mapping_element::source() const noexcept {
    return source_;
}

mapping_element::destination_type& mapping_element::destination() noexcept {
    return destination_;
}

mapping_element::destination_type const& mapping_element::destination() const noexcept {
    return destination_;
}

bool operator==(mapping_element const& a, mapping_element const& b) noexcept {
    return a.source() == b.source()
        && a.destination() == b.destination();
}

bool operator!=(mapping_element const& a, mapping_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, mapping_element const& value) {
    return out << "mapping("
               << value.source() << " => "
               << value.destination() << ")";
}

} // namespace takatori::relation::details

std::size_t std::hash<takatori::relation::details::mapping_element>::operator()(
        takatori::relation::details::mapping_element const& value) const noexcept {
    return takatori::util::hash(value.source(), value.destination());
}
