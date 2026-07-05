#include <takatori/statement/rename_column.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

rename_column::rename_column(
        descriptor::storage table,
        descriptor::variable target,
        name_type replacement) noexcept:
    table_ { std::move(table) },
    target_ { std::move(target) },
    replacement_ { std::move(replacement) }
{}

rename_column::rename_column(util::clone_tag_t, rename_column const& other) noexcept:
    rename_column {
            other.table_,
            other.target_,
            other.replacement_,
    }
{}

rename_column::rename_column(util::clone_tag_t, rename_column&& other) noexcept:
    rename_column {
            std::move(other.table_),
            std::move(other.target_),
            std::move(other.replacement_),
    }
{}

statement_kind rename_column::kind() const noexcept {
    return tag;
}

rename_column* rename_column::clone() const & {
    return new rename_column(util::clone_tag, *this); // NOLINT
}

rename_column* rename_column::clone() && {
    return new rename_column(util::clone_tag, std::move(*this)); // NOLINT
}

descriptor::storage& rename_column::table() noexcept {
    return table_;
}

descriptor::storage const& rename_column::table() const noexcept {
    return table_;
}

descriptor::variable& rename_column::target() noexcept {
    return target_;
}

descriptor::variable const& rename_column::target() const noexcept {
    return target_;
}

rename_column::name_type& rename_column::replacement() noexcept {
    return replacement_;
}

rename_column::name_type const& rename_column::replacement() const noexcept {
    return replacement_;
}

bool operator==(rename_column const& a, rename_column const& b) noexcept {
    return a.table() == b.table()
        && a.target() == b.target()
        && a.replacement() == b.replacement();
}

bool operator!=(rename_column const& a, rename_column const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, rename_column const& value) {
    return out << value.kind() << "("
               << "table=" << value.table() << ", "
               << "target=" << value.target() << ", "
               << "replacement=" << value.replacement() << ")";
}


bool rename_column::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<rename_column>(other);
}

std::ostream& rename_column::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
