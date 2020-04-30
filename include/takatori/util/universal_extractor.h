#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace takatori::util {

/**
 * @brief extracts references using std::function objects.
 * @tparam the value type
 */
template<class T>
class universal_extractor {
public:
    /// @brief the value type
    using value_type = T;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<T>;
    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<T>;
    /// @brief the cursor type.
    using cursor_type = void*;

    /// @brief the get() adapter type
    using getter_type = std::function<reference(cursor_type)>;
    /// @brief the advance() adapter type
    using advancer_type = std::function<cursor_type(cursor_type, difference_type)>;

    /**
     * @brief creates a new instance
     * @param getter a function that extracts references on cursors
     * @param advancer a function that advances cursors
     */
    universal_extractor(getter_type getter, advancer_type advancer) noexcept
        : getter_(std::move(getter))
        , advancer_(std::move(advancer))
    {}

    /**
     * @brief extracts a reference on the cursor.
     * @param cursor the target cursor
     * @return the reference
     */
    [[nodiscard]] reference get(cursor_type cursor) const noexcept {
        return getter_(cursor);
    }

    /**
     * @brief advances the cursor
     * @param cursor the target cursor
     * @param offset the cursor offset
     * @return the advanced cursor
     */
    [[nodiscard]] cursor_type advance(cursor_type cursor, difference_type offset) const noexcept {
        return advancer_(cursor, offset);
    }

    /**
     * @brief returns the distance of two cursors.
     * @param from the base cursor
     * @param to the target cursor
     * @return the distance from the base cursor to the target cursor
     */
    [[nodiscard]] difference_type distance(cursor_type from, cursor_type to) const noexcept {
        // FIXME: better implementation
        if (from == to) {
            return 0;
        }
        if (from < to) {
            auto* p = from;
            difference_type r {};
            do {
                p = advancer_(p, 1);
                ++r;
            } while (p < to);
            return r;
        }
        return -distance(to, from);
    }

private:
    getter_type getter_;
    advancer_type advancer_;
};

} // namespace takatori::util
