#pragma once

#include "data_type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief boolean type.
 */
using boolean = simple_type<data_type_kind::boolean>;

/**
 * @brief data_type_kind_type for boolean.
 */
template<> struct data_type_kind_type<boolean::tag> : util::meta_type<boolean> {};

} // namespace takatori::type
