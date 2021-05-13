#include <takatori/scalar/coalesce.h>

#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_element_vector_forward.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

coalesce::coalesce(util::reference_vector<expression> alternatives) noexcept
    : alternatives_(*this, std::move(alternatives))
{}

coalesce::coalesce(std::initializer_list<util::rvalue_reference_wrapper<expression>> alternatives)
    : coalesce({ alternatives.begin(), alternatives.end() })
{}

coalesce::coalesce(util::clone_tag_t, coalesce const& other)
    : coalesce(tree::forward(other.alternatives_))
{}

coalesce::coalesce(util::clone_tag_t, coalesce&& other)
    : coalesce(tree::forward(std::move(other.alternatives_)))
{}

expression_kind coalesce::kind() const noexcept {
    return tag;
}

coalesce* coalesce::clone() const& {
    return new coalesce(util::clone_tag, *this); // NOLINT
}

coalesce* coalesce::clone() && {
    return new coalesce(util::clone_tag, std::move(*this)); // NOLINT;
}

tree::tree_element_vector<expression>& coalesce::alternatives() noexcept {
    return alternatives_;
}

tree::tree_element_vector<expression> const& coalesce::alternatives() const noexcept {
    return alternatives_;
}

bool operator==(coalesce const& a, coalesce const& b) noexcept {
    return a.alternatives() == b.alternatives();
}

bool operator!=(coalesce const& a, coalesce const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, coalesce const& value) {
    return out << value.kind() << "(" << value.alternatives() <<  ")";
}

bool coalesce::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<coalesce>(other);
}

std::ostream& coalesce::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
