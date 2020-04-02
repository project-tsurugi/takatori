#include <takatori/relation/find.h>

#include <takatori/tree/tree_fragment_vector.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

find::find(
        descriptor::relation source,
        std::vector<column, util::object_allocator<column>> columns,
        std::vector<key, util::object_allocator<key>> keys,
        util::object_creator creator) noexcept
    : output_(*this, 0, creator)
    , source_(std::move(source))
    , columns_(std::move(columns))
    , keys_(*this, std::move(keys))
{}

find::find(
        descriptor::relation source,
        std::initializer_list<column> columns,
        std::initializer_list<util::rvalue_reference_wrapper<key>> keys)
    : find(
            std::move(source),
            { columns.begin(), columns.end() },
            { keys.begin(), keys.end() })
{}


find::find(find const& other, util::object_creator creator)
    : find(
            other.source_,
            { other.columns_, creator.allocator<column>() },
            tree::forward(creator, other.keys_),
            creator)
{}

find::find(find&& other, util::object_creator creator)
    : find(
            std::move(other.source_),
            { std::move(other.columns_), creator.allocator<column>() },
            tree::forward(creator, std::move(other.keys_)),
            creator)
{}

expression_kind find::kind() const noexcept {
    return tag;
}

find* find::clone(util::object_creator creator) const& {
    return creator.create_object<find>(*this, creator);
}

find* find::clone(util::object_creator creator)&& {
    return creator.create_object<find>(std::move(*this), creator);
}


util::sequence_view<find::input_port_type> find::input_ports() noexcept {
    return {};
}

util::sequence_view<find::input_port_type const> find::input_ports() const noexcept {
    return {};
}

util::sequence_view<find::output_port_type> find::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<find::output_port_type const> find::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

find::output_port_type& find::output() noexcept {
    return output_;
}

find::output_port_type const& find::output() const noexcept {
    return output_;
}

descriptor::relation const& find::source() const noexcept {
    return source_;
}

find& find::source(descriptor::relation source) noexcept {
    source_ = std::move(source);
    return *this;
}

std::vector<find::column, util::object_allocator<find::column>>& find::columns() noexcept {
    return columns_;
}

std::vector<find::column, util::object_allocator<find::column>> const& find::columns() const noexcept {
    return columns_;
}

tree::tree_fragment_vector<find::key>& find::keys() noexcept {
    return keys_;
}

tree::tree_fragment_vector<find::key> const& find::keys() const noexcept {
    return keys_;
}

bool operator==(find const& a, find const& b) noexcept {
    return a.source() == b.source()
        && a.columns() == b.columns()
        && a.keys() == b.keys();
}

bool operator!=(find const& a, find const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, find const& value) {
    return out << value.kind() << "("
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ", "
               << "keys=" << value.keys() << ")";
}

bool find::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<find>(other);
}

std::ostream& find::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
