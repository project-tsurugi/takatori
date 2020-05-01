#include <takatori/statement/statement.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

statement::parent_type* statement::parent_element() noexcept {
    return parent_;
}

statement::parent_type const* statement::parent_element() const noexcept {
    return parent_;
}

void statement::parent_element(statement::parent_type* parent) noexcept {
    parent_ = parent;
}

util::optional_ptr<statement> statement::parent_statement() noexcept {
    return util::optional_ptr { util::downcast<statement>(parent_element()) };
}

util::optional_ptr<statement const> statement::parent_statement() const noexcept {
    return util::optional_ptr { util::downcast<statement>(parent_element()) };
}

document::region& statement::region() noexcept {
    return region_;
}

document::region const& statement::region() const noexcept {
    return region_;
}

bool operator==(statement const& a, statement const& b) noexcept {
    return a.equals(b);
}

bool operator!=(statement const& a, statement const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, statement const& value) {
    return value.print_to(out);
}

} // namespace takatori::statement
