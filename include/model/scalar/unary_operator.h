#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include "util/enum_tag.h"

namespace model::scalar {

/**
 * @brief represents kind of unary operators.
 */
enum class unary_operator {
    /// @brief unary plus.
    plus,
    /// @brief sign inversion
    sign_inversion,
    /// @brief sequence length
    length,
    /// @brief conditional negation
    conditional_not,
    /// @brief nullity test
    is_null,
    /// @brief truth test
    is_true,
    /// @brief false test
    is_false,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(unary_operator value) noexcept {
    using namespace std::string_view_literals;
    using kind = unary_operator;
    switch (value) {
        case kind::plus: return "plus"sv;
        case kind::sign_inversion: return "sign_inversion"sv;
        case kind::length: return "length"sv;
        case kind::conditional_not: return "conditional_not"sv;
        case kind::is_null: return "is_null"sv;
        case kind::is_true: return "is_true"sv;
        case kind::is_false: return "is_false"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, unary_operator value) noexcept {
    return out << to_string_view(value);
}

/**
 * @brief a tag of unary_operator.
 * @tparam Kind the unary_operator kind
 */
template<unary_operator Kind>
using unary_operator_tag_t = util::enum_tag_t<unary_operator, Kind>;

/**
 * @brief a tag object of unary_operator.
 * @tparam Kind the unary_operator kind
 */
template<unary_operator Kind>
inline constexpr unary_operator_tag_t<Kind> unary_operator_tag {};

/// @private
namespace impl {

/// @private
template<unary_operator Kind, class Callback, class... Args>
inline std::enable_if_t<
        std::is_invocable_v<Callback, unary_operator_tag_t<Kind>, Args...>,
        std::invoke_result_t<Callback, unary_operator_tag_t<Kind>, Args...>>
callback_unary_operator(Callback&& callback, Args&&... args) {
    return std::forward<Callback>(callback)(unary_operator_tag<Kind>, std::forward<Args>(args)...);
}

} // namespace impl

/**
 * @brief invoke callback function for individual unary operator kinds.
 * If the operator_kind is K, this may invoke Callback::operator()(unary_operator_tag_t<K>, Args...).
 * You must declare all callback functions for individual unary operators,
 * or declare Callback::operator()(unary_operator, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param operator_kind the operator kind
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, unary_operator operator_kind, Args&&... args) {
    using kind = unary_operator;
    switch (operator_kind) {
        case kind::plus: return impl::callback_unary_operator<kind::plus>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::sign_inversion: return impl::callback_unary_operator<kind::sign_inversion>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::length: return impl::callback_unary_operator<kind::length>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::conditional_not: return impl::callback_unary_operator<kind::conditional_not>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::is_null: return impl::callback_unary_operator<kind::is_null>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::is_true: return impl::callback_unary_operator<kind::is_true>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::is_false: return impl::callback_unary_operator<kind::is_false>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace model::scalar
