#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "util/object_creator.h"
#include "util/pointer_traits.h"
#include "util/rvalue_ptr.h"
#include "util/rvalue_reference_wrapper.h"

namespace util {

/// @private
namespace impl {

/// @private
struct is_clonable {
    template<class T>
    std::enable_if_t<
            std::is_assignable_v<T*&, decltype(std::declval<T const&>().clone(std::declval<object_creator>()))>,
            std::true_type>
    operator()(T&&) const { return {}; }
    std::false_type operator()(...) const { return {}; } // NOLINT
};

} // namespace impl

/**
 * @brief tests if the target type supports polymorphic clone.
 * The clonable class "T" must have "T* T::clone(object_creator) const".
 * @tparam T the target type
 */
template<class T> struct is_clonable;

/// @private
template<class T> struct is_clonable : std::invoke_result_t<impl::is_clonable, std::remove_cv_t<std::remove_reference_t<T>>> {};

/// @copydoc is_clonable
template<class T>
constexpr inline bool is_clonable_v = is_clonable<T>::value;

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @param creator object creator for creating clones
 * @return the created clone
 */
template<class T>
inline std::enable_if_t<
        is_clonable_v<std::remove_cv_t<std::remove_reference_t<T>>>,
        std::add_pointer_t<std::remove_cv_t<std::remove_reference_t<T>>>>
clone(T&& object, object_creator creator = {}) {
    return std::forward<T>(object).clone(creator);
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @param creator object creator for creating clones
 * @return the created clone
 */
template<class T>
inline std::enable_if_t<is_clonable_v<T>, T*>
clone(std::reference_wrapper<T> object, object_creator creator = {}) {
    return object.get().clone(creator);
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @param creator object creator for creating clones
 * @return the created clone
 */
template<class T>
inline std::enable_if_t<is_clonable_v<T>, T*>
clone(rvalue_reference_wrapper<T> object, object_creator creator = {}) {
    return object.get().clone(creator);
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @param creator object creator for creating clones
 * @return the created clone
 * @return nullptr if input is nullptr
 */
template<class T>
inline std::enable_if_t<is_clonable_v<T>, T*>
clone(T const* object, object_creator creator = {}) {
    if (!static_cast<bool>(object)) return nullptr;
    return object->clone(creator);
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @param creator object creator for creating clones
 * @return the created clone
 * @return nullptr if input is nullptr
 */
template<class T>
inline std::enable_if_t<is_clonable_v<T>, T*>
clone(rvalue_ptr<T> object, object_creator creator = {}) {
    if (!static_cast<bool>(object)) return nullptr;
    return object.value().clone(creator);
}

/**
 * @brief returns a clone of the given object.
 * @tparam T the object type
 * @param object the object to clone
 * @param creator object creator for creating clones
 * @return the created clone
 * @return nullptr if input is nullptr
 */
template<class T>
inline auto clone_unique(T&& object, object_creator creator = {}) {
    return creator.wrap_unique(clone(std::forward<T>(object), creator));
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

    /// @brief the pointer type
    using pointer = std::add_pointer_t<T>;

    /**
     * @brief the copier of the another object type.
     * @tparam the another object type
     */
    template<class U>
    using rebind = clonable_copier<U>;

    /**
     * @brief returns a new copy of the given object.
     * @param creator the object creator
     * @param object the target object
     * @return the created copy, it must be delete via creator.delete_object()
     */
    static pointer copy(object_creator creator, value_type const& object) {
        return clone(object, creator);
    }

    /**
     * @brief returns a new copy of the given object.
     * @param creator the object creator
     * @param object the target object
     * @return the created copy, it must be delete via creator.delete_object()
     */
    static pointer copy(object_creator creator, value_type&& object) {
        return clone(std::move(object), creator);
    }
};

} // namespace util
