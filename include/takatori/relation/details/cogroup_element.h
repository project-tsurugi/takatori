#pragma once

#include <iostream>
#include <initializer_list>
#include <vector>

#include "takatori/descriptor/relation.h"

#include "mapping_element.h"

#include "takatori/util/object_creator.h"

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
     * @param mandatory whether or not this group is mandatory
     */
    explicit cogroup_element(
            descriptor::relation source,
            std::vector<column, util::object_allocator<column>> columns,
            bool mandatory = false) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the source relation, must be refer the source exchange
     * @param columns the column mappings between the upstream exchange and output relation
     * @param mandatory whether or not this group is mandatory
     */
    cogroup_element(
            descriptor::relation source,
            std::initializer_list<column> columns,
            bool mandatory = false);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit cogroup_element(cogroup_element const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit cogroup_element(cogroup_element&& other, util::object_creator creator);

    /**
     * @brief returns the source relation, must be refer the source exchange.
     * @return the source relation
     */
    descriptor::relation const& source() const noexcept;

    /**
     * @brief sets the source exchange.
     * @param source the source exchange
     * @return this
     */
    cogroup_element& source(descriptor::relation source) noexcept;

    /**
     * @brief returns the column mappings, from the upstream exchange to output relation.
     * @return the column mappings
     */
    std::vector<column, util::object_allocator<column>>& columns() noexcept;

    /// @copydoc columns()
    std::vector<column, util::object_allocator<column>> const& columns() const noexcept;

    /**
     * @brief returns whether or not this group is mandatory.
     * @details this option effects like as join kind (e.g. inner, full_outer).
     * @return true this group is mandatory - if there are no rows for some keys in this source, the co-group operation does not emit rows for such the keys
     * @return false this group is optional - the co-group operation emits rows even if there are no rows for some keys in this source
     */
    bool mandatory() const noexcept;

    /**
     * @brief sets whether or not this group is mandatory.
     * @param mandatory whether or not it is mandatory
     * @return this
     */
    cogroup_element& mandatory(bool mandatory) noexcept;

private:
    descriptor::relation source_;
    std::vector<column, util::object_allocator<column>> columns_;
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
