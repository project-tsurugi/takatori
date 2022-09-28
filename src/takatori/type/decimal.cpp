#include <takatori/type/decimal.h>

#include <takatori/util/downcast.h>

namespace takatori::type {

type_kind decimal::kind() const noexcept {
    return tag;
}

decimal* decimal::clone() const& {
     return new decimal(precision_, scale_); // NOLINT
}

decimal* decimal::clone() && {
     return new decimal(std::move(precision_), scale_); // NOLINT
}

bool operator==(decimal const& a, decimal const& b) noexcept {
    return a.precision_ == b.precision_
        && a.scale_ == b.scale_;
}

bool operator!=(decimal const& a, decimal const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, decimal const& value) {
    out << "decimal(";
    if (value.precision_) {
        out << *value.precision_;
    } else {
        out << "*";
    }
    out << ", ";
    if (value.scale_) {
        out << *value.scale_;
    } else {
        out << "*";
    }
    out << ")";
    return out;
}

bool decimal::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<decimal>(other);
}

std::size_t
decimal::hash() const noexcept {
    constexpr auto flexible = static_cast<std::size_t>(-1);
    return static_cast<std::size_t>(precision_.value_or(flexible)) * 31
        + static_cast<std::size_t>(scale_.value_or(flexible));
}

std::ostream& decimal::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
