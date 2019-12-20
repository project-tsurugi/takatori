#include <takatori/relation/details/aggregate_element.h>

#include <utility>

#include <takatori/util/hash.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::details {

aggregate_element::aggregate_element(
        aggregate_element::function_type function,
        std::vector<argument_type, util::object_allocator<argument_type>> arguments,
        aggregate_element::destination_type destination) noexcept
    : function_(std::move(function))
    , arguments_(std::move(arguments))
    , destination_(std::move(destination))
{}

aggregate_element::aggregate_element(
        function_type function,
        argument_type argument,
        destination_type destination)
    : aggregate_element(
            std::move(function),
            decltype(arguments_) {},
            std::move(destination))
{
    arguments_.reserve(1);
    arguments_.emplace_back(std::move(argument));
}

aggregate_element::aggregate_element(
        aggregate_element::function_type function,
        std::initializer_list<argument_type> arguments,
        aggregate_element::destination_type destination)
    : aggregate_element(
            std::move(function),
            { arguments.begin(), arguments.end() },
            std::move(destination))
{}

aggregate_element::aggregate_element(aggregate_element const& other, util::object_creator creator)
    : aggregate_element(
            other.function_,
            { other.arguments_, creator.allocator<argument_type>() },
            other.destination_)
{}

aggregate_element::aggregate_element(aggregate_element&& other, util::object_creator creator)
    : aggregate_element(
            std::move(other.function_),
            { std::move(other.arguments_), creator.allocator<argument_type>() },
            std::move(other.destination_))
{}

aggregate_element::function_type const& aggregate_element::function() const noexcept {
    return function_;
}

aggregate_element& aggregate_element::function(function_type function) noexcept {
    function_ = std::move(function);
    return *this;
}

aggregate_element::destination_type const& aggregate_element::destination() const noexcept {
    return destination_;
}

aggregate_element& aggregate_element::destination(destination_type destination) noexcept {
    destination_ = std::move(destination);
    return *this;
}

std::vector<aggregate_element::argument_type, util::object_allocator<aggregate_element::argument_type>>& aggregate_element::arguments() noexcept {
    return arguments_;
}

std::vector<aggregate_element::argument_type, util::object_allocator<aggregate_element::argument_type>> const& aggregate_element::arguments() const noexcept {
    return arguments_;
}

bool operator==(aggregate_element const& a, aggregate_element const& b) noexcept {
    return a.function() == b.function()
        && a.arguments() == b.arguments()
        && a.destination() == b.destination();
}

bool operator!=(aggregate_element const& a, aggregate_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, aggregate_element const& value) {
    return out << "aggregate("
               << value.function()
               << util::print_support { value.arguments() } << " => "
               << value.destination() << ")";
}

} // namespace takatori::relation::details
