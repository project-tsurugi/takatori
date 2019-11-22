#include "takatori/relation/buffer.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"

namespace takatori::relation {

buffer::buffer(
        size_type size,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , outputs_(decltype(outputs_)::allocator_type(creator.allocator<decltype(outputs_)::value_type>()))
{
    outputs_.reserve(size);
    for (size_type i = 0; i < size; ++i) {
        outputs_.emplace_back(*this, i, creator);
    }
}

buffer::buffer(buffer const& other, util::object_creator creator)
    : buffer(
            other.outputs_.size(),
            creator)
{}

buffer::buffer(buffer&& other, util::object_creator creator)
    : buffer(
            other.outputs_.size(),
            creator)
{}

expression_kind buffer::kind() const noexcept {
    return tag;
}

util::sequence_view<buffer::input_port_type> buffer::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<buffer::input_port_type const> buffer::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<buffer::output_port_type> buffer::output_ports() noexcept {
    return util::sequence_view { outputs_ };
}

util::sequence_view<buffer::output_port_type const> buffer::output_ports() const noexcept {
    return util::sequence_view { outputs_ };
}

buffer* buffer::clone(util::object_creator creator) const& {
    return creator.create_object<buffer>(*this, creator);
}

buffer* buffer::clone(util::object_creator creator)&& {
    return creator.create_object<buffer>(std::move(*this), creator);
}

buffer::input_port_type& buffer::input() noexcept {
    return input_;
}

buffer::input_port_type const& buffer::input() const noexcept {
    return input_;
}

buffer::size_type buffer::size() const noexcept {
    return outputs_.size();
}

bool operator==(buffer const& a, buffer const& b) noexcept {
    return a.size() == b.size();
}

bool operator!=(buffer const& a, buffer const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, buffer const& value) {
    return out << value.kind() << "(" << value.size() << ")";
}

bool buffer::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<buffer>(other);
}

std::ostream& buffer::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
