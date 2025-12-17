#include <takatori/type/details/table_column.h>

#include <ostream>

#include <takatori/util/clonable.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/print_support.h>

namespace takatori::type::details {

table_column::table_column(name_type name, std::shared_ptr<data const> type) noexcept:
    name_ { std::move(name) },
    type_ { std::move(type) }
{}

table_column::table_column(std::string_view name, data&& type):
    table_column {
            std::string { name },
            util::clone_shared(std::move(type)),
    }
{}

std::string_view table_column::name() const noexcept {
    return name_;
}

data const& table_column::type() const noexcept {
    return *type_;
}

std::shared_ptr<data const> table_column::shared_type() const noexcept {
    return type_;
}

bool operator==(table_column const& a, table_column const& b) noexcept {
    if (std::addressof(a) == std::addressof(b)) {
        return true;
    }
    util::optional_ptr a_type { a.type_ };
    util::optional_ptr b_type { b.type_ };
    return a.name_ == b.name_ && a_type == b_type;
}

bool operator!=(table_column const& a, table_column const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, table_column const& value) {
    out << "column("
        << "name=" << value.name_ << ", "
        << "type=" << util::print_support { value.type_ } << ")";
    return out;
}

} // namespace takatori::type::details
