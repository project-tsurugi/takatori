#include <takatori/relation/buffer.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation {

buffer::buffer(size_type size)
    : input_ { *this, 0 }
{
    outputs_.reserve(size);
    for (size_type i = 0; i < size; ++i) {
        outputs_.emplace_back(*this, i);
    }
}

buffer::buffer(util::clone_tag_t, buffer const& other)
    : buffer(
            other.outputs_.size())
{}

buffer::buffer(util::clone_tag_t, buffer&& other)
    : buffer(
            other.outputs_.size())
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

buffer* buffer::clone() const& {
    return new buffer(util::clone_tag, *this); // NOLINT
}

buffer* buffer::clone()&& {
    return new buffer(util::clone_tag, std::move(*this)); // NOLINT;
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
