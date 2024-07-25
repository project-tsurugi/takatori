#include <takatori/relation/write.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

write::write(
        operator_kind_type operator_kind,
        descriptor::relation destination,
        std::vector<key> keys,
        std::vector<column> columns,
        std::vector<default_column> default_columns) noexcept :
    input_ { *this, 0 },
    operator_kind_ { operator_kind },
    destination_ { std::move(destination) },
    keys_ { std::move(keys) },
    columns_ { std::move(columns) },
    default_columns_ { std::move(default_columns) }
{}

write::write(
        operator_kind_type operator_kind,
        descriptor::relation destination,
        std::initializer_list<key> keys,
        std::initializer_list<column> columns,
        std::initializer_list<default_column> default_columns) :
    write {
            operator_kind,
            std::move(destination),
            { keys.begin(), keys.end() },
            { columns.begin(), columns.end() },
            { default_columns.begin(), default_columns.end() },
    }
{}


write::write(util::clone_tag_t, write const& other) :
    write {
            other.operator_kind_,
            other.destination_,
            other.keys_,
            other.columns_,
            other.default_columns_
    }
{}

write::write(util::clone_tag_t, write&& other) :
    write {
            other.operator_kind_,
            std::move(other.destination_),
            std::move(other.keys_),
            std::move(other.columns_),
            std::move(other.default_columns_)
    }
{}

expression_kind write::kind() const noexcept {
    return tag;
}

write* write::clone() const& {
    return new write(util::clone_tag, *this); // NOLINT
}

write* write::clone()&& {
    return new write(util::clone_tag, std::move(*this)); // NOLINT;
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

descriptor::relation& write::destination() noexcept {
    return destination_;
}

descriptor::relation const& write::destination() const noexcept {
    return destination_;
}

std::vector<write::key>& write::keys() noexcept {
    return keys_;
}

std::vector<write::key> const& write::keys() const noexcept {
    return keys_;
}

std::vector<write::column>& write::columns() noexcept {
    return columns_;
}

std::vector<write::column> const& write::columns() const noexcept {
    return columns_;
}

std::vector<write::default_column>& write::default_columns() noexcept {
    return default_columns_;
}

std::vector<write::default_column> const& write::default_columns() const noexcept {
    return default_columns_;
}

bool operator==(write const& a, write const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.destination() == b.destination()
        && a.keys() == b.keys()
        && a.columns() == b.columns()
        && a.default_columns() == b.default_columns();
}

bool operator!=(write const& a, write const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, write const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "destination=" << value.destination() << ", "
               << "keys=" << util::print_support { value.keys() } << ", "
               << "columns=" << util::print_support { value.columns() } << ", "
               << "default_columns=" << util::print_support { value.default_columns() } << ")";
}

bool write::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<write>(other);
}

std::ostream& write::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
