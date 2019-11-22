#pragma once

#include <iostream>
#include <utility>

#include "print_support.h"

namespace takatori::util {

/**
 * @brief printing support for printable objects.
 * @tparam T1 the first element type
 * @tparam T2 the second element type
 */
template<class T1, class T2>
class print_support<std::pair<T1, T2> const> {
public:
    /// @brief the value type
    using value_type = std::pair<T1, T2> const;

    /**
     * @brief creates a new object.
     * @param object the print target
     */
    explicit constexpr print_support(value_type& object) noexcept : ptr_(std::addressof(object)) {}

    /**
     * @brief appends string representation of wrapped object.
     * @param out the target output
     * @param object the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, print_support const& object) {
        return out << "{"
                   << util::print_support { object.ptr_->first } << ", "
                   << util::print_support { object.ptr_->second } << "}";
    }

private:
    value_type* ptr_;
};

} // namespace takatori::util
