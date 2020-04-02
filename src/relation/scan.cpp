#include <takatori/relation/scan.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

scan::scan(
        descriptor::relation source,
        std::vector<column, util::object_allocator<column>> columns,
        endpoint lower,
        endpoint upper,
        std::optional<std::size_t> limit,
        util::object_creator creator) noexcept
    : output_(*this, 0, creator)
    , source_(std::move(source))
    , columns_(std::move(columns))
    , lower_(tree::bless_element(*this, std::move(lower)))
    , upper_(tree::bless_element(*this, std::move(upper)))
    , limit_(std::move(limit))
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
            std::move(limit))
{}

scan::scan(scan const& other, util::object_creator creator)
    : scan(
            other.source_,
            { other.columns_, creator.allocator<column>() },
            endpoint { other.lower_, creator },
            endpoint { other.upper_, creator },
            other.limit_,
            creator)
{}

scan::scan(scan&& other, util::object_creator creator)
    : scan(
            std::move(other.source_),
            { std::move(other.columns_), creator.allocator<column>() },
            endpoint { std::move(other.lower_), creator },
            endpoint { std::move(other.upper_), creator },
            std::move(other.limit_),
            creator)
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

scan* scan::clone(util::object_creator creator) const& {
    return creator.create_object<scan>(*this, creator);
}

scan* scan::clone(util::object_creator creator)&& {
    return creator.create_object<scan>(std::move(*this), creator);
}

scan::output_port_type& scan::output() noexcept {
    return output_;
}

scan::output_port_type const& scan::output() const noexcept {
    return output_;
}

descriptor::relation const& scan::source() const noexcept {
    return source_;
}

scan& scan::source(descriptor::relation source) noexcept {
    source_ = std::move(source);
    return *this;
}

std::vector<scan::column, util::object_allocator<scan::column>>& scan::columns() noexcept {
    return columns_;
}

std::vector<scan::column, util::object_allocator<scan::column>> const& scan::columns() const noexcept {
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
    limit_ = std::move(limit);
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
