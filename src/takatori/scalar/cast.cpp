#include <takatori/scalar/cast.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::scalar {

cast::cast(
        std::shared_ptr<type::data const> type,
        cast::loss_policy_type loss_policy,
        std::unique_ptr<expression> operand) noexcept
    : type_(std::move(type))
    , loss_policy_(loss_policy)
    , operand_(tree::bless_element(*this, std::move(operand))) {}

cast::cast(
        type::data&& type,
        cast::loss_policy_type loss_policy,
        expression&& operand)
    : cast(
            util::clone_shared(std::move(type)),
            loss_policy,
            util::clone_unique(std::move(operand))) {}

cast::cast(util::clone_tag_t, cast const& other)
    : cast(
            other.type_,
            other.loss_policy_,
            tree::forward(other.operand_))
{}

cast::cast(util::clone_tag_t, cast&& other)
    : cast(
            std::move(other.type_),
            other.loss_policy_,
            tree::forward(std::move(other.operand_)))
{}

expression_kind cast::kind() const noexcept {
    return tag;
}

cast* cast::clone() const& {
    return new cast(util::clone_tag, *this); // NOLINT
}

cast* cast::clone() && {
    return new cast(util::clone_tag, std::move(*this)); // NOLINT;
}

type::data const& cast::type() const noexcept {
    return *type_;
}

util::optional_ptr<type::data const> cast::optional_type() const noexcept {
    return util::optional_ptr { type_.get() };
}

std::shared_ptr<type::data const> cast::shared_type() const noexcept {
    return type_;
}

cast& cast::type(std::shared_ptr<type::data const> type) noexcept {
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

std::unique_ptr<expression> cast::release_operand() noexcept {
    return tree::release_element(std::move(operand_));
}

cast& cast::operand(std::unique_ptr<expression> operand) noexcept {
    return tree::assign_element(*this, operand_, std::move(operand));
}

util::ownership_reference<expression> cast::ownership_operand() {
    return tree::ownership_element(*this, operand_);
}

bool operator==(cast const& a, cast const& b) noexcept {
    return a.optional_type() == b.optional_type()
        && a.loss_policy() == b.loss_policy()
        && a.optional_operand() == b.optional_operand();
}

bool operator!=(cast const& a, cast const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, cast const& value) {
    return out << value.kind() << "("
               << "data=" << value.optional_type() << ", "
               << "loss_policy=" << value.loss_policy() << ", "
               << "operand=" << value.optional_operand() << ")";
}

bool cast::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<cast>(other);
}

std::ostream& cast::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
