#include "takatori/scalar/let_declarator.h"
#include "takatori/scalar/let.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_element_forward.h"

namespace takatori::scalar {

let_declarator::let_declarator(let_declarator const& other, util::object_creator creator)
    : variable_declarator(other, creator)
{}

let_declarator::let_declarator(let_declarator&& other, util::object_creator creator)
    : variable_declarator(std::move(other), creator)
{}

let_declarator::parent_type* let_declarator::parent_element() noexcept {
    return parent_;
}

let_declarator::parent_type const* let_declarator::parent_element() const noexcept {
    return parent_;
}

void let_declarator::parent_element(parent_type* parent) noexcept {
    parent_ = parent;
    fire_parent_element_changed();
}

let_declarator& let_declarator::value(util::unique_object_ptr<expression> value) noexcept {
    tree::assign_element_fragment(parent_, internal_value(), std::move(value));
    return *this;
}

bool operator==(let_declarator const& a, let_declarator const& b) noexcept {
    return a.equals(b);
}

bool operator!=(let_declarator const& a, let_declarator const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, let_declarator const& value) {
    out << let::tag << "::column";
    return value.print_to(out);
}

void let_declarator::fire_parent_element_changed() noexcept {
    if (parent_ != nullptr) {
        tree::bless_element(*parent_, internal_value());
    }
}

} // namespace takatori::scalar
