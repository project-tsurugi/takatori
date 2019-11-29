#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include "takatori/util/enum_tag.h"

namespace takatori::scalar {

/**
 * @brief represents kind of match operators.
 */
enum class match_operator {
    /// @brief "LIKE" collation.
    like,
    /// @brief "SIMILAR TO" collation.
    similar,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(match_operator value) noexcept {
    using namespace std::string_view_literals;
    using kind = match_operator;
    switch (value) {
        case kind::like: return "like"sv;
        case kind::similar: return "similar"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, match_operator value) noexcept {
    return out << to_string_view(value);
}

/**
 * @brief a tag of match_operator.
 * @tparam Kind the match_operator kind
 */
template<match_operator Kind>
using match_operator_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of match_operator.
 * @tparam Kind the match_operator kind
 */
template<match_operator Kind>
inline constexpr match_operator_tag_t<Kind> match_operator_tag {};

/**
 * @brief invoke callback function for individual match operator kinds.
 * If the operator_kind is K, this may invoke Callback::operator()(match_operator_tag_t<K>, Args...).
 * You must declare all callback functions for individual match operators,
 * or declare Callback::operator()(match_operator, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param operator_kind the operator kind
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, match_operator operator_kind, Args&&... args) {
    using kind = match_operator;
    switch (operator_kind) {
        case kind::like: return util::enum_tag_callback<kind::like>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::similar: return util::enum_tag_callback<kind::similar>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::scalar
