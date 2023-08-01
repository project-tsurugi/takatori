#include <takatori/type/octet.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind octet::kind() const noexcept {
    return tag;
}

octet* octet::clone() const& {
     return new octet(varying_t { varying_ }, length_); // NOLINT
}

octet* octet::clone() && {
     return new octet(varying_t { varying_ }, std::move(length_)); // NOLINT
}

bool operator==(octet const& a, octet const& b) noexcept {
    return a.varying_ == b.varying_
        && a.length_ == b.length_;
}

bool operator!=(octet const& a, octet const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, octet const& value) {
    out << "octet";
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

bool octet::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<octet>(other);
}

std::size_t
octet::hash() const noexcept {
    return (varying_ ? 0U : 1U) * 31U
           + static_cast<std::size_t>(length_.value_or(-1));
}

std::ostream& octet::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
