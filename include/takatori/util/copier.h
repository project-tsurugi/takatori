#pragma once

#include <stdexcept>
#include <type_traits>

#include "clonable.h"

namespace takatori::util {

/**
 * @brief a copiler but it just represents "copy is unavailable".
 * @tparam T the object type
 */
template<class T>
struct null_copier {

    /// @brief copy is NOT available.
    static constexpr inline bool is_available = false;

    /// @brief the object type
    using value_type = std::remove_const_t<T>;

    /// @brief the pointer type
    using pointer = std::add_pointer_t<T>;

    /**
     * @brief the copier of the another object type.
     * @tparam the another object type
     */
    template<class U>
    using rebind = null_copier<U>;

    /**
     * @brief just raise an error.
     * @throws std::runtime_error always
     */
    [[noreturn]] static pointer copy(object_creator, value_type const&) {
        throw std::runtime_error("copy() is unavailable");
    }
};

/**
 * @brief a copier which creates a copy via copy constructor.
 * @tparam T the object type
 */
template<class T>
struct standard_copier : std::is_copy_constructible<T> {

    /// @brief copy is available.
    static constexpr inline bool is_available = std::is_copy_constructible_v<T>;

    /// @brief the object type
    using value_type = std::remove_const_t<T>;

    /// @brief the pointer type
    using pointer = std::add_pointer_t<T>;

    /**
     * @brief the copier of the another object type.
     * @tparam the another object type
     */
    template<class U>
    using rebind = standard_copier<U>;

    /**
     * @brief returns a new copy of the given object.
     * @param creator the object creator
     * @param object the target object
     * @return the created copy, it must be delete via creator.delete_object()
     */
    static pointer copy(object_creator creator, value_type const& object) {
        return creator.create_object<value_type>(object);
    }

    /**
     * @brief returns a new copy of the given object.
     * @param creator the object creator
     * @param object the target object
     * @return the created copy, it must be delete via creator.delete_object()
     */
    static pointer copy(object_creator creator, value_type&& object) {
        return creator.create_object<value_type>(std::move(object));
    }
};

} // namespace takatori::util
