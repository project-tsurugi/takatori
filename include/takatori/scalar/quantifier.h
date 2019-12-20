#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <cstdlib>

#include <takatori/util/enum_tag.h>

namespace takatori::scalar {

/**
 * @brief represents kind of quantifier.
 */
enum class quantifier {
    /// @brief universal quantifier.
    all,
    /// @brief existential quantifier.
    any,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(quantifier value) noexcept {
    using namespace std::string_view_literals;
    using kind = quantifier;
    switch (value) {
        case kind::all: return "all"sv;
        case kind::any: return "any"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, quantifier value) noexcept {
    return out << to_string_view(value);
}

/**
 * @brief a tag of quantifier.
 * @tparam Kind the quantifier kind
 */
template<quantifier Kind>
using quantifier_tag_t = util::enum_tag_t<Kind>;

/**
 * @brief a tag object of quantifier.
 * @tparam Kind the quantifier kind
 */
template<quantifier Kind>
inline constexpr quantifier_tag_t<Kind> quantifier_tag {};

/**
 * @brief invoke callback function for individual quantifier kinds.
 * @details If the quantifier_kind is K, this may invoke Callback::operator()(quantifier_tag_t<K>, Args...).
 * @attention You must declare all callback functions for individual quantifiers,
 * or declare Callback::operator()(quantifier, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param quantifier_kind the quantifier kind
 * @param callback the callback object
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, quantifier quantifier_kind, Args&&... args) {
    using kind = quantifier;
    switch (quantifier_kind) {
        case kind::all: return util::enum_tag_callback<kind::all>(std::forward<Callback>(callback), std::forward<Args>(args)...);
        case kind::any: return util::enum_tag_callback<kind::any>(std::forward<Callback>(callback), std::forward<Args>(args)...);
    }
    std::abort();
}

} // namespace takatori::scalar
