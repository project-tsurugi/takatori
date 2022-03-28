#include <takatori/statement/create_table.h>

#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::statement {

create_table::create_table(
        descriptor::schema schema,
        descriptor::storage definition,
        descriptor::relation primary_key,
        std::vector<descriptor::relation> unique_keys) noexcept :
    schema_ { std::move(schema) },
    definition_ { std::move(definition) },
    primary_key_ { std::move(primary_key) },
    unique_keys_ { std::move(unique_keys) }
{}

create_table::create_table(util::clone_tag_t, create_table const& other) noexcept :
        create_table {
                other.schema_,
                other.definition_,
                other.primary_key_,
                other.unique_keys_,
        }
{}

create_table::create_table(util::clone_tag_t, create_table&& other) noexcept :
        create_table {
                std::move(other.schema_),
                std::move(other.definition_),
                std::move(other.primary_key_),
                std::move(other.unique_keys_),
        }
{}

statement_kind create_table::kind() const noexcept {
    return tag;
}

create_table* create_table::clone() const& {
    return new create_table(util::clone_tag, *this); // NOLINT
}

create_table* create_table::clone() && {
    return new create_table(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::schema& create_table::schema() noexcept {
    return schema_;
}

descriptor::schema const& create_table::schema() const noexcept {
    return schema_;
}

descriptor::storage& create_table::definition() noexcept {
    return definition_;
}

descriptor::storage const& create_table::definition() const noexcept {
    return definition_;
}

descriptor::relation& create_table::primary_key() noexcept {
    return primary_key_;
}

descriptor::relation const& create_table::primary_key() const noexcept {
    return primary_key_;
}

std::vector<descriptor::relation>& create_table::unique_keys() noexcept {
    return unique_keys_;
}

std::vector<descriptor::relation> const& create_table::unique_keys() const noexcept {
    return unique_keys_;
}

bool operator==(create_table const& a, create_table const& b) noexcept {
    return a.schema() == b.schema()
        && a.definition() == b.definition()
        && a.primary_key() == b.primary_key()
        && a.unique_keys() == b.unique_keys();
}

bool operator!=(create_table const& a, create_table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, create_table const& value) {
    using util::print_support;
    return out << value.kind() << "("
               << "schema=" << value.schema() << ", "
               << "definition=" << value.definition() << ", "
               << "primary_key=" << value.primary_key() << ", "
               << "unique_keys=" << print_support { value.unique_keys() } << ")";
}

bool create_table::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<create_table>(other);
}

std::ostream& create_table::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
