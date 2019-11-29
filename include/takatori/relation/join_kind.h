#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include <cstdlib>

#include "takatori/util/enum_tag.h"

namespace takatori::relation {

/**
 * @brief represents a join kind.
 */
enum class join_kind {
    /// @brief inner (or cross) join.
    inner,
    /// @brief left outer join.
    left_outer,
    /// @brief full outer join.
    full_outer,
    /// @brief left semi join
    semi,
    /// @brief left anti join
    anti,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(join_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = join_kind;
    switch (value) {
        case kind::inner: return "inner"sv;
        case kind::left_outer: return "left_outer"sv;
        case kind::full_outer: return "full_outer"sv;
        case kind::semi: return "semi"sv;
        case kind::anti: return "anti"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, join_kind value) {
    return out << to_string_view(value);
}

/**
 * @brief a tag of join_kind.
 * @tparam Kind the join_kind kind
 */
template<join_kind Kind>
using join_kind_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of join_kind.
 * @tparam Kind the join_kind kind
 */
template<join_kind Kind>
inline constexpr join_kind_tag_t<Kind> join_kind_tag {};

/**
 * @brief invoke callback function for individual join kinds.
 * If the join_kind is K, this may invoke Callback::operator()(join_kind_tag_t<K>, Args...).
 * You must declare all callback functions for individual join kinds,
 * or declare Callback::operator()(join_kind, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param tag_value the join_kind value
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, join_kind tag_value, Args&&... args) {
    using kind = join_kind;
    switch (tag_value) {
        case kind::inner: return util::enum_tag_callback<kind::inner>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::left_outer: return util::enum_tag_callback<kind::left_outer>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::full_outer: return util::enum_tag_callback<kind::full_outer>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::semi: return util::enum_tag_callback<kind::semi>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::anti: return util::enum_tag_callback<kind::anti>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::relation
