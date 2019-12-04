#pragma once

#include "type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief error type.
 */
using error = simple_type<type_kind::error>;

/**
 * @brief type_of for error.
 */
template<> struct type_of<error::tag> : util::meta_type<error> {};

} // namespace takatori::type
