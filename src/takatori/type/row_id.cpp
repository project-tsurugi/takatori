#include <takatori/type/row_id.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind row_id::kind() const noexcept {
    return tag;
}

row_id* row_id::clone(util::object_creator creator) const& {
    return creator.create_object<row_id>(sequence_);
}

row_id* row_id::clone(util::object_creator creator) && {
    return creator.create_object<row_id>(sequence_);
}

bool operator==(row_id const& a, row_id const& b) noexcept {
    return a.sequence_ == b.sequence_;
}

bool operator!=(row_id const& a, row_id const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, row_id const& value) {
    return out << "row_id" << "("
               << "sequence=" << value.sequence() << ")";
}

bool row_id::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<row_id>(other);
}

std::size_t
row_id::hash() const noexcept {
    return sequence_;
}

std::ostream& row_id::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
