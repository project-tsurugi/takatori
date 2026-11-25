#include <takatori/statement/details/partition_criteria.h>

#include <takatori/util/vector_print_support.h>

namespace takatori::statement::details {

partition_criteria::partition_criteria(std::vector<partition_key_element> elements) noexcept :
    elements_ { std::move(elements) }
{}

std::vector<partition_key_element>& partition_criteria::elements() noexcept {
    return elements_;
}

std::vector<partition_key_element> const& partition_criteria::elements() const noexcept {
    return elements_;
}

bool operator==(partition_criteria const& a, partition_criteria const& b) noexcept {
    return a.elements() == b.elements();
}

bool operator!=(partition_criteria const& a, partition_criteria const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, partition_criteria const& value) {
    using util::print_support;
    return out << "partition_criteria("
               << "elements=" << print_support { value.elements() } << ")";
}

} // namespace takatori::statement::details
