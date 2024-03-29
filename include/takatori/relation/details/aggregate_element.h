#pragma once

#include <initializer_list>
#include <memory>
#include <ostream>
#include <vector>

#include <takatori/util/clone_tag.h>
#include <takatori/descriptor/aggregate_function.h>
#include <takatori/descriptor/variable.h>


namespace takatori::relation::details {

/**
 * @brief represents element of aggregate operator.
 */
class aggregate_element {
public:
    /// @brief the aggregate function type.
    using function_type = descriptor::aggregate_function;

    /// @brief the argument column type.
    using argument_type = descriptor::variable;

    /// @brief the destination column type.
    using destination_type = descriptor::variable;

    /**
     * @brief creates a new instance.
     * @param function the aggregate function
     * @param arguments the target columns as the aggregate function arguments
     * @param destination the destination column
     */
    explicit aggregate_element(
            function_type function,
            std::vector<argument_type> arguments,
            destination_type destination) noexcept;

    /**
     * @brief creates a new instance.
     * @param function the aggregate function
     * @param argument the column to be aggregated
     * @param destination the destination column
     */
    aggregate_element(function_type function, argument_type argument, destination_type destination);

    /**
     * @brief creates a new instance.
     * @param function the aggregate function
     * @param arguments the target columns as the aggregate function arguments
     * @param destination the destination column
     */
    aggregate_element(
            function_type function,
            std::initializer_list<argument_type> arguments,
            destination_type destination);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit aggregate_element(util::clone_tag_t, aggregate_element const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit aggregate_element(util::clone_tag_t, aggregate_element&& other);

    /**
     * @brief returns the aggregate function.
     * @return the aggregate function
     */
    [[nodiscard]] function_type& function() noexcept;

    /// @copydoc function()
    [[nodiscard]] function_type const& function() const noexcept;

    /**
     * @brief returns the argument columns of the aggregate function.
     * @return the aggregate targets
     */
    [[nodiscard]] std::vector<argument_type>& arguments() noexcept;

    /// @copydoc arguments()
    [[nodiscard]] std::vector<argument_type> const& arguments() const noexcept;

    /**
     * @brief returns the destination column.
     * @return the destination column
     */
    [[nodiscard]] destination_type& destination() noexcept;

    /// @copydoc destination()
    [[nodiscard]] destination_type const& destination() const noexcept;

private:
    function_type function_;
    std::vector<argument_type> arguments_;
    destination_type destination_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(aggregate_element const& a, aggregate_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(aggregate_element const& a, aggregate_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, aggregate_element const& value);

} // namespace takatori::relation::details
