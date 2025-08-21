#include <takatori/statement/details/table_privilege_action.h>

namespace takatori::statement::details {

table_privilege_action::table_privilege_action() noexcept = default;

table_privilege_action::table_privilege_action(action_kind_type kind) noexcept:
    action_kind_ { kind }
{}

table_privilege_action::table_privilege_action(util::clone_tag_t, table_privilege_action const& other) :
    table_privilege_action {
            other.action_kind_,
    }
{}

table_privilege_action::table_privilege_action(util::clone_tag_t, table_privilege_action&& other) :
    table_privilege_action {
            other.action_kind_,
    }
{}

table_privilege_action::action_kind_type& table_privilege_action::action_kind() noexcept {
    return action_kind_;
}

table_privilege_action::action_kind_type const& table_privilege_action::action_kind() const noexcept {
    return action_kind_;
}

bool operator==(table_privilege_action const& a, table_privilege_action const& b) noexcept {
    return a.action_kind() == b.action_kind();
}

bool operator!=(table_privilege_action const& a, table_privilege_action const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, table_privilege_action const& value) {
    return out << "table_privilege_action("
               << "action_kind=" << value.action_kind() << ")";
}

} // namespace takatori::statement::details
