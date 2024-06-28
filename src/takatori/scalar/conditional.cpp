#include <takatori/scalar/conditional.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

conditional::conditional(
        std::vector<alternative> alternatives,
        std::unique_ptr<expression> default_expression) noexcept
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

conditional::conditional(util::clone_tag_t, conditional const& other)
    : conditional(
            tree::forward(other.alternatives_),
            tree::forward(other.default_expression_))
{}

conditional::conditional(util::clone_tag_t, conditional&& other)
    : conditional(
            tree::forward(std::move(other.alternatives_)),
            tree::forward(std::move(other.default_expression_)))
{}

expression_kind conditional::kind() const noexcept {
    return tag;
}

conditional* conditional::clone() const& {
    return new conditional(util::clone_tag, *this); // NOLINT
}

conditional* conditional::clone() && {
    return new conditional(util::clone_tag, std::move(*this)); // NOLINT;
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

conditional& conditional::default_expression(std::unique_ptr<expression> default_expression) noexcept {
    return tree::assign_element(*this, default_expression_, std::move(default_expression));
}

util::ownership_reference<expression> conditional::ownership_default_expression() {
    return tree::ownership_element(*this, default_expression_);
}

std::unique_ptr<expression> conditional::release_default_expression() noexcept {
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
    return out << value.kind() << "("
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
