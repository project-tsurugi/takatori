#pragma once

#include "type_kind.h"
#include "simple_type.h"

#include <takatori/util/meta_type.h>

namespace takatori::type {

// TODO: REVIEW - type name (datetime interval?)

/**
 * @brief time_interval type.
 */
using time_interval = simple_type<type_kind::time_interval>;

/**
 * @brief type_of for time_interval.
 */
template<> struct type_of<time_interval::tag> : util::meta_type<time_interval> {};

} // namespace takatori::type
