#include "takatori/scalar/conditional.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_element_forward.h"
#include "tree/tree_fragment_vector_forward.h"

#include "takatori/util/downcast.h"

namespace takatori::scalar {

conditional::conditional(
        std::vector<alternative, util::pmr::polymorphic_allocator<alternative>> alternatives,
        util::unique_object_ptr<expression> default_expression) noexcept
    : alternatives_(*this, std::move(alternatives))
    , default_expression_(tree::bless_element(*this, std::move(default_expression)))
{}

conditional::conditional(
        std::initializer_list<util::rvalue_reference_wrapper<alternative>> alternatives,
        util::rvalue_ptr<expression> default_expression)
    : conditional(
            { alternatives.begin(), alternatives.end() },
            util::clone_unique(default_expression))
{}

conditional::conditional(conditional const& other, util::object_creator creator)
    : conditional(
            tree::forward(creator, other.alternatives_),
            tree::forward(creator, other.default_expression_))
{}

conditional::conditional(conditional&& other, util::object_creator creator)
    : conditional(
            tree::forward(creator, std::move(other.alternatives_)),
            tree::forward(creator, std::move(other.default_expression_)))
{}

expression_kind conditional::kind() const noexcept {
    return tag;
}

conditional* conditional::clone(util::object_creator creator) const& {
    return creator.create_object<conditional>(*this, creator);
}

conditional* conditional::clone(util::object_creator creator) && {
    return creator.create_object<conditional>(std::move(*this), creator);
}

tree::tree_fragment_vector<conditional::alternative>& conditional::alternatives() noexcept {
    return alternatives_;
}

tree::tree_fragment_vector<conditional::alternative> const& conditional::alternatives() const noexcept {
    return alternatives_;
}

util::optional_ptr<expression> conditional::default_expression() noexcept {
    return util::optional_ptr { default_expression_.get() };
}

util::optional_ptr<expression const> conditional::default_expression() const noexcept {
    return util::optional_ptr { default_expression_.get() };
}

conditional& conditional::default_expression(util::unique_object_ptr<expression> default_expression) noexcept {
    return tree::assign_element(*this, default_expression_, std::move(default_expression));
}

util::unique_object_ptr<expression> conditional::release_default_expression() noexcept {
    return tree::release_element(std::move(default_expression_));
}

bool operator==(conditional const& a, conditional const& b) noexcept {
    return a.alternatives() == b.alternatives()
        && a.default_expression() == b.default_expression();
}

bool operator!=(conditional const& a, conditional const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, conditional const& value) {
    return out << "conditional("
            << "alternatives=" << value.alternatives() << ", "
            << "default=" << value.default_expression() << ")";
}

bool conditional::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<conditional>(other);
}

std::ostream& conditional::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
