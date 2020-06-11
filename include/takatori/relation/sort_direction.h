#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_tag.h>

namespace takatori::relation {

/**
 * @brief represents kind of sort direction.
 */
enum class sort_direction {
    /// @brief ascendant order.
    ascendant,
    /// @brief descendant order.
    descendant,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(sort_direction value) noexcept {
    using namespace std::string_view_literals;
    using kind = sort_direction;
    switch (value) {
        case kind::ascendant: return "ascendant"sv;
        case kind::descendant: return "descendant"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, sort_direction value) noexcept {
    return out << to_string_view(value);
}

/**
 * @brief a tag of direction.
 * @tparam Kind the direction kind
 */
template<sort_direction Kind>
using sort_direction_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of direction.
 * @tparam Kind the direction kind
 */
template<sort_direction Kind>
inline constexpr sort_direction_tag_t<Kind> sort_direction_tag {};

/**
 * @brief invoke callback function for individual sort direction kinds.
 * @details If the direction_kind is K, this may invoke Callback::operator()(sort_direction_tag_t<K>, Args...).
 * @attention You must declare all callback functions for individual directions,
 * or declare Callback::operator()(direction, Args...) as "default" callback function.
 * Each return type of callback functions must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param tag_value the direction kind
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, sort_direction tag_value, Args&&... args) {
    using kind = sort_direction;
    switch (tag_value) {
        case kind::ascendant: return util::enum_tag_callback<kind::ascendant>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::descendant: return util::enum_tag_callback<kind::descendant>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::relation
