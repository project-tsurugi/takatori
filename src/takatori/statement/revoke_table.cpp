#include <takatori/statement/revoke_table.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::statement {

revoke_table::revoke_table(std::vector<element_type> elements) noexcept :
    elements_ { std::move(elements) }
{}

revoke_table::revoke_table(std::initializer_list<element_type> elements) :
    elements_ { elements }
{}

revoke_table::revoke_table(util::clone_tag_t, revoke_table const& other) noexcept :
    revoke_table { other.elements_ }
{}

revoke_table::revoke_table(util::clone_tag_t, revoke_table&& other) noexcept :
    revoke_table { std::move(other.elements_) }
{}

statement_kind revoke_table::kind() const noexcept {
    return tag;
}

revoke_table* revoke_table::clone() const& {
    return new revoke_table(util::clone_tag, *this); // NOLINT
}

revoke_table* revoke_table::clone() && {
    return new revoke_table(util::clone_tag, std::move(*this)); // NOLINT;
}

std::vector<revoke_table::element_type>& revoke_table::elements() noexcept {
    return elements_;
}

std::vector<revoke_table::element_type> const& revoke_table::elements() const noexcept {
    return elements_;
}

bool operator==(revoke_table const& a, revoke_table const& b) noexcept {
    return std::addressof(a) == std::addressof(b);
}

bool operator!=(revoke_table const& a, revoke_table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, revoke_table const& value) {
    using util::print_support;
    return out << value.kind() << "("
               << "elements=" << print_support { value.elements() } << ")";
}

bool revoke_table::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<revoke_table>(other);
}

std::ostream& revoke_table::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
