#include <takatori/statement/create_index.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

create_index::create_index(
        descriptor::schema schema,
        descriptor::relation definition) noexcept :
    schema_ { std::move(schema) },
    definition_ { std::move(definition) }
{}

create_index::create_index(util::clone_tag_t, create_index const& other) noexcept :
        create_index {
                other.schema_,
                other.definition_,
        }
{}

create_index::create_index(util::clone_tag_t, create_index&& other) noexcept :
        create_index {
                std::move(other.schema_),
                std::move(other.definition_),
        }
{}

statement_kind create_index::kind() const noexcept {
    return tag;
}

create_index* create_index::clone() const& {
    return new create_index(util::clone_tag, *this); // NOLINT
}

create_index* create_index::clone() && {
    return new create_index(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::schema& create_index::schema() noexcept {
    return schema_;
}

descriptor::schema const& create_index::schema() const noexcept {
    return schema_;
}

descriptor::relation& create_index::definition() noexcept {
    return definition_;
}

descriptor::relation const& create_index::definition() const noexcept {
    return definition_;
}

bool operator==(create_index const& a, create_index const& b) noexcept {
    return a.schema() == b.schema()
        && a.definition() == b.definition();
}

bool operator!=(create_index const& a, create_index const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, create_index const& value) {
    return out << value.kind() << "("
               << "schema=" << value.schema() << ", "
               << "definition=" << value.definition() << ")";
}

bool create_index::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<create_index>(other);
}

std::ostream& create_index::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
