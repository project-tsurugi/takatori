#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <utility>

#include <cstdlib>

#include <takatori/util/enum_set.h>
#include <takatori/util/enum_tag.h>

namespace takatori::relation {

/**
 * @brief represents a write operation kind.
 */
enum class write_kind {
    /// @brief inserts rows, or fail if any target rows are already exist.
    insert,
    /// @brief inserts rows only if individual rows are absent.
    insert_skip,
    /// @brief inserts rows, or overwrites even if the target rows already exist.
    insert_overwrite,
    /// @brief update existing rows, or fail if any target rows are absent.
    update,
    /// @brief remove existing rows, or ignore if any target rows are absent.
    delete_,
};

/// @brief a set of write_kind.
using write_kind_set = util::enum_set<
        write_kind,
        write_kind::insert,
        write_kind::delete_>;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(write_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = write_kind;
    switch (value) {
        case kind::insert: return "insert"sv;
        case kind::insert_skip: return "insert_skip"sv;
        case kind::insert_overwrite: return "insert_overwrite"sv;
        case kind::update: return "update"sv;
        case kind::delete_: return "delete"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, write_kind value) {
    return out << to_string_view(value);
}

/**
 * @brief a tag of write_kind.
 * @tparam Kind the write_kind kind
 */
template<write_kind Kind>
using write_kind_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of write_kind.
 * @tparam Kind the write_kind kind
 */
template<write_kind Kind>
inline constexpr write_kind_tag_t<Kind> write_kind_tag {};

/**
 * @brief invoke callback function for individual write operation kinds.
 * @details If the write_kind is K, this may invoke Callback::operator()(write_kind_tag_t<K>, Args...).
 * @attention You must declare all callback functions for individual join kinds,
 * or declare Callback::operator()(write_kind, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param tag_value the write_kind value
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, write_kind tag_value, Args&&... args) {
    using kind = write_kind;
    switch (tag_value) {
        case kind::insert: return util::enum_tag_callback<kind::insert>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::insert_skip: return util::enum_tag_callback<kind::insert_skip>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::insert_overwrite: return util::enum_tag_callback<kind::insert_overwrite>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::update: return util::enum_tag_callback<kind::update>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::delete_: return util::enum_tag_callback<kind::delete_>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::relation
