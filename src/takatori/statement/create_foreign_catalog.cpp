#include <takatori/statement/create_foreign_catalog.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

create_foreign_catalog::create_foreign_catalog(
        name_type catalog_name,
        name_type server_name,
        option_set_type options) noexcept :
    catalog_name_ { std::move(catalog_name) },
    server_name_ { std::move(server_name) },
    options_ { options }
{}

create_foreign_catalog::create_foreign_catalog(util::clone_tag_t, create_foreign_catalog const& other) :
    create_foreign_catalog {
            other.catalog_name_,
            other.server_name_,
            other.options_,
    }
{}

create_foreign_catalog::create_foreign_catalog(util::clone_tag_t, create_foreign_catalog&& other) noexcept :
    create_foreign_catalog {
            std::move(other.catalog_name_),
            std::move(other.server_name_),
            other.options_,
    }
{}

statement_kind create_foreign_catalog::kind() const noexcept {
    return tag;
}

create_foreign_catalog* create_foreign_catalog::clone() const & {
    return new create_foreign_catalog(util::clone_tag, *this); // NOLINT
}

create_foreign_catalog* create_foreign_catalog::clone() && {
    return new create_foreign_catalog(util::clone_tag, std::move(*this)); // NOLINT;
}

create_foreign_catalog::name_type& create_foreign_catalog::catalog_name() noexcept {
    return catalog_name_;
}

create_foreign_catalog::name_type const& create_foreign_catalog::catalog_name() const noexcept {
    return catalog_name_;
}

create_foreign_catalog::name_type& create_foreign_catalog::server_name() noexcept {
    return server_name_;
}

create_foreign_catalog::name_type const& create_foreign_catalog::server_name() const noexcept {
    return server_name_;
}

create_foreign_catalog::option_set_type& create_foreign_catalog::options() noexcept {
    return options_;
}

create_foreign_catalog::option_set_type const& create_foreign_catalog::options() const noexcept {
    return options_;
}

bool create_foreign_catalog::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<create_foreign_catalog>(other);
}

std::ostream& create_foreign_catalog::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(create_foreign_catalog const& a, create_foreign_catalog const& b) noexcept {
    return a.catalog_name() == b.catalog_name()
        && a.server_name() == b.server_name()
        && a.options() == b.options();
}

bool operator!=(create_foreign_catalog const& a, create_foreign_catalog const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, create_foreign_catalog const& value) {
    return out << value.kind() << "("
               << "catalog_name=" << value.catalog_name() << ", "
               << "server_name=" << value.server_name() << ", "
               << "options=" << value.options() << ")";
}

} // namespace takatori::statement
