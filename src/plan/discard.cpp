#include "takatori/plan/discard.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"
#include "takatori/util/optional_print_support.h"
#include "takatori/util/vector_print_support.h"

namespace takatori::plan {

discard::discard(util::object_creator creator) noexcept
    : exchange(creator)
{}

discard::discard(discard const&, util::object_creator creator)
    : discard(creator)
{}

discard::discard(discard&&, util::object_creator creator)
    : discard(creator)
{}

step_kind discard::kind() const noexcept {
    return tag;
}

discard* discard::clone(util::object_creator creator) const& {
    return creator.create_object<discard>(*this, creator);
}

discard* discard::clone(util::object_creator creator) && {
    return creator.create_object<discard>(std::move(*this), creator);
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
