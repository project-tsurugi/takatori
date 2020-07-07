#include <takatori/relation/details/values_row.h>
#include <takatori/relation/values.h>

#include <takatori/tree/tree_element_vector_forward.h>

namespace takatori::relation::details {

values_row::values_row(util::reference_vector<scalar::expression> elements) noexcept
    : elements_(nullptr, std::move(elements))
{}

values_row::values_row(std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>> elements) noexcept
    : values_row({ elements.begin(), elements.end() })
{}

values_row::values_row(values_row const& other, util::object_creator creator)
    : values_row(tree::forward(creator, other.elements_))
{}

values_row::values_row(values_row&& other, util::object_creator creator)
    : values_row(tree::forward(creator, std::move(other.elements_)))
{}

values_row::parent_type* values_row::parent_element() noexcept {
    return parent_;
}

values_row::parent_type const* values_row::parent_element() const noexcept {
    return parent_;
}

void values_row::parent_element(values_row::parent_type* parent) noexcept {
    parent_ = parent;
    elements_.parent_element(parent);
}

tree::tree_element_vector<scalar::expression>& values_row::elements() noexcept {
    return elements_;
}

tree::tree_element_vector<scalar::expression> const& values_row::elements() const noexcept {
    return elements_;
}

std::ostream& operator<<(std::ostream& out, values_row const& value) {
    return out << "row("
               << "elements=" << value.elements() << ")";
}

bool operator==(values_row const& a, values_row const& b) noexcept {
    return a.elements() == b.elements();
}

bool operator!=(values_row const& a, values_row const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::relation::details
