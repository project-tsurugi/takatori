#pragma once

#include "type_kind.h"
#include "simple_type.h"

#include <takatori/util/meta_type.h>

namespace takatori::type {

/**
 * @brief boolean type.
 */
using boolean = simple_type<type_kind::boolean>;

/**
 * @brief type_of for boolean.
 */
template<> struct type_of<boolean::tag> : util::meta_type<boolean> {};

} // namespace takatori::type
