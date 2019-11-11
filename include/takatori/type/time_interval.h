#pragma once

#include "data_type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief time_interval type.
 */
using time_interval = simple_type<data_type_kind::time_interval>;

/**
 * @brief data_type_kind_type for time_interval.
 */
template<> struct data_type_kind_type<time_interval::tag> : util::meta_type<time_interval> {};

} // namespace takatori::type
