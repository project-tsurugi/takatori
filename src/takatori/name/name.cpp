#include <takatori/name/name.h>

#include <takatori/util/hash.h>

namespace takatori::name {

name::name(std::vector<identifier_type> identifiers) noexcept :
    identifiers_ { std::move(identifiers) }
{}

name::name(std::initializer_list<std::string_view> identifiers) {
    identifiers_.reserve(identifiers.size());
    for (auto element : identifiers) {
        identifiers_.emplace_back(std::string { element });
    }
}

std::vector<name::identifier_type> const& name::identifiers() const noexcept {
    return identifiers_;
}

name& name::identifiers(std::vector<identifier_type> identifiers) noexcept {
    identifiers_ = std::move(identifiers);
    return *this;
}

document::region& name::region() noexcept {
    return region_;
}

document::region const& name::region() const noexcept {
    return region_;
}

bool name::empty() const noexcept {
    return identifiers_.empty();
}

name::const_reference name::operator[](size_type position) const {
    return identifiers_[position];
}

name::const_reference name::front() const {
    return identifiers_.front();
}

name::const_reference name::back() const {
    return identifiers_.back();
}

name::size_type name::size() const noexcept {
    return identifiers_.size();
}

name::iterator name::begin() const noexcept {
    return identifiers_.cbegin();
}

name::iterator name::end() const noexcept {
    return identifiers_.cend();
}

std::reverse_iterator<name::iterator> name::rbegin() const noexcept {
    return std::make_reverse_iterator(end());
}

std::reverse_iterator<name::iterator> name::rend() const noexcept {
    return std::make_reverse_iterator(begin());
}

name::operator bool() const noexcept {
    return !empty();
}

bool operator==(name const& a, name const& b) noexcept {
    return a.identifiers() == b.identifiers();
}

bool operator==(name const& a, identifier const& b) noexcept {
    return a.size() == 1 && a[0] == b;
}

bool operator==(identifier const& a, name const& b) noexcept {
    return b == a;
}

bool operator!=(name const& a, name const& b) noexcept {
    return !(a == b);
}

bool operator!=(name const& a, identifier const& b) noexcept {
    return !(a == b);
}

bool operator!=(identifier const& a, name const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, name const& value) {
    bool cont = false;
    for (auto const& element : value) {
        if (cont) {
            out << ".";
        }
        out << element;
        cont = true;
    }
    return out;
}

} // namespace takatori::name

std::size_t std::hash<takatori::name::name>::operator()(takatori::name::name const& v) const noexcept {
    std::hash<takatori::name::identifier> h {};
    std::size_t result = 0;
    for (auto const& element : v) {
        result *= takatori::util::hash_prime;
        result += h(element);
    }
    return result;
}
