#include <takatori/scalar/function_call.h>

#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_element_vector_forward.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

function_call::function_call(
        descriptor::function function,
        std::initializer_list<util::rvalue_reference_wrapper<expression>> arguments)
    : function_call(
        std::move(function),
        { arguments.begin(), arguments.end() })
{}

function_call::function_call(function_call const& other, util::object_creator creator)
    : function_call(
        other.function_,
        tree::forward(creator, other.arguments_))
{}

function_call::function_call(function_call&& other, util::object_creator creator)
    : function_call(
        std::move(other.function_),
        tree::forward(creator, std::move(other.arguments_)))
{}

expression_kind function_call::kind() const noexcept {
    return tag;
}

function_call* function_call::clone(util::object_creator creator) const& {
    return creator.create_object<function_call>(*this, creator);
}

function_call* function_call::clone(util::object_creator creator) && {
    return creator.create_object<function_call>(std::move(*this), creator);
}

descriptor::function const& function_call::function() const noexcept {
    return function_;
}

function_call& function_call::function(descriptor::function function) noexcept {
    function_ = std::move(function);
    return *this;
}

tree::tree_element_vector<expression>& function_call::arguments() noexcept {
    return arguments_;
}

tree::tree_element_vector<expression> const& function_call::arguments() const noexcept {
    return arguments_;
}

bool operator==(function_call const& a, function_call const& b) noexcept {
    return a.function() == b.function()
        && a.arguments() == b.arguments();
}

bool operator!=(function_call const& a, function_call const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, function_call const& value) {
    return out << value.kind() << "("
               << "function=" << value.function() << ", "
               << "arguments=" << value.arguments() <<  ")";
}

bool function_call::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<function_call>(other);
}

std::ostream& function_call::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
