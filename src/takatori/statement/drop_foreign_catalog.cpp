#include <takatori/statement/drop_foreign_catalog.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

drop_foreign_catalog::drop_foreign_catalog(
        name_type catalog_name,
        option_set_type options) noexcept :
    catalog_name_ { std::move(catalog_name) },
    options_ { options }
{}

drop_foreign_catalog::drop_foreign_catalog(util::clone_tag_t, drop_foreign_catalog const& other) :
    drop_foreign_catalog {
            other.catalog_name_,
            other.options_,
    }
{}

drop_foreign_catalog::drop_foreign_catalog(util::clone_tag_t, drop_foreign_catalog&& other) noexcept :
    drop_foreign_catalog {
            std::move(other.catalog_name_),
            other.options_,
    }
{}

statement_kind drop_foreign_catalog::kind() const noexcept {
    return tag;
}

drop_foreign_catalog* drop_foreign_catalog::clone() const & {
    return new drop_foreign_catalog(util::clone_tag, *this); // NOLINT
}

drop_foreign_catalog* drop_foreign_catalog::clone() && {
    return new drop_foreign_catalog(util::clone_tag, std::move(*this)); // NOLINT;
}

drop_foreign_catalog::name_type& drop_foreign_catalog::catalog_name() noexcept {
    return catalog_name_;
}

drop_foreign_catalog::name_type const& drop_foreign_catalog::catalog_name() const noexcept {
    return catalog_name_;
}

drop_foreign_catalog::option_set_type& drop_foreign_catalog::options() noexcept {
    return options_;
}

drop_foreign_catalog::option_set_type const& drop_foreign_catalog::options() const noexcept {
    return options_;
}

bool drop_foreign_catalog::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<drop_foreign_catalog>(other);
}

std::ostream& drop_foreign_catalog::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(drop_foreign_catalog const& a, drop_foreign_catalog const& b) noexcept {
    return a.catalog_name() == b.catalog_name()
        && a.options() == b.options();
}

bool operator!=(drop_foreign_catalog const& a, drop_foreign_catalog const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, drop_foreign_catalog const& value) {
    return out << value.kind() << "("
               << "catalog_name=" << value.catalog_name() << ", "
               << "options=" << value.options() << ")";
}

} // namespace takatori::statement
