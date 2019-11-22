#include "takatori/relation/intermediate/intersection.h"

#include "takatori/util/downcast.h"
#include "takatori/util/vector_print_support.h"

namespace takatori::relation::intermediate {

intersection::intersection(
        std::vector<key_pair, util::object_allocator<key_pair>> key_pairs,
        util::object_creator creator) noexcept
    : inputs_({
            input_port_type { *this, left_index, creator },
            input_port_type { *this, right_index, creator },
    })
    , output_(*this, 0, creator)
    , key_pairs_(std::move(key_pairs))
{}

intersection::intersection(std::initializer_list<key_pair> key_pairs)
    : intersection({ key_pairs.begin(), key_pairs.end() })
{}

intersection::intersection(intersection const& other, util::object_creator creator)
    : intersection(
            decltype(key_pairs_) { other.key_pairs_, creator.allocator<key_pair>() },
            creator)
{}

intersection::intersection(intersection&& other, util::object_creator creator)
    : intersection(
            decltype(key_pairs_) { std::move(other.key_pairs_), creator.allocator<key_pair>() },
            creator)
{}

expression_kind intersection::kind() const noexcept {
    return tag;
}

util::sequence_view<intersection::input_port_type> intersection::input_ports() noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<intersection::input_port_type const> intersection::input_ports() const noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<intersection::output_port_type> intersection::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<intersection::output_port_type const> intersection::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

intersection* intersection::clone(util::object_creator creator) const& {
    return creator.create_object<intersection>(*this, creator);
}

intersection* intersection::clone(util::object_creator creator)&& {
    return creator.create_object<intersection>(std::move(*this), creator);
}

intersection::input_port_type& intersection::left() noexcept {
    return inputs_[left_index];
}

intersection::input_port_type const& intersection::left() const noexcept {
    return inputs_[left_index];
}

intersection::input_port_type& intersection::right() noexcept {
    return inputs_[right_index];
}

intersection::input_port_type const& intersection::right() const noexcept {
    return inputs_[right_index];
}

intersection::output_port_type& intersection::output() noexcept {
    return output_;
}

intersection::output_port_type const& intersection::output() const noexcept {
    return output_;
}

std::vector<intersection::key_pair, util::object_allocator<intersection::key_pair>>& intersection::key_pairs() noexcept {
    return key_pairs_;
}

std::vector<intersection::key_pair, util::object_allocator<intersection::key_pair>> const& intersection::key_pairs() const noexcept {
    return key_pairs_;
}

bool operator==(intersection const& a, intersection const& b) noexcept {
    return a.key_pairs() == b.key_pairs();
}

bool operator!=(intersection const& a, intersection const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, intersection const& value) {
    return out << value.kind() << "("
               << "key_pairs=" << util::print_support { value.key_pairs() } << ")";
}

bool intersection::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<intersection>(other);
}

std::ostream& intersection::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
