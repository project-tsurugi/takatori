#include "model/scalar/let.h"

#include "model/tree_element_util.h"
#include "model/tree_element_forward.h"
#include "model/tree_fragment_vector_forward.h"

#include "util/downcast.h"
#include "util/print_support.h"

namespace model::scalar {

let::let(
        std::vector<declarator, util::pmr::polymorphic_allocator<declarator>> variables,
        util::unique_object_ptr<expression> body) noexcept
    : variables_(*this, std::move(variables))
    , body_(bless_element(*this, std::move(body)))
{}

let::let(
        declarator&& variable,
        util::rvalue_ptr<expression> body)
    : let(
        { util::rvalue_reference_wrapper { std::move(variable) } },
        body)
{}

let::let(
        std::initializer_list<util::rvalue_reference_wrapper<declarator>> variables,
        util::rvalue_ptr<expression> body)
    : let(
            { variables.begin(), variables.end() },
            util::clone_unique(body))
{}

let::let(let const& other, util::object_creator creator)
    : let(
            forward(creator, other.variables_),
            forward(creator, other.body_))
{}

let::let(let&& other, util::object_creator creator)
    : let(
            forward(creator, std::move(other.variables_)),
            forward(creator, std::move(other.body_)))
{}

expression::parent_type* let::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* let::parent_element() const noexcept {
    return parent_;
}

void let::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

expression_kind let::kind() const noexcept {
    return tag;
}

let* let::clone(util::object_creator creator) const& {
    return creator.create_object<let>(*this, creator);
}

let* let::clone(util::object_creator creator) && {
    return creator.create_object<let>(std::move(*this), creator);
}

tree_fragment_vector<let::declarator>& let::variables() noexcept {
    return variables_;
}

tree_fragment_vector<let::declarator> const& let::variables() const noexcept {
    return variables_;
}

expression& let::body() noexcept {
    return *body_;
}

expression const& let::body() const noexcept {
    return *body_;
}

util::optional_ptr<expression> let::optional_body() noexcept {
    return util::optional_ptr { body_.get() };
}

util::optional_ptr<expression const> let::optional_body() const noexcept {
    return util::optional_ptr { body_.get() };
}

util::unique_object_ptr<expression> let::release_body() noexcept {
    return release_element(std::move(body_));
}

let& let::body(util::unique_object_ptr<expression> body) noexcept {
    return assign_element(*this, body_, std::move(body));
}

bool operator==(let const& a, let const& b) noexcept {
    return a.variables() == b.variables()
        && a.body() == b.body();
}

bool operator!=(let const& a, let const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, let const& value) {
    return out << "let("
            << "variables=" << value.variables() << ", "
            << "body=" << value.body() << ")";
}

bool let::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<let>(other);
}

std::ostream& let::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace model::scalar
