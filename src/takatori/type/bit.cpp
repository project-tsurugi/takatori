#include <takatori/type/bit.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind bit::kind() const noexcept {
    return tag;
}

bit* bit::clone() const& {
     return new bit(varying_t { varying_ }, length_); // NOLINT
}

bit* bit::clone() && {
     return new bit(varying_t { varying_ }, length_); // NOLINT
}

bool operator==(bit const& a, bit const& b) noexcept {
    return a.varying_ == b.varying_
        && a.length_ == b.length_;
}

bool operator!=(bit const& a, bit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, bit const& value) {
    out << "bit";
    if (value.varying_) {
        out << "-varying";
    }
    out << "(";
    if (value.length_) {
        out << value.length_.value();
    } else {
        out << "*";
    }
    out << ")";
    return out;
}

bool bit::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<bit>(other);
}

std::size_t
bit::hash() const noexcept {
    return (varying_ ? 0 : 1) * 31
           + static_cast<std::size_t>(length_.value_or(-1));
}

std::ostream& bit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
