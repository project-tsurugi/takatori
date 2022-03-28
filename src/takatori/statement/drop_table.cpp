#include <takatori/statement/drop_table.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

drop_table::drop_table(
        descriptor::schema schema,
        descriptor::storage target) noexcept :
    schema_ { std::move(schema) },
    target_ { std::move(target) }
{}

drop_table::drop_table(util::clone_tag_t, drop_table const& other) noexcept :
        drop_table {
                other.schema_,
                other.target_,
        }
{}

drop_table::drop_table(util::clone_tag_t, drop_table&& other) noexcept :
        drop_table {
                std::move(other.schema_),
                std::move(other.target_),
        }
{}

statement_kind drop_table::kind() const noexcept {
    return tag;
}

drop_table* drop_table::clone() const& {
    return new drop_table(util::clone_tag, *this); // NOLINT
}

drop_table* drop_table::clone() && {
    return new drop_table(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::schema& drop_table::schema() noexcept {
    return schema_;
}

descriptor::schema const& drop_table::schema() const noexcept {
    return schema_;
}

descriptor::storage& drop_table::target() noexcept {
    return target_;
}

descriptor::storage const& drop_table::target() const noexcept {
    return target_;
}

bool operator==(drop_table const& a, drop_table const& b) noexcept {
    return a.schema() == b.schema()
        && a.target() == b.target();
}

bool operator!=(drop_table const& a, drop_table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, drop_table const& value) {
    return out << value.kind() << "("
               << "schema=" << value.schema() << ", "
               << "target=" << value.target() << ")";
}

bool drop_table::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<drop_table>(other);
}

std::ostream& drop_table::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
