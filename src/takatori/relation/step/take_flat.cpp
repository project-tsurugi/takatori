#include <takatori/relation/step/take_flat.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::step {

take_flat::take_flat(
        descriptor::relation source,
        std::vector<column> columns) noexcept
    : output_(*this, 0)
    , source_(std::move(source))
    , columns_(std::move(columns))
{}

take_flat::take_flat(
        descriptor::relation source,
        std::initializer_list<column> columns)
    : take_flat(
            std::move(source),
            { columns.begin(), columns.end() })
{}


take_flat::take_flat(util::clone_tag_t, take_flat const& other)
    : take_flat(
            other.source_,
            { other.columns_ })
{}

take_flat::take_flat(util::clone_tag_t, take_flat&& other)
    : take_flat(
            std::move(other.source_),
            { std::move(other.columns_) })
{}

expression_kind take_flat::kind() const noexcept {
    return tag;
}

take_flat* take_flat::clone() const& {
    return new take_flat(util::clone_tag, *this); // NOLINT
}

take_flat* take_flat::clone()&& {
    return new take_flat(util::clone_tag, std::move(*this)); // NOLINT;
}


util::sequence_view<take_flat::input_port_type> take_flat::input_ports() noexcept {
    return {};
}

util::sequence_view<take_flat::input_port_type const> take_flat::input_ports() const noexcept {
    return {};
}

util::sequence_view<take_flat::output_port_type> take_flat::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<take_flat::output_port_type const> take_flat::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

take_flat::output_port_type& take_flat::output() noexcept {
    return output_;
}

take_flat::output_port_type const& take_flat::output() const noexcept {
    return output_;
}

descriptor::relation& take_flat::source() noexcept {
    return source_;
}

descriptor::relation const& take_flat::source() const noexcept {
    return source_;
}

std::vector<take_flat::column>& take_flat::columns() noexcept {
    return columns_;
}

std::vector<take_flat::column> const& take_flat::columns() const noexcept {
    return columns_;
}

bool operator==(take_flat const& a, take_flat const& b) noexcept {
    return a.source() == b.source()
        && a.columns() == b.columns();
}

bool operator!=(take_flat const& a, take_flat const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, take_flat const& value) {
    return out << value.kind() << "("
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool take_flat::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<take_flat>(other);
}

std::ostream& take_flat::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
