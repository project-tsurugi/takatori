#include <takatori/statement/truncate_table.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

truncate_table::truncate_table(
        descriptor::storage target,
        option_set_type options) noexcept :
    target_ { std::move(target) },
    options_ { options }
{}

truncate_table::truncate_table(util::clone_tag_t, truncate_table const& other) noexcept :
    truncate_table {
            other.target_,
            other.options_,
    }
{}

truncate_table::truncate_table(util::clone_tag_t, truncate_table&& other) noexcept :
    truncate_table {
            std::move(other.target_),
            other.options_,
    }
{}

statement_kind truncate_table::kind() const noexcept {
    return tag;
}

truncate_table* truncate_table::clone() const & {
    return new truncate_table(util::clone_tag, *this); // NOLINT
}

truncate_table* truncate_table::clone() && {
    return new truncate_table(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::storage& truncate_table::target() noexcept {
    return target_;
}

descriptor::storage const& truncate_table::target() const noexcept {
    return target_;
}

truncate_table::option_set_type& truncate_table::options() noexcept {
    return options_;
}

truncate_table::option_set_type const& truncate_table::options() const noexcept {
    return options_;
}

bool truncate_table::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<truncate_table>(other);
}

std::ostream& truncate_table::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(truncate_table const& a, truncate_table const& b) noexcept {
    return a.target() == b.target()
        && a.options() == b.options();
}

bool operator!=(truncate_table const& a, truncate_table const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, truncate_table const& value) {
    return out << value.kind() << "("
               << "target=" << value.target() << ", "
               << "options=" << value.options() << ")";
}

} // namespace takatori::statement
