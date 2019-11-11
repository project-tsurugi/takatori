#pragma once

#include "data_type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief 4-bytes floating point number type.
 */
using float4 = simple_type<data_type_kind::float4>;

/**
 * @brief 8-bytes floating point number type.
 */
using float8 = simple_type<data_type_kind::float8>;

/**
 * @brief data_type_kind_type for float4.
 */
template<> struct data_type_kind_type<float4::tag> : util::meta_type<float4> {};

/**
 * @brief data_type_kind_type for float8.
 */
template<> struct data_type_kind_type<float8::tag> : util::meta_type<float8> {};

} // namespace takatori::type
