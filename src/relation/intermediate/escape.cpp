#include <takatori/relation/intermediate/escape.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>
#include <takatori/util/pair_print_support.h>

namespace takatori::relation::intermediate {

escape::escape(
        std::vector<mapping, util::object_allocator<mapping>> mappings,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , mappings_(std::move(mappings))
{}

escape::escape(std::initializer_list<mapping> mappings)
    : escape({ mappings.begin(), mappings.end() })
{}

escape::escape(escape const& other, util::object_creator creator)
    : escape(
            decltype(mappings_) { other.mappings_, creator.allocator<mapping>() },
            creator)
{}

escape::escape(escape&& other, util::object_creator creator)
    : escape(
            decltype(mappings_) { std::move(other.mappings_), creator.allocator<mapping>() },
            creator)
{}

expression_kind escape::kind() const noexcept {
    return tag;
}

util::sequence_view<escape::input_port_type> escape::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<escape::input_port_type const> escape::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<escape::output_port_type> escape::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<escape::output_port_type const> escape::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

escape* escape::clone(util::object_creator creator) const& {
    return creator.create_object<escape>(*this, creator);
}

escape* escape::clone(util::object_creator creator)&& {
    return creator.create_object<escape>(std::move(*this), creator);
}

escape::input_port_type& escape::input() noexcept {
    return input_;
}

escape::input_port_type const& escape::input() const noexcept {
    return input_;
}

escape::output_port_type& escape::output() noexcept {
    return output_;
}

escape::output_port_type const& escape::output() const noexcept {
    return output_;
}

std::vector<escape::mapping, util::object_allocator<escape::mapping>>& escape::mappings() noexcept {
    return mappings_;
}

std::vector<escape::mapping, util::object_allocator<escape::mapping>> const& escape::mappings() const noexcept {
    return mappings_;
}

bool operator==(escape const& a, escape const& b) noexcept {
    return a.mappings() == b.mappings();
}

bool operator!=(escape const& a, escape const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, escape const& value) {
    return out << value.kind() << "("
               << "mappings=" << util::print_support { value.mappings() } << ")";
}

bool escape::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<escape>(other);
}

std::ostream& escape::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
