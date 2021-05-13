#include <takatori/plan/discard.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

discard::discard(util::clone_tag_t, discard const&) : discard {} {}

discard::discard(util::clone_tag_t, discard&&) : discard {} {}

step_kind discard::kind() const noexcept {
    return tag;
}

discard* discard::clone() const& {
    return new discard(util::clone_tag, *this); // NOLINT
}

discard* discard::clone() && {
    return new discard(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<descriptor::variable const> discard::input_columns() const noexcept {
    return {};
}

util::sequence_view<descriptor::variable const> discard::output_columns() const noexcept {
    return {};
}

bool operator==(discard const&, discard const&) noexcept {
    return true;
}

bool operator!=(discard const& a, discard const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, discard const& value) {
    return out << value.kind() << "(" << ")";
}

bool discard::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<discard>(other);
}

std::ostream& discard::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::plan
