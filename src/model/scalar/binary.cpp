#include "model/scalar/binary.h"

#include "model/tree_element_util.h"
#include "model/tree_element_forward.h"

#include "util/clonable.h"
#include "util/downcast.h"

namespace model::scalar {

binary::binary(
        binary::operator_kind_type operator_kind,
        util::unique_object_ptr<expression> left,
        util::unique_object_ptr<expression> right) noexcept
    : operator_kind_(operator_kind)
    , left_(bless_element(*this, std::move(left)))
    , right_(bless_element(*this, std::move(right)))
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

binary::binary(binary const& other, util::object_creator creator) noexcept
    : binary(
            other.operator_kind_,
            forward(creator, other.left_),
            forward(creator, other.right_))
{}

binary::binary(binary&& other, util::object_creator creator) noexcept
    : binary(
            other.operator_kind_,
            forward(creator, std::move(other.left_)),
            forward(creator, std::move(other.right_)))
{}

expression::parent_type* binary::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* binary::parent_element() const noexcept {
    return parent_;
}

void binary::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

expression_kind binary::kind() const noexcept {
    return tag;
}

binary* binary::clone(util::object_creator creator) const& {
    return creator.create_object<binary>(*this, creator);
}

binary* binary::clone(util::object_creator creator) && {
    return creator.create_object<binary>(std::move(*this), creator);
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

util::unique_object_ptr<expression> binary::release_left() noexcept {
    return release_element(std::move(left_));
}

binary& binary::left(util::unique_object_ptr<expression> left) noexcept {
    return assign_element(*this, left_, std::move(left));
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

util::unique_object_ptr<expression> binary::release_right() noexcept {
    return release_element(std::move(right_));
}

binary& binary::right(util::unique_object_ptr<expression> right) noexcept {
    return assign_element(*this, right_, std::move(right));
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
    return out << "binary("
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

} // namespace model::scalar
