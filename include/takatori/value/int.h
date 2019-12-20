#pragma once

#include <cstdint>

#include "value_kind.h"
#include "simple_value.h"

#include <takatori/util/meta_type.h>

namespace takatori::value {

/**
 * @brief traits for value_kind::int4.
 */
template<>
struct simple_value_traits<value_kind::int4> {
    /// @brief the entity type.
    using entity_type = std::int32_t;
    /// @brief the view type.
    using view_type = entity_type;
};

/**
 * @brief traits for value_kind::int8.
 */
template<>
struct simple_value_traits<value_kind::int8> {
    /// @brief the entity type.
    using entity_type = std::int64_t;
    /// @brief the view type.
    using view_type = entity_type;
};

/**
 * @brief 4-octets integral number as polymorphic value model.
 */
using int4 = simple_value<value_kind::int4>;

/**
 * @brief 8-octets integral number as polymorphic value model.
 */
using int8 = simple_value<value_kind::int8>;

/**
 * @brief type_of for int4.
 */
template<> struct type_of<int4::tag> : util::meta_type<int4> {};

/**
 * @brief type_of for int8.
 */
template<> struct type_of<int8::tag> : util::meta_type<int8> {};

} // namespace takatori::type
