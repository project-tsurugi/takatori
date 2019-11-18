#include "takatori/relation/where.h"

#include "takatori/tree/tree_element_util.h"
#include "tree/tree_element_forward.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"

namespace takatori::relation {

where::where(util::unique_object_ptr<scalar::expression> condition) noexcept
    : condition_(tree::bless_element(*this, std::move(condition)))
{}

where::where(scalar::expression&& condition) noexcept
    : where(util::clone_unique(std::move(condition)))
{}

where::where(where const& other, util::object_creator creator)
    : where(tree::forward(creator, other.condition_))
{}

where::where(where&& other, util::object_creator creator)
    : where(tree::forward(creator, std::move(other.condition_)))
{}

expression_kind where::kind() const noexcept {
    return tag;
}

util::sequence_view<where::input_port_type> where::input_ports() noexcept {
    return util::sequence_view { &input_ };
}

util::sequence_view<where::input_port_type const> where::input_ports() const noexcept {
    return util::sequence_view { &input_ };
}

util::sequence_view<where::output_port_type> where::output_ports() noexcept {
    return util::sequence_view { &output_ };
}

util::sequence_view<where::output_port_type const> where::output_ports() const noexcept {
    return util::sequence_view { &output_ };
}

where* where::clone(util::object_creator creator) const& {
    return creator.create_object<where>(*this, creator);
}

where* where::clone(util::object_creator creator)&& {
    return creator.create_object<where>(std::move(*this), creator);
}

where::input_port_type& where::input() noexcept {
    return input_;
}

where::input_port_type const& where::input() const noexcept {
    return input_;
}

where::output_port_type& where::output() noexcept {
    return output_;
}

where::output_port_type const& where::output() const noexcept {
    return output_;
}

scalar::expression& where::condition() noexcept {
    return *condition_;
}

scalar::expression const& where::condition() const noexcept {
    return *condition_;
}

util::optional_ptr<scalar::expression> where::optional_condition() noexcept {
    return util::optional_ptr { condition_.get() };
}

util::optional_ptr<scalar::expression const> where::optional_condition() const noexcept {
    return util::optional_ptr { condition_.get() };
}

util::unique_object_ptr<scalar::expression> where::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

where& where::condition(util::unique_object_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
}

bool operator==(where const& a, where const& b) noexcept {
    return a.optional_condition() == b.optional_condition();
}

bool operator!=(where const& a, where const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, where const& value) {
    return out << value.kind() << "("
               << "condition=" << value.optional_condition() << ")";
}

bool where::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<where>(other);
}

std::ostream& where::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
