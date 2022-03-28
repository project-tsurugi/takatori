#include <takatori/statement/drop_index.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

drop_index::drop_index(
        descriptor::schema schema,
        descriptor::relation target) noexcept :
        schema_ { std::move(schema) },
        target_ { std::move(target) }
{}

drop_index::drop_index(util::clone_tag_t, drop_index const& other) noexcept :
        drop_index {
                other.schema_,
                other.target_,
        }
{}

drop_index::drop_index(util::clone_tag_t, drop_index&& other) noexcept :
        drop_index {
                std::move(other.schema_),
                std::move(other.target_),
        }
{}

statement_kind drop_index::kind() const noexcept {
    return tag;
}

drop_index* drop_index::clone() const& {
    return new drop_index(util::clone_tag, *this); // NOLINT
}

drop_index* drop_index::clone() && {
    return new drop_index(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::schema& drop_index::schema() noexcept {
    return schema_;
}

descriptor::schema const& drop_index::schema() const noexcept {
    return schema_;
}

descriptor::relation& drop_index::target() noexcept {
    return target_;
}

descriptor::relation const& drop_index::target() const noexcept {
    return target_;
}

bool operator==(drop_index const& a, drop_index const& b) noexcept {
    return a.schema() == b.schema()
            && a.target() == b.target();
}

bool operator!=(drop_index const& a, drop_index const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, drop_index const& value) {
    return out << value.kind() << "("
               << "schema=" << value.schema() << ", "
               << "target=" << value.target() << ")";
}

bool drop_index::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<drop_index>(other);
}

std::ostream& drop_index::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
