#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include <cstdlib>

#include "takatori/util/enum_tag.h"

namespace takatori::relation {

/**
 * @brief represents an end-point kind of intervals.
 */
enum class endpoint_kind {

    /// @brief end-point is unspecified (unbound interval).
    unbound,

    /// @brief includes end-point key.
    inclusive,

    /// @brief excludes end-point key.
    exclusive,

    /// @brief includes entries which contain the end-point key as prefix.
    prefixed_inclusive,

    /// @brief excludes entries which contain the end-point key as prefix.
    prefixed_exclusive,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(endpoint_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = endpoint_kind;
    switch (value) {
        case kind::unbound: return "unbound"sv;
        case kind::inclusive: return "inclusive"sv;
        case kind::exclusive: return "exclusive"sv;
        case kind::prefixed_inclusive: return "prefixed_inclusive"sv;
        case kind::prefixed_exclusive: return "prefixed_exclusive"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, endpoint_kind value) {
    return out << to_string_view(value);
}

/**
 * @brief a tag of endpoint_kind.
 * @tparam Kind the endpoint kind
 */
template<endpoint_kind Kind>
using endpoint_kind_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of endpoint_kind.
 * @tparam Kind the endpoint kind
 */
template<endpoint_kind Kind>
inline constexpr endpoint_kind_tag_t<Kind> endpoint_kind_tag {};

/**
 * @brief invoke callback function for individual join kinds.
 * If the endpoint_kind is K, this may invoke Callback::operator()(endpoint_kind_tag_t<K>, Args...).
 * You must declare all callback functions for individual join kinds,
 * or declare Callback::operator()(endpoint_kind, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param tag_value the endpoint_kind value
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, endpoint_kind tag_value, Args&&... args) {
    using kind = endpoint_kind;
    switch (tag_value) {
        case kind::unbound: return util::enum_tag_callback<kind::unbound>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::inclusive: return util::enum_tag_callback<kind::inclusive>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::exclusive: return util::enum_tag_callback<kind::exclusive>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::prefixed_inclusive: return util::enum_tag_callback<kind::prefixed_inclusive>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::prefixed_exclusive: return util::enum_tag_callback<kind::prefixed_exclusive>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::relation
