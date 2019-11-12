#pragma once

#include "value_kind.h"
#include "simple_value.h"

#include "takatori/util/meta_type.h"

namespace takatori::value {

/**
 * @brief traits for value_kind::float4.
 */
template<> struct simple_value_traits<value_kind::float4> : util::meta_type<float> {};

/**
 * @brief traits for value_kind::float8.
 */
template<> struct simple_value_traits<value_kind::float8> : util::meta_type<double> {};

/**
 * @brief 4-octets floating point number.
 */
using float4 = simple_value<value_kind::float4>;

/**
 * @brief 8-octets floating point number.
 */
using float8 = simple_value<value_kind::float8>;

/**
 * @brief type_of for float4.
 */
template<> struct type_of<float4::tag> : util::meta_type<float4> {};

/**
 * @brief type_of for float8.
 */
template<> struct type_of<float8::tag> : util::meta_type<float8> {};

} // namespace takatori::type
