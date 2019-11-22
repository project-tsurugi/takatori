#pragma once

#include <iostream>
#include <vector>

#include "print_support.h"

namespace takatori::util {

/**
 * @brief printing support for printable objects.
 * @tparam T the target type
 * @tparam Allocator the allocator type
 */
template<class T, class Allocator>
class print_support<std::vector<T, Allocator>> {
public:
    /// @brief the value type
    using value_type = std::vector<T, Allocator>;

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
        out << "{";
        bool cont = false;
        for (auto&& e: *object.ptr_) {
            if (cont) out << ", ";
            cont = true;
            out << util::print_support { e };
        }
        out << "}";
        return out;
    }

private:
    value_type const* ptr_;
};

} // namespace takatori::util
