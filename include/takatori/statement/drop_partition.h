#pragma once

#include <ostream>
#include <vector>

#include <takatori/descriptor/relation.h>
#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"
#include "details/partition_criteria.h"

namespace takatori::statement {

/**
 * @brief drop partition statement.
 * @details This represents `ALTER TABLE ... DROP PARTITION (...), PARTITION (...), ...` in SQL,
 *   which drops partitions from a partitioned table.
 *
 *   - index() is the index that defines the partitioning, that is, the primary index of the target table.
 *     Its partition key columns are referred from the elements of target().
 *   - target() corresponds to the list of `PARTITION (...)` clauses.
 *     This statement drops the partitions that satisfy **any of** the partition criteria.
 *
 *   The target partitions are specified by the partition key values instead of the partition names,
 *   because the partitions are created dynamically.
 *
 *   For example, `ALTER TABLE t DROP PARTITION (k = 1), PARTITION (k >= 10, k < 20)` has two partition criteria:
 *   the first one has an element for `k` with a predicate `= 1`,
 *   and the second one has an element for `k` with two predicates `>= 10` and `< 20`.
 * @see details::partition_criteria
 * @see details::partition_key_element
 */
class drop_partition final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag { statement_kind::drop_partition };

    /**
     * @brief creates a new instance.
     * @param index the target index that the partitions will be dropped from
     * @param target the partition criteria to drop
     */
    explicit drop_partition(
            descriptor::relation index,
            std::vector<details::partition_criteria> target) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit drop_partition(util::clone_tag_t, drop_partition const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit drop_partition(util::clone_tag_t, drop_partition&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] drop_partition* clone() const& override;
    [[nodiscard]] drop_partition* clone() && override;

    /**
     * @brief returns the target index that the partitions will be dropped from.
     * @return the target index
     */
    [[nodiscard]] descriptor::relation& index() noexcept;

    /// @copydoc index()
    [[nodiscard]] descriptor::relation const& index() const noexcept;

    /**
     * @brief returns the partition criteria to drop.
     * @details This operation will target the partitions that satisfy **any of the criteria** in the list.
     * @return the partition criteria
     */
    [[nodiscard]] std::vector<details::partition_criteria>& target() noexcept;

    /// @copydoc target()
    [[nodiscard]] std::vector<details::partition_criteria> const& target() const noexcept;

    /**
     * @brief returns whether the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(drop_partition const& a, drop_partition const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(drop_partition const& a, drop_partition const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, drop_partition const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::relation index_;
    std::vector<details::partition_criteria> target_;
};

} // namespace takatori::statement
