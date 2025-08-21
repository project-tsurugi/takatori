#include <takatori/statement/details/table_privilege_element.h>

#include <takatori/util/vector_print_support.h>

namespace takatori::statement::details {

table_privilege_element::table_privilege_element(
        descriptor::storage table,
        std::vector<table_privilege_action> default_privileges,
        std::vector<table_authorization_entry> authorization_entries) noexcept :
    table_ { std::move(table) },
    default_privileges_ { std::move(default_privileges) },
    authorization_entries_ { std::move(authorization_entries) }
{}

table_privilege_element::table_privilege_element(util::clone_tag_t, table_privilege_element const& other) :
    table_privilege_element {
            other.table_,
            other.default_privileges_,
            other.authorization_entries_,
    }
{}

table_privilege_element::table_privilege_element(util::clone_tag_t, table_privilege_element&& other) :
    table_privilege_element {
            std::move(other.table_),
            std::move(other.default_privileges_),
            std::move(other.authorization_entries_),
    }
{}

descriptor::storage& table_privilege_element::table() noexcept {
    return table_;
}

descriptor::storage const& table_privilege_element::table() const noexcept {
    return table_;
}

std::vector<table_privilege_action>& table_privilege_element::default_privileges() noexcept {
    return default_privileges_;
}

std::vector<table_privilege_action> const& table_privilege_element::default_privileges() const noexcept {
    return default_privileges_;
}

std::vector<table_authorization_entry>& table_privilege_element::authorization_entries() noexcept {
    return authorization_entries_;
}

std::vector<table_authorization_entry> const& table_privilege_element::authorization_entries() const noexcept {
    return authorization_entries_;
}

bool operator==(table_privilege_element const& a, table_privilege_element const& b) noexcept {
    return a.table() == b.table()
        && a.default_privileges() == b.default_privileges()
        && a.authorization_entries() == b.authorization_entries();
}

bool operator!=(table_privilege_element const& a, table_privilege_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, table_privilege_element const& value) {
    using util::print_support;
    return out << "table_privilege_element("
               << "table=" << value.table() << ", "
               << "default_privileges=" << print_support { value.default_privileges() } << ", "
               << "authorization_entries=" << print_support { value.authorization_entries() } << ")";
}

} // namespace takatori::statement::details
