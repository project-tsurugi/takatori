#pragma once

#include "type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief 4-octets binary floating point number type.
 */
using float4 = simple_type<type_kind::float4>;

/**
 * @brief 8-octets binary floating point number type.
 */
using float8 = simple_type<type_kind::float8>;

/**
 * @brief type_of for float4.
 */
template<> struct type_of<float4::tag> : util::meta_type<float4> {};

/**
 * @brief type_of for float8.
 */
template<> struct type_of<float8::tag> : util::meta_type<float8> {};

} // namespace takatori::type
