#include <takatori/scalar/compare.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::scalar {

compare::compare(
        operator_kind_type operator_kind,
        std::unique_ptr<expression> left,
        std::unique_ptr<expression> right) noexcept
    : operator_kind_(operator_kind)
    , left_(tree::bless_element(*this, std::move(left)))
    , right_(tree::bless_element(*this, std::move(right)))
{}

compare::compare(
        operator_kind_type operator_kind,
        expression&& left,
        expression&& right) noexcept
    : compare(
            operator_kind,
            util::clone_unique(std::move(left)),
            util::clone_unique(std::move(right)))
{}

compare::compare(util::clone_tag_t, compare const& other) noexcept
    : compare(
            other.operator_kind_,
            tree::forward(other.left_),
            tree::forward(other.right_))
{}

compare::compare(util::clone_tag_t, compare&& other) noexcept
    : compare(
            other.operator_kind_,
            tree::forward(std::move(other.left_)),
            tree::forward(std::move(other.right_)))
{}

expression_kind compare::kind() const noexcept {
    return tag;
}

compare* compare::clone() const& {
    return new compare(util::clone_tag, *this); // NOLINT
}

compare* compare::clone() && {
    return new compare(util::clone_tag, std::move(*this)); // NOLINT;
}

compare::operator_kind_type compare::operator_kind() const noexcept {
    return operator_kind_;
}

compare& compare::operator_kind(compare::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

expression& compare::left() noexcept {
    return *left_;
}

expression const& compare::left() const noexcept {
    return *left_;
}

util::optional_ptr<expression> compare::optional_left() noexcept {
    return util::optional_ptr { left_.get() };
}

util::optional_ptr<expression const> compare::optional_left() const noexcept {
    return util::optional_ptr { left_.get() };
}

std::unique_ptr<expression> compare::release_left() noexcept {
    return tree::release_element(std::move(left_));
}

compare& compare::left(std::unique_ptr<expression> left) noexcept {
    return tree::assign_element(*this, left_, std::move(left));
}

util::ownership_reference<expression> compare::ownership_left() {
    return tree::ownership_element(*this, left_);
}

expression& compare::right() noexcept {
    return *right_;
}

expression const& compare::right() const noexcept {
    return *right_;
}

util::optional_ptr<expression> compare::optional_right() noexcept {
    return util::optional_ptr { right_.get() };
}

util::optional_ptr<expression const> compare::optional_right() const noexcept {
    return util::optional_ptr { right_.get() };
}

std::unique_ptr<expression> compare::release_right() noexcept {
    return tree::release_element(std::move(right_));
}

compare& compare::right(std::unique_ptr<expression> right) noexcept {
    return tree::assign_element(*this, right_, std::move(right));
}

util::ownership_reference<expression> compare::ownership_right() {
    return tree::ownership_element(*this, right_);
}

bool operator==(compare const& a, compare const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.optional_left() == b.optional_left()
        && a.optional_right() == b.optional_right();
}

bool operator!=(compare const& a, compare const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, compare const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "left=" << value.optional_left() << ", "
               << "right=" << value.optional_right() << ")";
}

bool compare::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<compare>(other);
}

std::ostream& compare::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
