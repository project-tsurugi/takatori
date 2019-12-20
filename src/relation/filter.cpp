#include <takatori/relation/filter.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation {

filter::filter(
        util::unique_object_ptr<scalar::expression> condition,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , condition_(tree::bless_element(*this, std::move(condition)))
{}

filter::filter(scalar::expression&& condition) noexcept
    : filter(util::clone_unique(std::move(condition)))
{}

filter::filter(filter const& other, util::object_creator creator)
    : filter(
            tree::forward(creator, other.condition_),
            creator)
{}

filter::filter(filter&& other, util::object_creator creator)
    : filter(
            tree::forward(creator, std::move(other.condition_)),
            creator)
{}

expression_kind filter::kind() const noexcept {
    return tag;
}

util::sequence_view<filter::input_port_type> filter::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<filter::input_port_type const> filter::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<filter::output_port_type> filter::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<filter::output_port_type const> filter::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

filter* filter::clone(util::object_creator creator) const& {
    return creator.create_object<filter>(*this, creator);
}

filter* filter::clone(util::object_creator creator)&& {
    return creator.create_object<filter>(std::move(*this), creator);
}

filter::input_port_type& filter::input() noexcept {
    return input_;
}

filter::input_port_type const& filter::input() const noexcept {
    return input_;
}

filter::output_port_type& filter::output() noexcept {
    return output_;
}

filter::output_port_type const& filter::output() const noexcept {
    return output_;
}

scalar::expression& filter::condition() noexcept {
    return *condition_;
}

scalar::expression const& filter::condition() const noexcept {
    return *condition_;
}

util::optional_ptr<scalar::expression> filter::optional_condition() noexcept {
    return util::optional_ptr { condition_.get() };
}

util::optional_ptr<scalar::expression const> filter::optional_condition() const noexcept {
    return util::optional_ptr { condition_.get() };
}

util::unique_object_ptr<scalar::expression> filter::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

filter& filter::condition(util::unique_object_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
}

bool operator==(filter const& a, filter const& b) noexcept {
    return a.optional_condition() == b.optional_condition();
}

bool operator!=(filter const& a, filter const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, filter const& value) {
    return out << value.kind() << "("
               << "condition=" << value.optional_condition() << ")";
}

bool filter::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<filter>(other);
}

std::ostream& filter::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
