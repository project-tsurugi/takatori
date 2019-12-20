#include <takatori/scalar/coalesce.h>

#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_element_vector_forward.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

coalesce::coalesce(std::initializer_list<util::rvalue_reference_wrapper<expression>> alternatives)
    : coalesce({ alternatives.begin(), alternatives.end() })
{}

coalesce::coalesce(coalesce const& other, util::object_creator creator)
    : coalesce(tree::forward(creator, other.alternatives_))
{}

coalesce::coalesce(coalesce&& other, util::object_creator creator)
    : coalesce(tree::forward(creator, std::move(other.alternatives_)))
{}

expression_kind coalesce::kind() const noexcept {
    return tag;
}

coalesce* coalesce::clone(util::object_creator creator) const& {
    return creator.create_object<coalesce>(*this, creator);
}

coalesce* coalesce::clone(util::object_creator creator) && {
    return creator.create_object<coalesce>(std::move(*this), creator);
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
