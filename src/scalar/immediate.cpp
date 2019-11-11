#include "takatori/scalar/immediate.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"

namespace takatori::scalar {

immediate::immediate(
        descriptor::value value,
        std::shared_ptr<type::data_type const> data_type) noexcept
    : value_(std::move(value))
    , data_type_(std::move(data_type))
{}

immediate::immediate(descriptor::value value, type::data_type&& data_type)
    : immediate(
            std::move(value),
            util::clone_shared(std::move(data_type)))
{}

immediate::immediate(immediate const& other, util::object_creator) noexcept
    : immediate(other.value_, other.data_type_)
{}

immediate::immediate(immediate&& other, util::object_creator) noexcept
    : immediate(std::move(other.value_), std::move(other.data_type_))
{}

expression::parent_type* immediate::parent_element() noexcept {
    return parent_;
}

expression::parent_type const* immediate::parent_element() const noexcept {
    return parent_;
}

void immediate::parent_element(expression::parent_type* parent) noexcept {
    parent_ = parent;
}

expression_kind immediate::kind() const noexcept {
    return tag;
}

immediate* immediate::clone(util::object_creator creator) const& {
    return creator.create_object<immediate>(*this, creator);
}

immediate* immediate::clone(util::object_creator creator) && {
    return creator.create_object<immediate>(std::move(*this), creator);
}

descriptor::value const& immediate::value() const noexcept {
    return value_;
}

immediate& immediate::value(descriptor::value value) noexcept {
    value_ = std::move(value);
    return *this;
}

type::data_type const& immediate::data_type() const noexcept {
    return *data_type_;
}

util::optional_ptr<type::data_type const> immediate::optional_data_type() const noexcept {
    return util::optional_ptr { data_type_.get() };
}

std::shared_ptr<type::data_type const> immediate::shared_data_type() const noexcept {
    return data_type_;
}

immediate& immediate::data_type(std::shared_ptr<type::data_type const> data_type) noexcept {
    data_type_ = std::move(data_type);
    return *this;
}

bool operator==(immediate const& a, immediate const& b) noexcept {
    return a.value() == b.value()
        && b.optional_data_type() == b.optional_data_type();
}

bool operator!=(immediate const& a, immediate const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, immediate const& value) {
    return out << "immediate("
               << "value=" << value.value() << ", "
               << "data_type=" << value.optional_data_type() << ")";
}

bool immediate::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<immediate>(other);
}

std::ostream& immediate::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
