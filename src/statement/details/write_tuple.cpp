#include <takatori/statement/details/write_tuple.h>
#include <takatori/statement/write.h>

#include <takatori/tree/tree_element_vector_forward.h>

namespace takatori::statement::details {

write_tuple::write_tuple(util::reference_vector<scalar::expression> elements) noexcept
    : elements_(nullptr, std::move(elements))
{}

write_tuple::write_tuple(std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>> elements) noexcept
    : write_tuple({ elements.begin(), elements.end() })
{}

write_tuple::write_tuple(write_tuple const& other, util::object_creator creator)
    : write_tuple(tree::forward(creator, other.elements_))
{}

write_tuple::write_tuple(write_tuple&& other, util::object_creator creator)
    : write_tuple(tree::forward(creator, std::move(other.elements_)))
{}

write_tuple::parent_type* write_tuple::parent_element() noexcept {
    return parent_;
}

write_tuple::parent_type const* write_tuple::parent_element() const noexcept {
    return parent_;
}

void write_tuple::parent_element(write_tuple::parent_type* parent) noexcept {
    parent_ = parent;
    elements_.parent_element(parent);
}

tree::tree_element_vector<scalar::expression>& write_tuple::elements() noexcept {
    return elements_;
}

tree::tree_element_vector<scalar::expression> const& write_tuple::elements() const noexcept {
    return elements_;
}

std::ostream& operator<<(std::ostream& out, write_tuple const& value) {
    return out << "tuple("
               << "elements=" << value.elements() << ")";
}

bool operator==(write_tuple const& a, write_tuple const& b) noexcept {
    return a.elements() == b.elements();
}

bool operator!=(write_tuple const& a, write_tuple const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::statement::details
