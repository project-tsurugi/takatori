#include <takatori/value/bit.h>

#include <takatori/util/exception.h>

namespace takatori::value {

using ::takatori::util::throw_exception;

bit::bit(entity_type value) noexcept
    : entity_(std::move(value))
{}

bit::bit(std::string_view bits) {
    entity_.reserve(bits.size());
    for (auto iter = bits.rbegin(); iter != bits.rend(); ++iter) {
        auto c = *iter;
        if (c == '0') {
            entity_.push_back(false);
        } else if (c == '1') {
            entity_.push_back(true);
        } else {
            throw_exception(std::invalid_argument(std::string { bits }));
        }
    }
}

value_kind bit::kind() const noexcept {
    return tag;
}

bit* bit::clone() const& {
     return new bit(entity_); // NOLINT
}

bit* bit::clone() && {
     return new bit(std::move(entity_)); // NOLINT
}

bit::view_type bit::get() const noexcept {
    return entity_;
}

bit::operator view_type() const noexcept {
    return get();
}

bool bit::equals(data const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<bit>(other);
}

std::size_t bit::hash() const noexcept {
    constexpr auto nbits = sizeof(block_type) * 8;
    std::size_t result = 0;
    for (entity_type::size_type i = 0, n = entity_.size(); i < n; ++i) {
        if (entity_[i]) {
            result ^= static_cast<std::size_t>(1) << (i % nbits);
        }
    }
    result = result * 31 + entity_.size();
    return result;
}

std::ostream& bit::print_to(std::ostream& out) const {
    return out << *this;
}

bool operator==(bit const& a, bit const& b) noexcept {
    return a.get() == b.get();
}

bool operator!=(bit const& a, bit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, bit const& value) {
    return out << "bit(" << value.get() << ")";
}

} // namespace takatori::value
