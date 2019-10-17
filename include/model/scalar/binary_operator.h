#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include "util/enum_tag.h"

namespace model::scalar {

/**
 * @brief represents kind of binary operators.
 */
enum class binary_operator {
    /// @brief sum
    add,
    /// @brief difference
    subtract,
    /// @brief product
    multiply,
    /// @brief quotient
    divide,
    /// @brief remainder
    remainder,
    /// @brief concatenate sequences
    concat,
    /// @brief conjunction
    conditional_and,
    /// @brief disjunction
    conditional_or,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(binary_operator value) noexcept {
    using namespace std::string_view_literals;
    using kind = binary_operator;
    switch (value) {
        case kind::add: return "add"sv;
        case kind::subtract: return "subtract"sv;
        case kind::multiply: return "multiply"sv;
        case kind::divide: return "divide"sv;
        case kind::remainder: return "remainder"sv;
        case kind::concat: return "concat"sv;
        case kind::conditional_and: return "conditional_and"sv;
        case kind::conditional_or: return "conditional_or"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, binary_operator value) noexcept {
    return out << to_string_view(value);
}

/**
 * @brief a tag of binary_operator.
 * @tparam Kind the binary_operator kind
 */
template<binary_operator Kind>
using binary_operator_tag_t = util::enum_tag_t<binary_operator, Kind>;

/**
 * @brief a tag object of binary_operator.
 * @tparam Kind the binary_operator kind
 */
template<binary_operator Kind>
inline constexpr binary_operator_tag_t<Kind> binary_operator_tag {};

/// @private
namespace impl {

/// @private
template<binary_operator Kind, class Callback, class... Args>
inline std::enable_if_t<
        std::is_invocable_v<Callback, binary_operator_tag_t<Kind>, Args...>,
        std::invoke_result_t<Callback, binary_operator_tag_t<Kind>, Args...>>
callback_binary_operator(Callback&& callback, Args&&... args) {
    return std::forward<Callback>(callback)(binary_operator_tag<Kind>, std::forward<Args>(args)...);
}

} // namespace impl

/**
 * @brief invoke callback function for individual binary operator kinds.
 * If the operator_kind is K, this may invoke Callback::operator()(binary_operator_tag_t<K>, Args...).
 * You must declare all callback functions for individual binary operators,
 * or declare Callback::operator()(binary_operator, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param operator_kind the operator kind
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, binary_operator operator_kind, Args&&... args) {
    using kind = binary_operator;
    switch (operator_kind) {
        case kind::add: return impl::callback_binary_operator<kind::add>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::subtract: return impl::callback_binary_operator<kind::subtract>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::multiply: return impl::callback_binary_operator<kind::multiply>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::divide: return impl::callback_binary_operator<kind::divide>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::remainder: return impl::callback_binary_operator<kind::remainder>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::concat: return impl::callback_binary_operator<kind::concat>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::conditional_and: return impl::callback_binary_operator<kind::conditional_and>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::conditional_or: return impl::callback_binary_operator<kind::conditional_or>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace model::scalar
