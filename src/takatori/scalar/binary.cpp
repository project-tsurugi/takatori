#include <takatori/scalar/binary.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::scalar {

binary::binary(
        binary::operator_kind_type operator_kind,
        std::unique_ptr<expression> left,
        std::unique_ptr<expression> right) noexcept
    : operator_kind_(operator_kind)
    , left_(tree::bless_element(*this, std::move(left)))
    , right_(tree::bless_element(*this, std::move(right)))
{}

binary::binary(
        binary::operator_kind_type operator_kind,
        expression&& left,
        expression&& right) noexcept
    : binary(
            operator_kind,
            util::clone_unique(std::move(left)),
            util::clone_unique(std::move(right)))
{}

binary::binary(util::clone_tag_t, binary const& other) noexcept
    : binary(
            other.operator_kind_,
            tree::forward(other.left_),
            tree::forward(other.right_))
{}

binary::binary(util::clone_tag_t, binary&& other) noexcept
    : binary(
            other.operator_kind_,
            tree::forward(std::move(other.left_)),
            tree::forward(std::move(other.right_)))
{}

expression_kind binary::kind() const noexcept {
    return tag;
}

binary* binary::clone() const& {
    return new binary(util::clone_tag, *this); // NOLINT
}

binary* binary::clone() && {
    return new binary(util::clone_tag, std::move(*this)); // NOLINT;
}

binary::operator_kind_type binary::operator_kind() const noexcept {
    return operator_kind_;
}

binary& binary::operator_kind(binary::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

expression& binary::left() noexcept {
    return *left_;
}

expression const& binary::left() const noexcept {
    return *left_;
}

util::optional_ptr<expression> binary::optional_left() noexcept {
    return util::optional_ptr { left_.get() };
}

util::optional_ptr<expression const> binary::optional_left() const noexcept {
    return util::optional_ptr { left_.get() };
}

std::unique_ptr<expression> binary::release_left() noexcept {
    return tree::release_element(std::move(left_));
}

binary& binary::left(std::unique_ptr<expression> left) noexcept {
    return tree::assign_element(*this, left_, std::move(left));
}

util::ownership_reference<expression> binary::ownership_left() {
    return tree::ownership_element(*this, left_);
}

expression& binary::right() noexcept {
    return *right_;
}

expression const& binary::right() const noexcept {
    return *right_;
}

util::optional_ptr<expression> binary::optional_right() noexcept {
    return util::optional_ptr { right_.get() };
}

util::optional_ptr<expression const> binary::optional_right() const noexcept {
    return util::optional_ptr { right_.get() };
}

std::unique_ptr<expression> binary::release_right() noexcept {
    return tree::release_element(std::move(right_));
}

binary& binary::right(std::unique_ptr<expression> right) noexcept {
    return tree::assign_element(*this, right_, std::move(right));
}

util::ownership_reference<expression> binary::ownership_right() {
    return tree::ownership_element(*this, right_);
}

bool operator==(binary const& a, binary const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.optional_left() == b.optional_left()
        && a.optional_right() == b.optional_right();
}

bool operator!=(binary const& a, binary const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, binary const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "left=" << value.optional_left() << ", "
               << "right=" << value.optional_right() << ")";
}

bool binary::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<binary>(other);
}

std::ostream& binary::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
