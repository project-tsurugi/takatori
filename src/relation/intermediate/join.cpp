#include <takatori/relation/intermediate/join.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation::intermediate {


join::join(
        operator_kind_type operator_kind,
        util::unique_object_ptr<scalar::expression> condition,
        util::object_creator creator) noexcept
    : inputs_({
        input_port_type { *this, left_index, creator },
        input_port_type { *this, right_index, creator },
    })
    , output_(*this, 0, creator)
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

join::join(join const& other, util::object_creator creator)
    : join(
            other.operator_kind_,
            tree::forward(creator, other.condition_),
            creator)
{}

join::join(join&& other, util::object_creator creator)
    : join(
            other.operator_kind_,
            tree::forward(creator, std::move(other.condition_)),
            creator)
{}

expression_kind join::kind() const noexcept {
    return tag;
}

util::sequence_view<join::input_port_type> join::input_ports() noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<join::input_port_type const> join::input_ports() const noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<join::output_port_type> join::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<join::output_port_type const> join::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

join* join::clone(util::object_creator creator) const& {
    return creator.create_object<join>(*this, creator);
}

join* join::clone(util::object_creator creator)&& {
    return creator.create_object<join>(std::move(*this), creator);
}

join::input_port_type& join::left() noexcept {
    return inputs_[left_index];
}

join::input_port_type const& join::left() const noexcept {
    return inputs_[left_index];
}

join::input_port_type& join::right() noexcept {
    return inputs_[right_index];
}

join::input_port_type const& join::right() const noexcept {
    return inputs_[right_index];
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

util::unique_object_ptr<scalar::expression> join::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

join& join::condition(util::unique_object_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
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

} // namespace takatori::relation::intermediate
