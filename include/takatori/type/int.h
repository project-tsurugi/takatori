#pragma once

#include "data_type_kind.h"
#include "simple_type.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief 1-byte integral number type.
 */
using int1 = simple_type<data_type_kind::int1>;

/**
 * @brief 2-bytes integral number type.
 */
using int2 = simple_type<data_type_kind::int2>;

/**
 * @brief 4-bytes integral number type.
 */
using int4 = simple_type<data_type_kind::int4>;

/**
 * @brief 8-bytes integral number type.
 */
using int8 = simple_type<data_type_kind::int8>;

/**
 * @brief data_type_kind_type for int1.
 */
template<> struct data_type_kind_type<int1::tag> : util::meta_type<int1> {};

/**
 * @brief data_type_kind_type for int2.
 */
template<> struct data_type_kind_type<int2::tag> : util::meta_type<int2> {};

/**
 * @brief data_type_kind_type for int4.
 */
template<> struct data_type_kind_type<int4::tag> : util::meta_type<int4> {};

/**
 * @brief data_type_kind_type for int8.
 */
template<> struct data_type_kind_type<int8::tag> : util::meta_type<int8> {};

} // namespace takatori::type
