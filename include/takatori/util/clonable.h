#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "detect.h"
#include "pointer_traits.h"
#include "rvalue_ptr.h"
#include "rvalue_reference_wrapper.h"

namespace takatori::util {

/// @private
namespace impl {

/// @private
template<class T> using clonable_t = decltype(std::declval<T>().clone());

} // namespace impl

/**
 * @brief tests if the target type supports polymorphic clone.
 * @details The clonable class "T" must have "T* T::clone() const".
 * @tparam T the target type
 */
template<class T> using is_clonable = std::is_convertible<
        util::detect_t<impl::clonable_t, std::remove_reference_t<T> const&>,
        std::remove_const_t<std::remove_reference_t<T>>*>;

/// @copydoc is_clonable
template<class T>
constexpr inline bool is_clonable_v = is_clonable<T>::value; // NOLINT(modernize-type-traits)

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @return the created clone
 */
template<class T>
[[nodiscard]] inline std::enable_if_t<
        is_clonable_v<std::remove_cv_t<std::remove_reference_t<T>>>,
        std::add_pointer_t<std::remove_cv_t<std::remove_reference_t<T>>>>
clone(T&& object) {
    return std::forward<T>(object).clone();
}

/// @copydoc clone(T&&)
template<class T>
[[nodiscard]] inline std::enable_if_t<
        is_clonable_v<T>,
        std::add_pointer_t<std::remove_cv_t<T>>>
clone(std::reference_wrapper<T> object) {
    return object.get().clone();
}

/// @copydoc clone(T&&)
template<class T>
[[nodiscard]] inline std::enable_if_t<
        is_clonable_v<T>,
        std::add_pointer_t<std::remove_cv_t<T>>>
clone(rvalue_reference_wrapper<T> object) {
    return object.get().clone();
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @return the created clone
 * @return `nullptr` if the object is absent
 */
template<class T>
[[nodiscard]] inline std::enable_if_t<is_clonable_v<T>, T*>
clone(T const* object) {
    if (!static_cast<bool>(object)) return nullptr;
    return object->clone();
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @tparam D the deleter type
 * @param object the object to clone
 * @return the created clone
 * @return empty if the object is absent
 */
template<class T, class D>
[[nodiscard]] inline std::enable_if_t<
        is_clonable_v<T>,
        std::add_pointer_t<std::remove_cv_t<T>>>
clone(std::unique_ptr<T, D> const& object) {
    if (!static_cast<bool>(object)) return nullptr;
    return object->clone();
}

/// @copydoc clone(std::unique_ptr<T, D> const&)
template<class T, class D>
[[nodiscard]] inline std::enable_if_t<
        is_clonable_v<T>,
        std::add_pointer_t<std::remove_cv_t<T>>>
clone(std::unique_ptr<T, D>&& object) {
    if (!static_cast<bool>(object)) return {};
    // TODO: require deep clone?
    return object.release();
}

/// @copydoc clone(T const*)
template<class T>
[[nodiscard]] inline std::enable_if_t<
        is_clonable_v<T>,
        std::add_pointer_t<std::remove_cv_t<T>>>
clone(rvalue_ptr<T> object) {
    if (!static_cast<bool>(object)) return {};
    return object.value().clone();
}

/**
 * @brief returns a clone of the given object, and wraps it into std::unique_ptr.
 * @tparam T the object type
 * @param object the object to clone
 * @return the created clone
 * @return empty if input is nullptr
 */
template<class T>
[[nodiscard]] inline auto clone_unique(T&& object) {
    auto* raw = clone(std::forward<T>(object));
    return std::unique_ptr<std::remove_pointer_t<decltype(raw)>>(raw);
}

/**
 * @brief returns a clone of the given object, and wraps it into std::shared_ptr.
 * @tparam T the object type
 * @param object the object to clone
 * @return the created clone
 * @return empty if input is nullptr
 */
template<class T>
[[nodiscard]] inline auto clone_shared(T&& object) {
    auto* raw = clone(std::forward<T>(object));
    return std::shared_ptr<std::remove_pointer_t<decltype(raw)>>(raw);
}

/**
 * @brief a copier which creates a copy via clone().
 * @tparam T the object type
 */
template<class T>
struct clonable_copier : is_clonable<T> {

    /// @brief copy is available.
    static constexpr inline bool is_available = is_clonable_v<T>;

    /// @brief the value type
    using value_type = std::remove_const_t<T>;

    /**
     * @brief the copier of the another object type.
     * @tparam the another object type
     */
    template<class U>
    using rebind = clonable_copier<U>;

    /**
     * @brief returns a new copy of the given object.
     * @param object the target object
     */
    [[nodiscard]] static std::unique_ptr<value_type> copy(value_type const& object) {
        return clone_unique(object);
    }

    /**
     * @brief returns a new copy of the given object.
     * @param object the target object
     */
    [[nodiscard]] static std::unique_ptr<value_type> copy(value_type&& object) {
        return clone_unique(std::move(object));
    }
};

} // namespace takatori::util
