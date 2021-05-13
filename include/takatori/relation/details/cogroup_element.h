#pragma once

#include <initializer_list>
#include <memory>
#include <ostream>
#include <vector>

#include <takatori/descriptor/relation.h>
#include <takatori/util/clone_tag.h>

#include "mapping_element.h"

namespace takatori::relation::details {

/**
 * @brief represents individual sources of co-group.
 */
class cogroup_element {
public:
    /// @brief column mapping between the upstream exchange and the output relation.
    using column = mapping_element;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the source exchange
     * @param columns the column mappings between the upstream exchange and output relation
     * @param mandatory whether or not this group is mandatory:
     *      The mandatory group never become empty, that is, the co-group row will not be generated if the mandatory
     *      group is empty. Or if it is not a mandatory group, co-group row will be always generated even if some
     *      groups are empty.
     */
    explicit cogroup_element(
            descriptor::relation source,
            std::vector<column> columns,
            bool mandatory = false) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the source exchange
     * @param columns the column mappings between the upstream exchange and output relation
     * @param mandatory whether or not this group is mandatory:
     *      The mandatory group never become empty, that is, the co-group row will not be generated if the mandatory
     *      group is empty. Or if it is not a mandatory group, co-group row will be always generated even if some
     *      groups are empty.
     */
    cogroup_element( // NOLINT
            descriptor::relation source,
            std::initializer_list<column> columns = {},
            bool mandatory = false);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit cogroup_element(util::clone_tag_t, cogroup_element const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit cogroup_element(util::clone_tag_t, cogroup_element&& other);

    /**
     * @brief returns the source relation, must be refer the source exchange.
     * @return the source relation
     */
    [[nodiscard]] descriptor::relation& source() noexcept;

    /// @copydoc source()
    [[nodiscard]] descriptor::relation const& source() const noexcept;

    /**
     * @brief returns the column mappings, from the upstream exchange to output relation.
     * @return the column mappings
     */
    [[nodiscard]] std::vector<column>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<column> const& columns() const noexcept;

    /**
     * @brief returns whether or not this group is mandatory.
     * @details this option effects like as join kind (e.g. inner, full_outer).
     * @return true this group is mandatory - if there are no rows for some keys in this source, the co-group operation does not emit rows for such the keys
     * @return false this group is optional - the co-group operation emits rows even if there are no rows for some keys in this source
     */
    [[nodiscard]] bool mandatory() const noexcept;

    /**
     * @brief sets whether or not this group is mandatory.
     * @param mandatory whether or not it is mandatory
     * @return this
     */
    cogroup_element& mandatory(bool mandatory) noexcept;

private:
    descriptor::relation source_;
    std::vector<column> columns_;
    bool mandatory_;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(cogroup_element const& a, cogroup_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(cogroup_element const& a, cogroup_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, cogroup_element const& value);

} // namespace takatori::relation::details
