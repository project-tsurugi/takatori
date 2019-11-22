#include "takatori/relation/write.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"
#include "takatori/util/vector_print_support.h"

namespace takatori::relation {

write::write(
        operator_kind_type operator_kind,
        descriptor::relation destination,
        std::vector<key, util::object_allocator<key>> keys,
        std::vector<column, util::object_allocator<column>> columns,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , operator_kind_(operator_kind)
    , destination_(std::move(destination))
    , keys_(std::move(keys))
    , columns_(std::move(columns))
{}

write::write(
        operator_kind_type operator_kind,
        descriptor::relation destination,
        std::initializer_list<key> keys,
        std::initializer_list<column> columns)
    : write(
            operator_kind,
            std::move(destination),
            { keys.begin(), keys.end() },
            { columns.begin(), columns.end() })
{}


write::write(write const& other, util::object_creator creator)
    : write(
            other.operator_kind_,
            other.destination_,
            { other.keys_, creator.allocator<key>() },
            { other.columns_, creator.allocator<column>() },
            creator)
{}

write::write(write&& other, util::object_creator creator)
    : write(
            other.operator_kind_,
            std::move(other.destination_),
            { std::move(other.keys_), creator.allocator<key>() },
            { std::move(other.columns_), creator.allocator<column>() },
            creator)
{}

expression_kind write::kind() const noexcept {
    return tag;
}

write* write::clone(util::object_creator creator) const& {
    return creator.create_object<write>(*this, creator);
}

write* write::clone(util::object_creator creator)&& {
    return creator.create_object<write>(std::move(*this), creator);
}


util::sequence_view<write::input_port_type> write::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<write::input_port_type const> write::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<write::output_port_type> write::output_ports() noexcept {
    return {};
}

util::sequence_view<write::output_port_type const> write::output_ports() const noexcept {
    return {};
}

write::input_port_type& write::input() noexcept {
    return input_;
}

write::input_port_type const& write::input() const noexcept {
    return input_;
}

write::operator_kind_type write::operator_kind() const noexcept {
    return operator_kind_;
}

write& write::operator_kind(write::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

descriptor::relation const& write::destination() const noexcept {
    return destination_;
}

write& write::destination(descriptor::relation destination) noexcept {
    destination_ = std::move(destination);
    return *this;
}

std::vector<write::key, util::object_allocator<write::key>>& write::keys() noexcept {
    return keys_;
}

std::vector<write::key, util::object_allocator<write::key>> const& write::keys() const noexcept {
    return keys_;
}

std::vector<write::column, util::object_allocator<write::column>>& write::columns() noexcept {
    return columns_;
}

std::vector<write::column, util::object_allocator<write::column>> const& write::columns() const noexcept {
    return columns_;
}

bool operator==(write const& a, write const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.destination() == b.destination()
        && a.keys() == b.keys()
        && a.columns() == b.columns();
}

bool operator!=(write const& a, write const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, write const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "destination=" << value.destination() << ", "
               << "keys=" << util::print_support { value.keys() } << ", "
               << "columns=" << util::print_support { value.columns() } << ")";
}

bool write::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<write>(other);
}

std::ostream& write::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
