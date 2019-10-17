#include "model/scalar/unary.h"

#include "model/tree_element_util.h"
#include "model/tree_element_forward.h"

#include "util/clonable.h"
#include "util/downcast.h"

namespace model::scalar {

unary::unary(
        unary::operator_kind_type operator_kind,
        util::unique_object_ptr<expression> operand) noexcept
    : operator_kind_(operator_kind)
    , operand_(bless_element(*this, std::move(operand)))
{}

unary::unary(
        unary::operator_kind_type operator_kind,
        expression&& operand) noexcept
    : unary(
            operator_kind,
            util::clone_unique(std::move(operand)))
{}

unary::unary(unary const& other, util::object_creator creator) noexcept
    : unary(
            other.operator_kind_,
            forward(creator, other.operand_))
{}

unary::unary(unary&& other, util::object_creator creator) noexcept
    : unary(
            other.operator_kind_,
            forward(creator, std::move(other.operand_)))
{}

expression::parent_type* unary::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* unary::parent_element() const noexcept {
    return parent_;
}

void unary::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

expression_kind unary::kind() const noexcept {
    return tag;
}

unary* unary::clone(util::object_creator creator) const& {
    return creator.create_object<unary>(*this, creator);
}

unary* unary::clone(util::object_creator creator) && {
    return creator.create_object<unary>(std::move(*this), creator);
}

unary::operator_kind_type unary::operator_kind() const noexcept {
    return operator_kind_;
}

unary& unary::operator_kind(unary::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

expression& unary::operand() noexcept {
    return *operand_;
}

expression const& unary::operand() const noexcept {
    return *operand_;
}

util::optional_ptr<expression> unary::optional_operand() noexcept {
    return util::optional_ptr { operand_.get() };
}

util::optional_ptr<expression const> unary::optional_operand() const noexcept {
    return util::optional_ptr { operand_.get() };
}

util::unique_object_ptr<expression> unary::release_operand() noexcept {
    return release_element(std::move(operand_));
}

unary& unary::operand(util::unique_object_ptr<expression> operand) noexcept {
    return assign_element(*this, operand_, std::move(operand));
}

bool operator==(unary const& a, unary const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.optional_operand() == b.optional_operand();
}

bool operator!=(unary const& a, unary const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, unary const& value) {
    return out << "unary("
               << "operator_kind=" << value.operator_kind() << ", "
               << "operand=" << value.optional_operand() << ")";
}

bool unary::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<unary>(other);
}

std::ostream& unary::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace model::scalar
