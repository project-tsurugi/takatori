#include <takatori/relation/intermediate/difference.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

difference::difference(
        quantifier_kind quantifier,
        std::vector<group_key_pair, util::object_allocator<group_key_pair>> group_key_pairs,
        util::object_creator creator) noexcept
    : inputs_({
            input_port_type { *this, left_index, creator },
            input_port_type { *this, right_index, creator },
    })
    , output_(*this, 0, creator)
    , quantifier_(quantifier)
    , group_key_pairs_(std::move(group_key_pairs))
{}

difference::difference(
        std::initializer_list<group_key_pair> group_key_pairs,
        quantifier_kind quantifier)
    : difference(
            quantifier,
            { group_key_pairs.begin(), group_key_pairs.end() })
{}

difference::difference(difference const& other, util::object_creator creator)
    : difference(
            other.quantifier_,
            decltype(group_key_pairs_) { other.group_key_pairs_, creator.allocator<group_key_pair>() },
            creator)
{}

difference::difference(difference&& other, util::object_creator creator)
    : difference(
            other.quantifier_,
            decltype(group_key_pairs_) { std::move(other.group_key_pairs_), creator.allocator<group_key_pair>() },
            creator)
{}

expression_kind difference::kind() const noexcept {
    return tag;
}

util::sequence_view<difference::input_port_type> difference::input_ports() noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<difference::input_port_type const> difference::input_ports() const noexcept {
    return util::sequence_view { inputs_ };
}

util::sequence_view<difference::output_port_type> difference::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<difference::output_port_type const> difference::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

difference* difference::clone(util::object_creator creator) const& {
    return creator.create_object<difference>(*this, creator);
}

difference* difference::clone(util::object_creator creator)&& {
    return creator.create_object<difference>(std::move(*this), creator);
}

difference::input_port_type& difference::left() noexcept {
    return inputs_[left_index];
}

difference::input_port_type const& difference::left() const noexcept {
    return inputs_[left_index];
}

difference::input_port_type& difference::right() noexcept {
    return inputs_[right_index];
}

difference::input_port_type const& difference::right() const noexcept {
    return inputs_[right_index];
}

difference::output_port_type& difference::output() noexcept {
    return output_;
}

difference::output_port_type const& difference::output() const noexcept {
    return output_;
}

difference::quantifier_kind difference::quantifier() const noexcept {
    return quantifier_;
}

difference& difference::quantifier(quantifier_kind quantifier) noexcept {
    quantifier_ = quantifier;
    return *this;
}

std::vector<difference::group_key_pair, util::object_allocator<difference::group_key_pair>>& difference::group_key_pairs() noexcept {
    return group_key_pairs_;
}

std::vector<difference::group_key_pair, util::object_allocator<difference::group_key_pair>> const& difference::group_key_pairs() const noexcept {
    return group_key_pairs_;
}

bool operator==(difference const& a, difference const& b) noexcept {
    return a.quantifier() == b.quantifier()
            && a.group_key_pairs() == b.group_key_pairs();
}

bool operator!=(difference const& a, difference const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, difference const& value) {
    return out << value.kind() << "("
               << "quantifier=" << value.quantifier() << ", "
               << "group_key_pairs=" << util::print_support { value.group_key_pairs() } << ")";
}

bool difference::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<difference>(other);
}

std::ostream& difference::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
