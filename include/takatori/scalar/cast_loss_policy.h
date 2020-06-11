#pragma once

#include <ostream>
#include <memory>
#include <string>
#include <string_view>

#include <takatori/util/enum_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::scalar {

/**
 * @brief action kind when the value precision was decreased.
 */
enum class cast_loss_policy {

    /// @brief ignores precision decrease (may apply default trimming or rounding).
    ignore,

    /// @brief round towards to -infinity.
    floor,

    /// @brief round towards to +infinity.
    ceil,

    /// @brief replace with unknown value.
    unknown,

    /// @brief raise warning and continue to operation (may apply default trimming or rounding).
    warn,

    /// @brief raise error and halt the operation.
    error,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(cast_loss_policy value) noexcept {
    using namespace std::string_view_literals;
    using kind = cast_loss_policy;
    switch (value) {
        case kind::ignore: return "ignore"sv;
        case kind::floor: return "floor"sv;
        case kind::ceil: return "ceil"sv;
        case kind::unknown: return "unknown"sv;
        case kind::warn: return "warn"sv;
        case kind::error: return "error"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, cast_loss_policy value) {
    return out << to_string_view(value);
}

/**
 * @brief a tag of cast_loss_policy.
 * @tparam Kind the cast_loss_policy kind
 */
template<cast_loss_policy Kind>
using cast_loss_policy_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of cast_loss_policy.
 * @tparam Kind the cast_loss_policy kind
 */
template<cast_loss_policy Kind>
inline constexpr cast_loss_policy_tag_t<Kind> cast_loss_policy_tag {};

/**
 * @brief invoke callback function for individual loss policies.
 * @details If the policy is K, this may invoke Callback::operator()(cast_loss_policy_tag_t<K>, Args...).
 * @attention You must declare all callback functions for individual loss policies,
 * or declare Callback::operator()(cast_loss_policy, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param policy the policy
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, cast_loss_policy policy, Args&&... args) {
    using kind = cast_loss_policy;
    switch (policy) {
        case kind::ignore: return util::enum_tag_callback<kind::ignore>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::floor: return util::enum_tag_callback<kind::floor>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::ceil: return util::enum_tag_callback<kind::ceil>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::unknown: return util::enum_tag_callback<kind::unknown>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::warn: return util::enum_tag_callback<kind::warn>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::error: return util::enum_tag_callback<kind::error>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::scalar
