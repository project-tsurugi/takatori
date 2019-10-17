#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include "util/enum_tag.h"

namespace model::scalar {

/**
 * @brief represents kind of comparison operators.
 */
enum class comparison_operator {
    /// @brief equal to
    equal,
    /// @brief not equal to
    not_equal,
    /// @brief less than
    less,
    /// @brief less than or equal to
    less_equal,
    /// @brief greater than
    greater,
    /// @brief greater than or equal to
    greater_equal,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(comparison_operator value) noexcept {
    using namespace std::string_view_literals;
    using kind = comparison_operator;
    switch (value) {
        case kind::equal: return "equal"sv;
        case kind::not_equal: return "not_equal"sv;
        case kind::less: return "less"sv;
        case kind::less_equal: return "less_equal"sv;
        case kind::greater: return "greater"sv;
        case kind::greater_equal: return "greater_equal"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, comparison_operator value) noexcept {
    return out << to_string_view(value);
}

/**
 * @brief a tag of comparison_operator.
 * @tparam Kind the comparison_operator kind
 */
template<comparison_operator Kind>
using comparison_operator_tag_t = util::enum_tag_t<comparison_operator, Kind>;

/**
 * @brief a tag object of comparison_operator.
 * @tparam Kind the comparison_operator kind
 */
template<comparison_operator Kind>
inline constexpr comparison_operator_tag_t<Kind> comparison_operator_tag {};

/// @private
namespace impl {

/// @private
template<comparison_operator Kind, class Callback, class... Args>
inline std::enable_if_t<
        std::is_invocable_v<Callback, comparison_operator_tag_t<Kind>, Args...>,
        std::invoke_result_t<Callback, comparison_operator_tag_t<Kind>, Args...>>
callback_comparison_operator(Callback&& callback, Args&&... args) {
    return std::forward<Callback>(callback)(comparison_operator_tag<Kind>, std::forward<Args>(args)...);
}

} // namespace impl

/**
 * @brief invoke callback function for individual comparison operator kinds.
 * If the operator_kind is K, this may invoke Callback::operator()(comparison_operator_tag_t<K>, Args...).
 * You must declare all callback functions for individual comparison operators,
 * or declare Callback::operator()(comparison_operator, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param operator_kind the operator kind
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, comparison_operator operator_kind, Args&&... args) {
    using kind = comparison_operator;
    switch (operator_kind) {
        case kind::equal: return impl::callback_comparison_operator<kind::equal>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::not_equal: return impl::callback_comparison_operator<kind::not_equal>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::less: return impl::callback_comparison_operator<kind::less>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::less_equal: return impl::callback_comparison_operator<kind::less_equal>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::greater: return impl::callback_comparison_operator<kind::greater>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::greater_equal: return impl::callback_comparison_operator<kind::greater_equal>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace model::scalar
