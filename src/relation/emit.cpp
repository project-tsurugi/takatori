#include <takatori/relation/emit.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

emit::emit(
        std::vector<column, util::object_allocator<column>> columns,
        std::vector<sort_key, util::object_allocator<sort_key>> sort_keys,
        std::optional<size_type> limit,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , columns_(std::move(columns))
    , sort_keys_(std::move(sort_keys))
    , limit_(std::move(limit))
{}

emit::emit(
        std::initializer_list<column> columns,
        std::initializer_list<sort_key> sort_keys,
        std::optional<size_type> limit)
    : emit(
            { columns.begin(), columns.end() },
            { sort_keys.begin(), sort_keys.end() },
            std::move(limit))
{}


emit::emit(emit const& other, util::object_creator creator)
    : emit(
            { other.columns_, creator.allocator<column>() },
            { other.sort_keys_, creator.allocator<sort_key>() },
            other.limit_,
            creator)
{}

emit::emit(emit&& other, util::object_creator creator)
    : emit(
            { std::move(other.columns_), creator.allocator<column>() },
            { std::move(other.sort_keys_), creator.allocator<sort_key>() },
            std::move(other.limit_),
            creator)
{}

expression_kind emit::kind() const noexcept {
    return tag;
}

emit* emit::clone(util::object_creator creator) const& {
    return creator.create_object<emit>(*this, creator);
}

emit* emit::clone(util::object_creator creator)&& {
    return creator.create_object<emit>(std::move(*this), creator);
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

std::vector<emit::column, util::object_allocator<emit::column>>& emit::columns() noexcept {
    return columns_;
}

std::vector<emit::column, util::object_allocator<emit::column>> const& emit::columns() const noexcept {
    return columns_;
}

std::vector<emit::sort_key, util::object_allocator<emit::sort_key>>& emit::sort_keys() noexcept {
    return sort_keys_;
}

std::vector<emit::sort_key, util::object_allocator<emit::sort_key>> const& emit::sort_keys() const noexcept {
    return sort_keys_;
}

std::optional<emit::size_type> const& emit::limit() const noexcept {
    return limit_;
}

emit& emit::limit(std::optional<emit::size_type> limit) noexcept {
    limit_ = std::move(limit);
    return *this;
}

bool operator==(emit const& a, emit const& b) noexcept {
    return a.columns() == b.columns()
        && a.sort_keys() == b.sort_keys()
        && a.limit() == b.limit();
}

bool operator!=(emit const& a, emit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, emit const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ", "
               << "sort_keys=" << util::print_support { value.sort_keys() } << ", "
               << "limit=" << util::print_support { value.limit() } << ")";
}

bool emit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<emit>(other);
}

std::ostream& emit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
