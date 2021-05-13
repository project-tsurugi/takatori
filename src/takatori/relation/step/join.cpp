#include <takatori/relation/step/join.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation::step {


join::join(
        operator_kind_type operator_kind,
        std::unique_ptr<scalar::expression> condition) noexcept
    : input_(*this, 0)
    , output_(*this, 0)
    , operator_kind_(operator_kind)
    , condition_(tree::bless_element(*this, std::move(condition)))
{}

join::join(
        operator_kind_type operator_kind,
        util::rvalue_ptr<scalar::expression> condition) noexcept
    : join(
            operator_kind,
            util::clone_unique(condition))
{}

join::join(util::clone_tag_t, join const& other)
    : join(
            other.operator_kind_,
            tree::forward(other.condition_))
{}

join::join(util::clone_tag_t, join&& other)
    : join(
            other.operator_kind_,
            tree::forward(std::move(other.condition_)))
{}

expression_kind join::kind() const noexcept {
    return tag;
}

util::sequence_view<join::input_port_type> join::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<join::input_port_type const> join::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<join::output_port_type> join::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<join::output_port_type const> join::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

join* join::clone() const& {
    return new join(util::clone_tag, *this); // NOLINT
}

join* join::clone()&& {
    return new join(util::clone_tag, std::move(*this)); // NOLINT;
}

join::input_port_type& join::input() noexcept {
    return input_;
}

join::input_port_type const& join::input() const noexcept {
    return input_;
}

join::output_port_type& join::output() noexcept {
    return output_;
}

join::output_port_type const& join::output() const noexcept {
    return output_;
}

join::operator_kind_type join::operator_kind() const noexcept {
    return operator_kind_;
}

join& join::operator_kind(join::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

util::optional_ptr<scalar::expression> join::condition() noexcept {
    return util::optional_ptr { condition_.get() };
}

util::optional_ptr<scalar::expression const> join::condition() const noexcept {
    return util::optional_ptr { condition_.get() };
}

std::unique_ptr<scalar::expression> join::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

join& join::condition(std::unique_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
}

util::ownership_reference<scalar::expression> join::ownership_condition() noexcept {
    return tree::ownership_element(*this, condition_);
}

bool operator==(join const& a, join const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.condition() == b.condition();
}

bool operator!=(join const& a, join const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, join const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "condition=" << value.condition() << ")";
}

bool join::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<join>(other);
}

std::ostream& join::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
