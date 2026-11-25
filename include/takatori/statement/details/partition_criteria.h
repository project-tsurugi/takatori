#pragma once

#include <ostream>
#include <vector>

#include "partition_key_element.h"

namespace takatori::statement::details {

/**
 * @brief the partition criteria to specify the target partitions.
 * @details This corresponds to a `PARTITION (...)` clause in SQL.
 */
class partition_criteria {
public:
    /**
     * @brief creates a new instance.
     * @param elements the partition key elements
     * @attention this does not validate the elements, see elements() for the conditions
     */
    explicit partition_criteria(std::vector<partition_key_element> elements) noexcept;

    /**
     * @brief returns the partition key elements of the criteria for each partition key column.
     * @details This criteria satisfies **all the predicates** of the partition key elements.
     *
     *   The elements must satisfy the following conditions,
     *   where "partition key columns" are the ones of the index in the owner statement
     *   (e.g. drop_partition::index()):
     *
     *   - the elements are ordered by the partition key columns
     *   - each partition key column appears at most once
     *   - the elements target a leading part of the partition key columns,
     *     that is, the first N partition key columns for some N
     *   - the elements except the last one only have a single
     *     scalar::comparison_operator::equal predicate
     *
     *   Consumers of this criteria can rely on the above conditions.
     * @return the partition key elements
     * @attention this object does not validate the above conditions, the producer must satisfy them
     * @see partition_key_element::predicates()
     */
    [[nodiscard]] std::vector<partition_key_element>& elements() noexcept;

    /// @copydoc elements()
    [[nodiscard]] std::vector<partition_key_element> const& elements() const noexcept;

private:
    std::vector<partition_key_element> elements_;
};

/**
 * @brief returns whether the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(partition_criteria const& a, partition_criteria const& b) noexcept;

/**
 * @brief returns whether the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(partition_criteria const& a, partition_criteria const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, partition_criteria const& value);

} // namespace takatori::statement::details
