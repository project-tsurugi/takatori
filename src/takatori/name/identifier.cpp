#include <takatori/name/identifier.h>

namespace takatori::name {

identifier::identifier(token_type token) noexcept :
    token_ { std::move(token) }
{}

identifier::token_type const& identifier::token() const noexcept {
    return token_;
}

identifier& identifier::token(token_type token) noexcept {
    token_ = std::move(token);
    return *this;
}

document::region& identifier::region() noexcept {
    return region_;
}

document::region const& identifier::region() const noexcept {
    return region_;
}

identifier::operator bool() const noexcept {
    return !token_.empty();
}

bool operator==(identifier const& a, identifier const& b) noexcept {
    return a.token() == b.token();
}

bool operator!=(identifier const& a, identifier const& b) noexcept {
    return !(a == b);
}

bool operator<(identifier const& a, identifier const& b) noexcept {
    return a.token() < b.token();
}

bool operator>(identifier const& a, identifier const& b) noexcept {
    return b < a;
}

bool operator<=(identifier const& a, identifier const& b) noexcept {
    return !(b < a);
}

bool operator>=(identifier const& a, identifier const& b) noexcept {
    return !(a < b);
}

std::ostream& operator<<(std::ostream& out, identifier const& value) {
    return out << value.token();
}

} // namespace takatori::name

std::size_t std::hash<takatori::name::identifier>::operator()(takatori::name::identifier const& v) const noexcept {
    std::hash<std::string_view> h {};
    return h(v.token());
}
