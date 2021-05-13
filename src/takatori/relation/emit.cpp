#include <takatori/relation/emit.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

emit::emit(std::vector<column> columns) noexcept
    : input_(*this, 0)
    , columns_(std::move(columns))
{}

emit::emit(std::initializer_list<column> columns)
    : emit({ columns.begin(), columns.end() })
{}


emit::emit(util::clone_tag_t, emit const& other)
    : emit(
            decltype(columns_) { other.columns_ })
{}

emit::emit(util::clone_tag_t, emit&& other)
    : emit(
            decltype(columns_) { std::move(other.columns_) })
{}

expression_kind emit::kind() const noexcept {
    return tag;
}

emit* emit::clone() const& {
    return new emit(util::clone_tag, *this); // NOLINT
}

emit* emit::clone()&& {
    return new emit(util::clone_tag, std::move(*this)); // NOLINT;
}


util::sequence_view<emit::input_port_type> emit::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<emit::input_port_type const> emit::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<emit::output_port_type> emit::output_ports() noexcept {
    return {};
}

util::sequence_view<emit::output_port_type const> emit::output_ports() const noexcept {
    return {};
}

emit::input_port_type& emit::input() noexcept {
    return input_;
}

emit::input_port_type const& emit::input() const noexcept {
    return input_;
}

std::vector<emit::column>& emit::columns() noexcept {
    return columns_;
}

std::vector<emit::column> const& emit::columns() const noexcept {
    return columns_;
}

bool operator==(emit const& a, emit const& b) noexcept {
    return a.columns() == b.columns();
}

bool operator!=(emit const& a, emit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, emit const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool emit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<emit>(other);
}

std::ostream& emit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
