#pragma once

#include "data_type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief date type.
 */
using date = simple_type<data_type_kind::date>;

/**
 * @brief data_type_kind_type for date.
 */
template<> struct data_type_kind_type<date::tag> : util::meta_type<date> {};

} // namespace takatori::type
