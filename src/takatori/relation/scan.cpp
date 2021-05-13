#include <takatori/relation/scan.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

scan::scan(
        descriptor::relation source,
        std::vector<column> columns,
        endpoint lower,
        endpoint upper,
        std::optional<std::size_t> limit) noexcept
    : output_(*this, 0)
    , source_(std::move(source))
    , columns_(std::move(columns))
    , lower_(tree::bless_element(*this, std::move(lower)))
    , upper_(tree::bless_element(*this, std::move(upper)))
    , limit_(limit)
{}

scan::scan(
        descriptor::relation source,
        std::initializer_list<column> columns,
        endpoint lower,
        endpoint upper,
        std::optional<std::size_t> limit)
    : scan(
            std::move(source),
            { columns.begin(), columns.end() },
            std::move(lower),
            std::move(upper),
            limit)
{}

scan::scan(util::clone_tag_t, scan const& other)
    : scan(
            other.source_,
            { other.columns_ },
            endpoint { util::clone_tag, other.lower_ },
            endpoint { util::clone_tag, other.upper_ },
            other.limit_)
{}

scan::scan(util::clone_tag_t, scan&& other)
    : scan(
            std::move(other.source_),
            { std::move(other.columns_) },
            endpoint { util::clone_tag, std::move(other.lower_) },
            endpoint { util::clone_tag, std::move(other.upper_) },
            other.limit_)
{}

expression_kind scan::kind() const noexcept {
    return tag;
}

util::sequence_view<scan::input_port_type> scan::input_ports() noexcept {
    return {};
}

util::sequence_view<scan::input_port_type const> scan::input_ports() const noexcept {
    return {};
}

util::sequence_view<scan::output_port_type> scan::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<scan::output_port_type const> scan::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

scan* scan::clone() const& {
    return new scan(util::clone_tag, *this); // NOLINT
}

scan* scan::clone()&& {
    return new scan(util::clone_tag, std::move(*this)); // NOLINT;
}

scan::output_port_type& scan::output() noexcept {
    return output_;
}

scan::output_port_type const& scan::output() const noexcept {
    return output_;
}

descriptor::relation& scan::source() noexcept {
    return source_;
}

descriptor::relation const& scan::source() const noexcept {
    return source_;
}

std::vector<scan::column>& scan::columns() noexcept {
    return columns_;
}

std::vector<scan::column> const& scan::columns() const noexcept {
    return columns_;
}

scan::endpoint& scan::lower() noexcept {
    return lower_;
}

scan::endpoint const& scan::lower() const noexcept {
    return lower_;
}

scan::endpoint& scan::upper() noexcept {
    return upper_;
}

scan::endpoint const& scan::upper() const noexcept {
    return upper_;
}

std::optional<std::size_t> const& scan::limit() const noexcept {
    return limit_;
}

scan& scan::limit(std::optional<std::size_t> limit) noexcept {
    limit_ = limit;
    return *this;
}

bool operator==(scan const& a, scan const& b) noexcept {
    return a.source() == b.source()
        && a.columns() == b.columns()
        && a.lower() == b.lower()
        && a.upper() == b.upper()
        && a.limit() == b.limit();
}

bool operator!=(scan const& a, scan const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, scan const& value) {
    return out << value.kind() << "("
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ", "
               << "lower=" << value.lower() << ", "
               << "upper=" << value.upper() << ", "
               << "limit=" << util::print_support { value.limit() } << ")";
}

bool scan::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<scan>(other);
}

std::ostream& scan::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
