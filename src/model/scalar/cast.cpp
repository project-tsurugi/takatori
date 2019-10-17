#include "model/scalar/cast.h"

#include "model/tree_element_util.h"
#include "model/tree_element_forward.h"

#include "util/clonable.h"
#include "util/downcast.h"

namespace model::scalar {

cast::cast(
        descriptor::type_descriptor type,
        cast::loss_policy_type loss_policy,
        util::unique_object_ptr<expression> operand) noexcept
    : type_(std::move(type)), loss_policy_(loss_policy), operand_(bless_element(*this, std::move(operand))) {}

cast::cast(
        descriptor::type_descriptor type,
        cast::loss_policy_type loss_policy,
        expression&& operand)
    : cast(std::move(type), loss_policy, util::clone_unique(std::move(operand))) {}

cast::cast(cast const& other, util::object_creator creator)
    : cast(other.type_, other.loss_policy_, forward(creator, other.operand_))
{}

cast::cast(cast&& other, util::object_creator creator)
    : cast(std::move(other.type_), other.loss_policy_, forward(creator, std::move(other.operand_)))
{}

expression::parent_type* cast::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* cast::parent_element() const noexcept {
    return parent_;
}

void cast::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

expression_kind cast::kind() const noexcept {
    return tag;
}

cast* cast::clone(util::object_creator creator) const& {
    return creator.create_object<cast>(*this, creator);
}

cast* cast::clone(util::object_creator creator) && {
    return creator.create_object<cast>(std::move(*this), creator);
}

descriptor::type_descriptor const& cast::type() const noexcept {
    return type_;
}

cast& cast::type(descriptor::type_descriptor type) noexcept {
    type_ = std::move(type);
    return *this;
}

cast::loss_policy_type cast::loss_policy() const noexcept {
    return loss_policy_;
}

cast& cast::loss_policy(cast::loss_policy_type loss_policy) noexcept {
    loss_policy_ = loss_policy;
    return *this;
}

expression& cast::operand() noexcept {
    return *operand_;
}

expression const& cast::operand() const noexcept {
    return *operand_;
}

util::optional_ptr<expression> cast::optional_operand() noexcept {
    return util::optional_ptr { operand_.get() };
}

util::optional_ptr<expression const> cast::optional_operand() const noexcept {
    return util::optional_ptr { operand_.get() };
}

util::unique_object_ptr<expression> cast::release_operand() noexcept {
    return release_element(std::move(operand_));
}

cast& cast::operand(util::unique_object_ptr<expression> operand) noexcept {
    return assign_element(*this, operand_, std::move(operand));
}

bool operator==(cast const& a, cast const& b) noexcept {
    return a.type() == b.type()
        && a.loss_policy() == b.loss_policy()
        && a.optional_operand() == b.optional_operand();
}

bool operator!=(cast const& a, cast const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, cast const& value) {
    return out << "cast("
               << "type=" << value.type() << ", "
               << "loss_policy=" << value.loss_policy() << ", "
               << "operand=" << value.optional_operand() << ")";
}

bool cast::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<cast>(other);
}

std::ostream& cast::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace model::scalar
