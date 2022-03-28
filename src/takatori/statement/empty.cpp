#include <takatori/statement/empty.h>

#include <takatori/util/downcast.h>

namespace takatori::statement {

empty::empty(util::clone_tag_t, empty const&) noexcept :
    empty {}
{}

empty::empty(util::clone_tag_t, empty&&) noexcept :
    empty {}
{}

statement_kind empty::kind() const noexcept {
    return tag;
}

empty* empty::clone() const& {
    return new empty(util::clone_tag, *this); // NOLINT
}

empty* empty::clone() && {
    return new empty(util::clone_tag, std::move(*this)); // NOLINT;
}

bool operator==(empty const&, empty const&) noexcept {
    return true;
}

bool operator!=(empty const&, empty const&) noexcept {
    return false;
}

std::ostream& operator<<(std::ostream& out, empty const& value) {
    return out << value.kind() << "(" << ")";
}

bool empty::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<empty>(other);
}

std::ostream& empty::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
