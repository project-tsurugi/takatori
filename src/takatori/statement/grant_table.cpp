#include <takatori/statement/grant_table.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::statement {

grant_table::grant_table(std::vector<element_type> elements) noexcept :
    elements_ { std::move(elements) }
{}

grant_table::grant_table(std::initializer_list<element_type> elements) :
    elements_ { elements }
{}

grant_table::grant_table(util::clone_tag_t, grant_table const& other) noexcept :
    grant_table { other.elements_ }
{}

grant_table::grant_table(util::clone_tag_t, grant_table&& other) noexcept :
    grant_table { std::move(other.elements_) }
{}

statement_kind grant_table::kind() const noexcept {
    return tag;
}

grant_table* grant_table::clone() const& {
    return new grant_table(util::clone_tag, *this); // NOLINT
}

grant_table* grant_table::clone() && {
    return new grant_table(util::clone_tag, std::move(*this)); // NOLINT;
}

std::vector<grant_table::element_type>& grant_table::elements() noexcept {
    return elements_;
}

std::vector<grant_table::element_type> const& grant_table::elements() const noexcept {
    return elements_;
}

bool operator==(grant_table const& a, grant_table const& b) noexcept {
    return std::addressof(a) == std::addressof(b);
}

bool operator!=(grant_table const& a, grant_table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, grant_table const& value) {
    using util::print_support;
    return out << value.kind() << "("
               << "elements=" << print_support { value.elements() } << ")";
}

bool grant_table::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<grant_table>(other);
}

std::ostream& grant_table::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
