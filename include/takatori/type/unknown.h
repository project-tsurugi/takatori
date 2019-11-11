#pragma once

#include "data_type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief unknown type.
 */
using unknown = simple_type<data_type_kind::unknown>;

/**
 * @brief data_type_kind_type for unknown.
 */
template<> struct data_type_kind_type<unknown::tag> : util::meta_type<unknown> {};

} // namespace takatori::type
