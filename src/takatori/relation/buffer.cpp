#include <takatori/relation/buffer.h>

#include <memory>
#include <stdexcept>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/exception.h>
#include <takatori/util/print_support.h>

namespace takatori::relation {

using util::throw_exception;

buffer::buffer(size_type size, bool stop_with_primary) :
    input_ { *this, 0 },
    stop_with_primary_ { stop_with_primary }
{
    if (size == 0) {
        throw_exception(std::invalid_argument("buffer must have at least one output"));
    }
    outputs_.reserve(size);
    for (size_type i = 0; i < size; ++i) {
        outputs_.emplace_back(*this, i);
    }
}

buffer::buffer(util::clone_tag_t, buffer const& other) :
    buffer {
            other.outputs_.size(),
            other.stop_with_primary_,
    }
{}

buffer::buffer(util::clone_tag_t, buffer&& other) :
    buffer {
            other.outputs_.size(),
            other.stop_with_primary_,
    }
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

buffer::output_port_type& buffer::primary_output() noexcept {
    return outputs_.front();
}

buffer::output_port_type const& buffer::primary_output() const noexcept {
    return outputs_.front();
}

bool buffer::stop_with_primary() const noexcept {
    return stop_with_primary_;
}

buffer& buffer::stop_with_primary(bool enabled) noexcept {
    stop_with_primary_ = enabled;
    return *this;
}

bool operator==(buffer const& a, buffer const& b) noexcept {
    if (std::addressof(a) == std::addressof(b)) {
        return true;
    }
    return a.size() == b.size()
        && a.stop_with_primary() == b.stop_with_primary();
}

bool operator!=(buffer const& a, buffer const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, buffer const& value) {
    return out << value.kind() << "("
               << "size=" << value.size() << ", "
               << "stop_with_primary=" << util::print_support { value.stop_with_primary() } << ")";
}

bool buffer::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<buffer>(other);
}

std::ostream& buffer::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
