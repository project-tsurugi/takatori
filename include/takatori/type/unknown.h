#pragma once

#include "type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief unknown type.
 */
using unknown = simple_type<type_kind::unknown>;

/**
 * @brief type_of for unknown.
 */
template<> struct type_of<unknown::tag> : util::meta_type<unknown> {};

} // namespace takatori::type
