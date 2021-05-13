#include <takatori/relation/intermediate/union.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>
#include <takatori/util/pair_print_support.h>

namespace takatori::relation::intermediate {

union_::union_(
        quantifier_kind quantifier,
        std::vector<mapping> mappings) noexcept
    : inputs_({
            input_port_type { *this, left_index },
            input_port_type { *this, right_index },
    })
    , output_(*this, 0)
    , quantifier_(quantifier)
    , mappings_(std::move(mappings))
{}

union_::union_(
        std::initializer_list<mapping> mappings,
        quantifier_kind quantifier)
    : union_(
            quantifier,
            { mappings.begin(), mappings.end() })
{}

union_::union_(util::clone_tag_t, union_ const& other)
    : union_(
            other.quantifier_,
            decltype(mappings_) { other.mappings_ })
{}

union_::union_(util::clone_tag_t, union_&& other)
    : union_(
            other.quantifier_,
            decltype(mappings_) { other.mappings_ })
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

union_* union_::clone() const& {
    return new union_(util::clone_tag, *this); // NOLINT
}

union_* union_::clone()&& {
    return new union_(util::clone_tag, std::move(*this)); // NOLINT;
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

union_::quantifier_kind union_::quantifier() const noexcept {
    return quantifier_;
}

union_& union_::quantifier(quantifier_kind quantifier) noexcept {
    quantifier_ = quantifier;
    return *this;
}

std::vector<union_::mapping>& union_::mappings() noexcept {
    return mappings_;
}

std::vector<union_::mapping> const& union_::mappings() const noexcept {
    return mappings_;
}

bool operator==(union_ const& a, union_ const& b) noexcept {
    return a.quantifier() == b.quantifier()
            && a.mappings() == b.mappings();
}

bool operator!=(union_ const& a, union_ const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, union_ const& value) {
    return out << value.kind() << "("
               << "quantifier=" << value.quantifier() << ", "
               << "mappings=" << util::print_support { value.mappings() } << ")";
}

bool union_::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<union_>(other);
}

std::ostream& union_::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
