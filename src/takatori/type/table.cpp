#include <takatori/type/table.h>

#include <ostream>

#include <takatori/util/downcast.h>
#include <takatori/util/hash.h>

namespace takatori::type {

table::table(std::vector<column_type> columns) noexcept:
    columns_ { std::move(columns) }
{}

table::table(std::initializer_list<column_type> columns):
    table {
            std::vector<column_type> { columns }
    }
{}

type_kind table::kind() const noexcept {
    return tag;
}

table* table::clone() const & {
    return new table { columns_ }; // NOLINT
}

table* table::clone() && {
    return new table { std::move(columns_) }; // NOLINT
}

std::vector<table::column_type> const& table::columns() const noexcept {
    return columns_;
}

bool table::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<table>(other);
}

std::size_t table::hash() const noexcept {
    std::size_t result = 0;
    for (auto&& column : columns_) {
        result = result * util::hash_prime + util::hash(column.name(), column.type());
    }
    return result;
}

std::ostream& table::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(table const& a, table const& b) noexcept {
    return a.columns_ == b.columns_;
}

bool operator!=(table const& a, table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, table const& value) {
    out << "table(";
    bool first = true;
    for (auto const& column : value.columns()) {
        if (!first) {
            out << ", ";
        }
        first = false;
        out << column;
    }
    out << ")";
    return out;
}

} // namespace takatori::type
