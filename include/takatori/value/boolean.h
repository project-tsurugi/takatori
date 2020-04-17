#pragma once

#include "value_kind.h"
#include "simple_value.h"

#include <takatori/util/meta_type.h>

namespace takatori::value {

/**
 * @brief traits for value_kind::boolean.
 */
template<>
struct simple_value_traits<value_kind::boolean> {
    /// @brief the entity type.
    using entity_type = bool;
    /// @brief the view type.
    using view_type = entity_type;
};

/**
 * @brief boolean value as polymorphic value model.
 */
using boolean = simple_value<value_kind::boolean>;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, boolean const& value);

/**
 * @brief type_of for boolean.
 */
template<> struct type_of<boolean::tag> : util::meta_type<boolean> {};

} // namespace takatori::type
