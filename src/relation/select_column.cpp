#include "takatori/relation/select_column.h"
#include "takatori/relation/select.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_element_forward.h"

namespace takatori::relation {

select_column::select_column(select_column const& other, util::object_creator creator)
    : variable_declarator(other, creator)
{}

select_column::select_column(select_column&& other, util::object_creator creator)
    : variable_declarator(other, creator)
{}

select_column::parent_type* select_column::parent_element() noexcept {
    return parent_;
}

select_column::parent_type const* select_column::parent_element() const noexcept {
    return parent_;
}

void select_column::parent_element(parent_type* parent) noexcept {
    parent_ = parent;
    fire_parent_element_changed();
}

select_column& select_column::value(util::unique_object_ptr<scalar::expression> value) noexcept {
    tree::assign_element_fragment(parent_, internal_value(), std::move(value));
    return *this;
}

bool operator==(select_column const& a, select_column const& b) noexcept {
    return a.equals(b);
}

bool operator!=(select_column const& a, select_column const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, select_column const& value) {
    out << select::tag << "::column";
    return value.print_to(out);
}

void select_column::fire_parent_element_changed() noexcept {
    if (parent_ != nullptr) {
        tree::bless_element(*parent_, internal_value());
    }
}

} // namespace takatori::relation
