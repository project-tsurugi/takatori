#include <takatori/relation/details/emit_element.h>

#include <utility>

#include <takatori/util/hash.h>
#include <takatori/util/optional_print_support.h>

namespace takatori::relation::details {

emit_element::emit_element(descriptor::variable source) noexcept
    : source_(std::move(source))
{}

emit_element::emit_element(descriptor::variable source, name_type name) noexcept
    : source_(std::move(source))
    , name_(std::move(name))
{}

descriptor::variable& emit_element::source() noexcept {
    return source_;
}

descriptor::variable const& emit_element::source() const noexcept {
    return source_;
}

std::optional<std::string_view> emit_element::name() const noexcept {
    return std::optional<std::string_view> { name_ };
}

emit_element& emit_element::name(emit_element::name_type name) noexcept {
    name_ = std::move(name);
    return *this;
}

emit_element& emit_element::name(std::optional<name_type> name) noexcept {
    name_ = std::move(name);
    return *this;
}

bool operator==(emit_element const& a, emit_element const& b) noexcept {
    return a.source() == b.source()
        && a.name() == b.name();
}

bool operator!=(emit_element const& a, emit_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, emit_element const& value) {
    return out << "sort_key" << "("
               << "variable=" << value.source() << ", "
               << "name=" << util::print_support { value.name() } << ")";
}

} // namespace takatori::relation::details

std::size_t std::hash<takatori::relation::details::emit_element>::operator()(
        takatori::relation::details::emit_element const& value) const noexcept {
    return takatori::util::hash(value.source(), value.name());
}
