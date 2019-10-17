#pragma once

#include <type_traits>

#include "model/scalar/expression.h"
#include "model/scalar/expression_kind.h"

namespace model::scalar {

/**
 * @brief test whether or not the type is a kind of expression.
 * @tparam T the model type
 */
template<class T> struct is_expression : std::is_base_of<expression, T> {};

/// @copydoc is_expression
template<class T> constexpr inline bool is_expression_v = is_expression<T>::value;

/**
 * @brief traits of expressions.
 * @tparam T the model type
 */
template<class T>
struct expression_traits {

    static_assert(is_expression_v<T>);

    /// @brief the model type.
    using type = T;

    /// @brief the model kind.
    constexpr static inline expression_kind kind = type::tag;
};

} // namespace model::scalar
