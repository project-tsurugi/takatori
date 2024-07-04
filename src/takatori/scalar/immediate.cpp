#include <takatori/scalar/immediate.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::scalar {

immediate::immediate(
        std::shared_ptr<value::data const> value,
        std::shared_ptr<type::data const> type) noexcept
    : value_(std::move(value))
    , type_(std::move(type))
{}

immediate::immediate(value::data&& value, type::data&& type)
    : immediate(
            util::clone_shared(std::move(value)),
            util::clone_shared(std::move(type)))
{}

immediate::immediate(util::clone_tag_t, immediate const& other) noexcept
    : immediate(other.value_, other.type_)
{}

immediate::immediate(util::clone_tag_t, immediate&& other) noexcept
    : immediate(std::move(other.value_), std::move(other.type_))
{}

expression_kind immediate::kind() const noexcept {
    return tag;
}

immediate* immediate::clone() const& {
    return new immediate(util::clone_tag, *this); // NOLINT
}

immediate* immediate::clone() && {
    return new immediate(util::clone_tag, std::move(*this)); // NOLINT;
}

value::data const& immediate::value() const noexcept {
    return *value_;
}

util::optional_ptr<value::data const> immediate::optional_value() const noexcept {
    return util::optional_ptr { value_.get() };
}

std::shared_ptr<value::data const> immediate::shared_value() const noexcept {
    return value_;
}

immediate& immediate::value(std::shared_ptr<value::data const> value) noexcept {
    value_ = std::move(value);
    return *this;
}

type::data const& immediate::type() const noexcept {
    return *type_;
}

util::optional_ptr<type::data const> immediate::optional_type() const noexcept {
    return util::optional_ptr { type_.get() };
}

std::shared_ptr<type::data const> immediate::shared_type() const noexcept {
    return type_;
}

immediate& immediate::type(std::shared_ptr<type::data const> type) noexcept {
    type_ = std::move(type);
    return *this;
}

bool operator==(immediate const& a, immediate const& b) noexcept {
    return a.optional_value() == b.optional_value()
        && a.optional_type() == b.optional_type();
}

bool operator!=(immediate const& a, immediate const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, immediate const& value) {
    return out << value.kind() << "("
               << "value=" << value.value() << ", "
               << "type=" << value.optional_type() << ")";
}

bool immediate::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<immediate>(other);
}

std::ostream& immediate::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
