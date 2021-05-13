#include <takatori/relation/step/offer.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::step {

offer::offer(
        descriptor::relation destination,
        std::vector<column> columns) noexcept
    : input_(*this, 0)
    , destination_(std::move(destination))
    , columns_(std::move(columns))
{}

offer::offer(
        descriptor::relation destination,
        std::initializer_list<column> columns)
    : offer(
            std::move(destination),
            { columns.begin(), columns.end() })
{}


offer::offer(util::clone_tag_t, offer const& other)
    : offer(
            other.destination_,
            { other.columns_ })
{}

offer::offer(util::clone_tag_t, offer&& other)
    : offer(
            std::move(other.destination_),
            { std::move(other.columns_) })
{}

expression_kind offer::kind() const noexcept {
    return tag;
}

offer* offer::clone() const& {
    return new offer(util::clone_tag, *this); // NOLINT
}

offer* offer::clone()&& {
    return new offer(util::clone_tag, std::move(*this)); // NOLINT;
}


util::sequence_view<offer::input_port_type> offer::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<offer::input_port_type const> offer::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<offer::output_port_type> offer::output_ports() noexcept {
    return {};
}

util::sequence_view<offer::output_port_type const> offer::output_ports() const noexcept {
    return {};
}

offer::input_port_type& offer::input() noexcept {
    return input_;
}

offer::input_port_type const& offer::input() const noexcept {
    return input_;
}

descriptor::relation const& offer::destination() const noexcept {
    return destination_;
}

offer& offer::destination(descriptor::relation destination) noexcept {
    destination_ = std::move(destination);
    return *this;
}

std::vector<offer::column>& offer::columns() noexcept {
    return columns_;
}

std::vector<offer::column> const& offer::columns() const noexcept {
    return columns_;
}

bool operator==(offer const& a, offer const& b) noexcept {
    return a.destination() == b.destination()
        && a.columns() == b.columns();
}

bool operator!=(offer const& a, offer const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, offer const& value) {
    return out << value.kind() << "("
               << "destination=" << value.destination() << ", "
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool offer::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<offer>(other);
}

std::ostream& offer::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
