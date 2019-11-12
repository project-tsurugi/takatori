#pragma once

#include "value_kind.h"
#include "simple_value.h"

#include "takatori/util/meta_type.h"

namespace takatori::value {

/**
 * @brief traits for value_kind::boolean.
 */
template<> struct simple_value_traits<value_kind::boolean> : util::meta_type<bool> {};

/**
 * @brief boolean value.
 */
using boolean = simple_value<value_kind::boolean>;

/**
 * @brief type_of for boolean.
 */
template<> struct type_of<boolean::tag> : util::meta_type<boolean> {};

} // namespace takatori::type
