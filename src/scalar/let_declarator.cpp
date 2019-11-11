#include "takatori/scalar/let_declarator.h"
#include "takatori/scalar/let.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_element_forward.h"

namespace takatori::scalar {

let_declarator::let_declarator(
        descriptor::variable variable,
        util::unique_object_ptr<expression> value) noexcept
    : variable_(std::move(variable))
    , value_(std::move(value))
{}

let_declarator::let_declarator(
        descriptor::variable variable,
        expression&& value) noexcept
    : let_declarator(
            std::move(variable),
            util::clone_unique(std::move(value)))
{}

let_declarator::let_declarator(
        let_declarator const& other,
        util::object_creator creator)
    : let_declarator(
            other.variable_,
            tree::forward(creator, other.value_))
{}

let_declarator::let_declarator(
        let_declarator&& other,
        util::object_creator creator)
    : let_declarator(
            std::move(other.variable_),
            tree::forward(creator, std::move(other.value_)))
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

descriptor::variable const& let_declarator::variable() const noexcept {
    return variable_;
}

let_declarator& let_declarator::variable(descriptor::variable variable) noexcept {
    variable_ = std::move(variable);
    return *this;
}

expression& let_declarator::value() noexcept {
    return *value_;
}

expression const& let_declarator::value() const noexcept {
    return *value_;
}

util::optional_ptr<expression> let_declarator::optional_value() noexcept {
    return util::optional_ptr { value_.get() };
}

util::optional_ptr<expression const> let_declarator::optional_value() const noexcept {
    return util::optional_ptr { value_.get() };
}

let_declarator& let_declarator::value(util::unique_object_ptr<expression> value) noexcept {
    tree::assign_element_fragment(parent_, value_, std::move(value));
    return *this;
}

util::unique_object_ptr<expression> let_declarator::release_value() noexcept {
    return tree::release_element(std::move(value_));
}

bool operator==(let_declarator const& a, let_declarator const& b) noexcept {
    return a.variable() == b.variable()
        && a.optional_value() == b.optional_value();
}

bool operator!=(let_declarator const& a, let_declarator const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, let_declarator const& value) {
    return out << "alternative("
               << "variable=" << value.variable() << ", "
               << "value=" << value.optional_value() << ")";
}

void let_declarator::fire_parent_element_changed() noexcept {
    if (parent_ != nullptr) {
        tree::bless_element(*parent_, value_);
    }
}

} // namespace takatori::scalar
