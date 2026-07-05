#include <takatori/statement/rename_table.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

rename_table::rename_table(
        descriptor::schema schema,
        descriptor::storage target,
        name_type replacement) noexcept:
    schema_ { std::move(schema) },
    target_ { std::move(target) },
    replacement_ { std::move(replacement) }
{}

rename_table::rename_table(util::clone_tag_t, rename_table const& other) noexcept:
    rename_table {
            other.schema_,
            other.target_,
            other.replacement_,
    }
{}

rename_table::rename_table(util::clone_tag_t, rename_table&& other) noexcept:
    rename_table {
            std::move(other.schema_),
            std::move(other.target_),
            std::move(other.replacement_),
    }
{}

statement_kind rename_table::kind() const noexcept {
    return tag;
}

rename_table* rename_table::clone() const & {
    return new rename_table(util::clone_tag, *this); // NOLINT
}

rename_table* rename_table::clone() && {
    return new rename_table(util::clone_tag, std::move(*this)); // NOLINT
}

descriptor::schema& rename_table::schema() noexcept {
    return schema_;
}

descriptor::schema const& rename_table::schema() const noexcept {
    return schema_;
}

descriptor::storage& rename_table::target() noexcept {
    return target_;
}

descriptor::storage const& rename_table::target() const noexcept {
    return target_;
}

rename_table::name_type& rename_table::replacement() noexcept {
    return replacement_;
}

rename_table::name_type const& rename_table::replacement() const noexcept {
    return replacement_;
}

bool operator==(rename_table const& a, rename_table const& b) noexcept {
    return a.schema() == b.schema()
        && a.target() == b.target()
        && a.replacement() == b.replacement();
}

bool operator!=(rename_table const& a, rename_table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, rename_table const& value) {
    return out << value.kind() << "("
               << "schema=" << value.schema() << ", "
               << "target=" << value.target() << ", "
               << "replacement=" << value.replacement() << ")";
}


bool rename_table::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<rename_table>(other);
}

std::ostream& rename_table::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
