#include <takatori/statement/details/table_authorization_entry.h>

#include <takatori/util/vector_print_support.h>

namespace takatori::statement::details {

table_authorization_entry::table_authorization_entry(
        user_kind_type user_kind,
        identifier_type authorization_identifier,
        std::vector<table_privilege_action> privileges) noexcept :
    user_kind_ { user_kind },
    authorization_identifier_ { std::move(authorization_identifier) },
    privileges_ { std::move(privileges) }
{}

table_authorization_entry::table_authorization_entry(util::clone_tag_t, table_authorization_entry const& other) :
    table_authorization_entry {
            other.user_kind_,
            other.authorization_identifier_,
            other.privileges_,
    }
{}

table_authorization_entry::table_authorization_entry(util::clone_tag_t, table_authorization_entry&& other) :
    table_authorization_entry {
            other.user_kind_,
            std::move(other.authorization_identifier_),
            std::move(other.privileges_),
    }
{}

table_authorization_entry::user_kind_type& table_authorization_entry::user_kind() noexcept {
    return user_kind_;
}

table_authorization_entry::user_kind_type const& table_authorization_entry::user_kind() const noexcept {
    return user_kind_;
}

table_authorization_entry::identifier_type& table_authorization_entry::authorization_identifier() noexcept {
    return authorization_identifier_;
}

table_authorization_entry::identifier_type const& table_authorization_entry::authorization_identifier() const noexcept {
    return authorization_identifier_;
}

std::vector<table_privilege_action>& table_authorization_entry::privileges() noexcept {
    return privileges_;
}

std::vector<table_privilege_action> const& table_authorization_entry::privileges() const noexcept {
    return privileges_;
}

bool operator==(table_authorization_entry const& a, table_authorization_entry const& b) noexcept {
    if (a.user_kind() != b.user_kind()) {
        return false;
    }
    if (a.user_kind() == authorization_user_kind::specified) {
        if (a.authorization_identifier() != b.authorization_identifier()) {
            return false;
        }
    }
    return a.privileges() == b.privileges();
}

bool operator!=(table_authorization_entry const& a, table_authorization_entry const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, table_authorization_entry const& value) {
    using util::print_support;
    return out << "table_authorization_entry("
               << "user_kind=" << value.user_kind() << ", "
               << "authorization_identifier=" << value.authorization_identifier() << ", "
               << "privileges=" << print_support { value.privileges() } << ")";
}

} // namespace takatori::statement::details
