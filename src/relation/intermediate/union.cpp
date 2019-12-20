#include <takatori/relation/intermediate/union.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>
#include <takatori/util/pair_print_support.h>

namespace takatori::relation::intermediate {

union_::union_(
        std::vector<mapping, util::object_allocator<mapping>> mappings,
        util::object_creator creator) noexcept
    : inputs_({
            input_port_type { *this, left_index, creator },
            input_port_type { *this, right_index, creator },
    })
    , output_(*this, 0, creator)
    , mappings_(std::move(mappings))
{}

union_::union_(std::initializer_list<mapping> mappings)
    : union_({ mappings.begin(), mappings.end() })
{}

union_::union_(union_ const& other, util::object_creator creator)
    : union_(
            decltype(mappings_) { other.mappings_, creator.allocator<mapping>() },
            creator)
{}

union_::union_(union_&& other, util::object_creator creator)
    : union_(
            decltype(mappings_) { other.mappings_, creator.allocator<mapping>() },
            creator)
{}

expression_kind union_::kind() const noexcept {
    return tag;
}

util::sequence_view<union_::input_port_type> union_::input_ports() noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<union_::input_port_type const> union_::input_ports() const noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<union_::output_port_type> union_::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<union_::output_port_type const> union_::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

union_* union_::clone(util::object_creator creator) const& {
    return creator.create_object<union_>(*this, creator);
}

union_* union_::clone(util::object_creator creator)&& {
    return creator.create_object<union_>(std::move(*this), creator);
}

union_::input_port_type& union_::left() noexcept {
    return inputs_[left_index];
}

union_::input_port_type const& union_::left() const noexcept {
    return inputs_[left_index];
}

union_::input_port_type& union_::right() noexcept {
    return inputs_[right_index];
}

union_::input_port_type const& union_::right() const noexcept {
    return inputs_[right_index];
}

union_::output_port_type& union_::output() noexcept {
    return output_;
}

union_::output_port_type const& union_::output() const noexcept {
    return output_;
}

std::vector<union_::mapping, util::object_allocator<union_::mapping>>& union_::mappings() noexcept {
    return mappings_;
}

std::vector<union_::mapping, util::object_allocator<union_::mapping>> const& union_::mappings() const noexcept {
    return mappings_;
}

bool operator==(union_ const& a, union_ const& b) noexcept {
    return a.mappings() == b.mappings();
}

bool operator!=(union_ const& a, union_ const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, union_ const& value) {
    return out << value.kind() << "("
               << "mappings=" << util::print_support { value.mappings() } << ")";
}

bool union_::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<union_>(other);
}

std::ostream& union_::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
