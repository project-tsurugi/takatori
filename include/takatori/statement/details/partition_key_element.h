#pragma once

#include <initializer_list>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <takatori/descriptor/variable.h>
#include <takatori/scalar/comparison_operator.h>
#include <takatori/value/data.h>

#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::statement::details {

/**
 * @brief the partition key element of the partition criteria.
 * @details This represents comparisons between a partition key column and literal values.
 */
class partition_key_element {
public:
    /// @brief the column name type.
    using name_type = std::string;

    /// @brief the predicate type that contains the comparison operator and the literal value in the right operand.
    using predicate_type = std::pair<scalar::comparison_operator, std::shared_ptr<value::data const>>;

    /**
     * @brief creates a new instance.
     * @param column the target partition key column
     * @param column_name the target partition key column name
     * @param predicates the predicates for the target column
     * @attention this does not validate the predicates, see predicates() for the conditions
     */
    partition_key_element(
            descriptor::variable column,
            name_type column_name,
            std::vector<predicate_type> predicates) noexcept;

    /**
     * @brief creates a new instance.
     * @param column the target partition key column
     * @param column_name the target partition key column name
     * @param predicates the predicates for the target column
     * @attention this may take copies of given literal values
     * @attention this does not validate the predicates, see predicates() for the conditions
     */
    partition_key_element(
            descriptor::variable column,
            name_type column_name,
            std::initializer_list<std::pair<
                    scalar::comparison_operator,
                    util::rvalue_reference_wrapper<value::data>>> predicates);

    /**
     * @brief returns the target partition key column of the predicate.
     * @return the target partition key column
     * @attention If the target is a virtual column, this even provides the source column of the virtual column.
     */
    [[nodiscard]] descriptor::variable& column() noexcept;

    /// @copydoc column()
    [[nodiscard]] descriptor::variable const& column() const noexcept;

    /**
     * @brief returns the target partition key column name of the predicate.
     * @details This name may be different from the column(), if this element targets the virtual column.
     * @return the name of the target key column
     */
    [[nodiscard]] name_type& column_name() noexcept;

    /// @copydoc column_name()
    [[nodiscard]] name_type const& column_name() const noexcept;

    /**
     * @brief returns the predicates of the partition key element.
     * @details Each predicate represents a comparison between the partition key column in the left operand,
     *   and a literal value in the right operand.
     *   This partition key element satisfies **all the predicates** in the list.
     *
     *   The predicates must be one of the following forms:
     *
     *   - a single comparison with scalar::comparison_operator::equal
     *   - a single comparison with an inequality operator
     *     (scalar::comparison_operator::less, less_equal, greater, or greater_equal)
     *   - a lower bound comparison (greater or greater_equal) followed by an upper bound comparison
     *     (less or less_equal)
     *
     *   Consumers of this element can rely on the above forms and their order.
     * @return the predicates
     * @attention this object does not validate the above conditions, the producer must satisfy them
     */
    [[nodiscard]] std::vector<predicate_type>& predicates() noexcept;

    /// @copydoc predicates()
    [[nodiscard]] std::vector<predicate_type> const& predicates() const noexcept;

private:
    descriptor::variable column_;
    name_type column_name_;
    std::vector<predicate_type> predicates_;
};

/**
 * @brief returns whether the two elements are equivalent.
 * @details This compares the literal values of the predicates by their contents.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(partition_key_element const& a, partition_key_element const& b) noexcept;

/**
 * @brief returns whether the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(partition_key_element const& a, partition_key_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, partition_key_element const& value);

} // namespace takatori::statement::details
