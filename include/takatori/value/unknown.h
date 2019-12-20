#pragma once

#include "value_kind.h"
#include "simple_value.h"
#include "unknown_kind.h"

#include <takatori/util/meta_type.h>

namespace takatori::value {

/**
 * @brief traits for value_kind::unknown.
 */
template<>
struct simple_value_traits<value_kind::unknown> {
    /// @brief the entity type.
    using entity_type = unknown_kind;
    /// @brief the view type.
    using view_type = entity_type;
};

/**
 * @brief unknown value.
 */
using unknown = simple_value<value_kind::unknown>;

/**
 * @brief type_of for unknown.
 */
template<> struct type_of<unknown::tag> : util::meta_type<unknown> {};

} // namespace takatori::type
