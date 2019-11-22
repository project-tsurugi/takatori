#include "takatori/relation/emit.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"
#include "takatori/util/vector_print_support.h"

namespace takatori::relation {

emit::emit(
        std::vector<column, util::object_allocator<column>> columns,
        std::vector<key, util::object_allocator<key>> keys,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , columns_(std::move(columns))
    , keys_(std::move(keys))
{}

emit::emit(
        std::initializer_list<column> columns,
        std::initializer_list<key> keys)
    : emit(
            { columns.begin(), columns.end() },
            { keys.begin(), keys.end() })
{}


emit::emit(emit const& other, util::object_creator creator)
    : emit(
            { other.columns_, creator.allocator<column>() },
            { other.keys_, creator.allocator<column>() },
            creator)
{}

emit::emit(emit&& other, util::object_creator creator)
    : emit(
            { std::move(other.columns_), creator.allocator<column>() },
            { std::move(other.keys_), creator.allocator<column>() },
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

std::vector<emit::key, util::object_allocator<emit::key>>& emit::keys() noexcept {
    return keys_;
}

std::vector<emit::key, util::object_allocator<emit::key>> const& emit::keys() const noexcept {
    return keys_;
}

bool operator==(emit const& a, emit const& b) noexcept {
    return a.columns() == b.columns()
        && a.keys() == b.keys();
}

bool operator!=(emit const& a, emit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, emit const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ", "
               << "keys=" << util::print_support { value.keys() } << ")";
}

bool emit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<emit>(other);
}

std::ostream& emit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
