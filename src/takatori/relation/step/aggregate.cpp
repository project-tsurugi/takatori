#include <takatori/relation/step/aggregate.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::step {

aggregate::aggregate(std::vector<column> columns) noexcept
    : input_(*this, 0)
    , output_(*this, 0)
    , columns_(std::move(columns))
{}

aggregate::aggregate(std::initializer_list<column> columns)
    : aggregate({ columns.begin(), columns.end() })
{}


aggregate::aggregate(util::clone_tag_t, aggregate const& other)
    : aggregate(
            decltype(columns_) { other.columns_ })
{}

aggregate::aggregate(util::clone_tag_t, aggregate&& other)
    : aggregate(
            decltype(columns_) { std::move(other.columns_) })
{}

expression_kind aggregate::kind() const noexcept {
    return tag;
}

aggregate* aggregate::clone() const& {
    return new aggregate(util::clone_tag, *this); // NOLINT
}

aggregate* aggregate::clone()&& {
    return new aggregate(util::clone_tag, std::move(*this)); // NOLINT;
}


util::sequence_view<aggregate::input_port_type> aggregate::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<aggregate::input_port_type const> aggregate::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<aggregate::output_port_type> aggregate::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<aggregate::output_port_type const> aggregate::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

aggregate::input_port_type& aggregate::input() noexcept {
    return input_;
}

aggregate::input_port_type const& aggregate::input() const noexcept {
    return input_;
}

aggregate::output_port_type& aggregate::output() noexcept {
    return output_;
}

aggregate::output_port_type const& aggregate::output() const noexcept {
    return output_;
}

std::vector<aggregate::column>& aggregate::columns() noexcept {
    return columns_;
}

std::vector<aggregate::column> const& aggregate::columns() const noexcept {
    return columns_;
}

bool operator==(aggregate const& a, aggregate const& b) noexcept {
    return a.columns() == b.columns();
}

bool operator!=(aggregate const& a, aggregate const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, aggregate const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool aggregate::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<aggregate>(other);
}

std::ostream& aggregate::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
