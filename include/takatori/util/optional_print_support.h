#pragma once

#include <ostream>
#include <optional>

#include "print_support.h"

namespace takatori::util {

/**
 * @brief printing support for printable objects.
 * @tparam T the element type
 */
template<class T>
class print_support<std::optional<T>> {
public:
    /// @brief the value type
    using value_type = std::optional<T> const;

    /**
     * @brief creates a new object.
     * @param object the print target
     */
    explicit constexpr print_support(value_type const& object) noexcept : ptr_(std::addressof(object)) {}

    /**
     * @brief appends string representation of wrapped object.
     * @param out the target output
     * @param object the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, print_support const& object) {
        if (auto&& element = *object.ptr_) {
            return out << util::print_support { element.value() };
        }
        return out << "()";
    }

private:
    value_type const* ptr_;
};

} // namespace takatori::util
