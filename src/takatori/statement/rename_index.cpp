#include <takatori/statement/rename_index.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

rename_index::rename_index(
        descriptor::schema schema,
        descriptor::relation target,
        name_type replacement) noexcept:
    schema_ { std::move(schema) },
    target_ { std::move(target) },
    replacement_ { std::move(replacement) }
{}

rename_index::rename_index(util::clone_tag_t, rename_index const& other) noexcept:
    rename_index {
            other.schema_,
            other.target_,
            other.replacement_,
    }
{}

rename_index::rename_index(util::clone_tag_t, rename_index&& other) noexcept:
    rename_index {
            std::move(other.schema_),
            std::move(other.target_),
            std::move(other.replacement_),
    }
{}

statement_kind rename_index::kind() const noexcept {
    return tag;
}

rename_index* rename_index::clone() const & {
    return new rename_index(util::clone_tag, *this); // NOLINT
}

rename_index* rename_index::clone() && {
    return new rename_index(util::clone_tag, std::move(*this)); // NOLINT
}

descriptor::schema& rename_index::schema() noexcept {
    return schema_;
}

descriptor::schema const& rename_index::schema() const noexcept {
    return schema_;
}

descriptor::relation& rename_index::target() noexcept {
    return target_;
}

descriptor::relation const& rename_index::target() const noexcept {
    return target_;
}

rename_index::name_type& rename_index::replacement() noexcept {
    return replacement_;
}

rename_index::name_type const& rename_index::replacement() const noexcept {
    return replacement_;
}

bool operator==(rename_index const& a, rename_index const& b) noexcept {
    return a.schema() == b.schema()
        && a.target() == b.target()
        && a.replacement() == b.replacement();
}

bool operator!=(rename_index const& a, rename_index const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, rename_index const& value) {
    return out << value.kind() << "("
               << "schema=" << value.schema() << ", "
               << "target=" << value.target() << ", "
               << "replacement=" << value.replacement() << ")";
}


bool rename_index::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<rename_index>(other);
}

std::ostream& rename_index::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
