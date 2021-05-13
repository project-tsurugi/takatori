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
        out << value.precision_.value();
    } else {
        out << "*";
    }
    out << ", " << value.scale_ << ")";
    return out;
}

bool decimal::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<decimal>(other);
}

std::size_t
decimal::hash() const noexcept {
    return static_cast<std::size_t>(precision_.value_or(0)) * 31
        + static_cast<std::size_t>(scale_);
}

std::ostream& decimal::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::type
