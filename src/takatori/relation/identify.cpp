#include <takatori/relation/identify.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation {

identify::identify(
        descriptor::variable variable,
        std::shared_ptr<type::row_id const> type) noexcept :
    input_ { *this, 0 },
    output_ { *this, 0 },
    variable_ { std::move(variable) },
    type_ { std::move(type) }
{}


identify::identify(descriptor::variable variable, type::row_id&& type) :
    identify {
            std::move(variable),
            util::clone_shared(std::move(type)),
    }
{}

identify::identify(util::clone_tag_t, identify const& other) :
    identify {
            other.variable_,
            other.type_,
    }
{}

identify::identify(util::clone_tag_t, identify&& other) :
    identify {
            std::move(other.variable_),
            std::move(other.type_),
    }
{}

expression_kind identify::kind() const noexcept {
    return tag;
}

util::sequence_view<identify::input_port_type> identify::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<identify::input_port_type const> identify::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<identify::output_port_type> identify::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<identify::output_port_type const> identify::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

identify* identify::clone() const& {
    return new identify(util::clone_tag, *this); // NOLINT
}

identify* identify::clone()&& {
    return new identify(util::clone_tag, std::move(*this)); // NOLINT;
}

identify::input_port_type& identify::input() noexcept {
    return input_;
}

identify::input_port_type const& identify::input() const noexcept {
    return input_;
}

identify::output_port_type& identify::output() noexcept {
    return output_;
}

identify::output_port_type const& identify::output() const noexcept {
    return output_;
}

descriptor::variable& identify::variable() noexcept {
    return variable_;
}

descriptor::variable const& identify::variable() const noexcept {
    return variable_;
}

type::row_id const& identify::type() const noexcept {
    return *type_;
}

util::optional_ptr<type::row_id const> identify::optional_type() const noexcept {
    return util::optional_ptr { type_.get() };
}

std::shared_ptr<type::row_id const> identify::shared_type() const noexcept {
    return type_;
}

identify& identify::type(std::shared_ptr<type::row_id const> type) noexcept {
    type_ = std::move(type);
    return *this;
}

bool operator==(identify const& a, identify const& b) noexcept {
    return a.variable() == b.variable()
            && a.optional_type() == b.optional_type();
}

bool operator!=(identify const& a, identify const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, identify const& value) {
    return out << value.kind() << "("
               << "variable=" << value.variable() << ", "
               << "type=" << value.optional_type() << ")";
}

bool identify::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<identify>(other);
}

std::ostream& identify::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
