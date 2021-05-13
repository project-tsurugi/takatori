#pragma once

#include <type_traits>

#include "clonable.h"
#include "exception.h"

namespace takatori::util {

/**
 * @brief a copier but it just represents "copy is unavailable".
 * @tparam T the object type
 */
template<class T>
struct null_copier {

    /// @brief copy is NOT available.
    static constexpr inline bool is_available = false;

    /// @brief the object type
    using value_type = std::remove_const_t<T>;

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
    [[noreturn]] static std::unique_ptr<value_type> copy(value_type const&) {
        throw_exception(std::runtime_error("copy() is unavailable"));
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

    /**
     * @brief the copier of the another object type.
     * @tparam the another object type
     */
    template<class U>
    using rebind = standard_copier<U>;

    /**
     * @brief returns a new copy of the given object.
     * @param object the target object
     * @return the created copy
     */
    [[nodiscard]] static std::unique_ptr<value_type> copy(value_type const& object) {
        return std::make_unique<value_type>(object);
    }

    /**
     * @brief returns a new copy of the given object.
     * @param object the target object
     * @return the created copy
     */
    [[nodiscard]] static std::unique_ptr<value_type> copy(value_type&& object) {
        return std::make_unique<value_type>(std::move(object));
    }
};

} // namespace takatori::util
