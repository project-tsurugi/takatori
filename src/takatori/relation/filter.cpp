#include <takatori/relation/filter.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation {

filter::filter(std::unique_ptr<scalar::expression> condition) noexcept
    : input_(*this, 0)
    , output_(*this, 0)
    , condition_(tree::bless_element(*this, std::move(condition)))
{}

filter::filter(scalar::expression&& condition) noexcept
    : filter(util::clone_unique(std::move(condition)))
{}

filter::filter(util::clone_tag_t, filter const& other)
    : filter(
            tree::forward(other.condition_))
{}

filter::filter(util::clone_tag_t, filter&& other)
    : filter(
            tree::forward(std::move(other.condition_)))
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

filter* filter::clone() const& {
    return new filter(util::clone_tag, *this); // NOLINT
}

filter* filter::clone()&& {
    return new filter(util::clone_tag, std::move(*this)); // NOLINT;
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

std::unique_ptr<scalar::expression> filter::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

filter& filter::condition(std::unique_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
}

util::ownership_reference<scalar::expression> filter::ownership_condition() {
    return tree::ownership_element(*this, condition_);
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
