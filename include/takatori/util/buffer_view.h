#pragma once

#include <ostream>
#include <string_view>
#include <type_traits>
#include <stdexcept>

#include <cstddef>

namespace takatori::util {

/**
 * @brief a buffer view.
 * @details This does not holds ownership of the buffer area.
 */
class buffer_view {
public:
    /// @brief the byte type.
    using value_type = char;

    /// @brief the pointer type.
    using pointer = std::add_pointer_t<value_type>;

    /// @brief the constant pointer type.
    using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;

    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<value_type>;

    /// @brief the size and position type.
    using size_type = std::size_t;

    /// @brief the iterator type.
    using iterator = pointer;

    /// @brief the constant iterator type.
    using const_iterator = const_pointer;

    /// @brief the difference type.
    using difference_type = std::ptrdiff_t;

    /**
     * @brief creates a new empty instance.
     */
    constexpr buffer_view() noexcept = default;

    /**
     * @brief creates a new instance.
     * @param data the head of this buffer
     * @param size the number of bytes in the buffer
     */
    constexpr buffer_view(pointer data, size_type size) noexcept :
        data_ { data },
        size_ { size }
    {}

    /**
     * @brief returns the head of this buffer.
     * @return the head of this buffer.
     */
    [[nodiscard]] constexpr pointer data() const noexcept {
        return data_;
    }

    /**
     * @brief the buffer capacity.
     * @return capacity size in bytes
     */
    [[nodiscard]] constexpr size_type size() const noexcept {
        return size_;
    }

    /**
     * @brief returns whether or not this buffer is empty.
     * @return true if this buffer is empty
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool empty() const noexcept {
        return size_ == 0;
    }

    /**
     * @brief returns content at the position on this buffer.
     * @param position the content position
     * @return the buffer content
     * @pre `position < this.size()`
     * @attention undefined behavior if the `position >= this.size()`
     */
    [[nodiscard]] constexpr reference operator[](size_type position) const noexcept {
        return *(data_ + position); // NOLINT
    }

    /**
     * @brief returns content at the position on this buffer.
     * @param position the content position
     * @return the buffer content
     * @pre `position < size()`
     * @throw std::out_of_range if `position >= this.size()`
     */
    [[nodiscard]] constexpr reference at(size_type position) const {
        if (position >= size_) {
            throw std::out_of_range("buffer position is out of range");
        }
        return operator[](position);
    }

    /**
     * @brief returns a slice of this buffer.
     * @param from the beginning position in this buffer
     * @param to the beginning position in this buffer
     * @return a new buffer_view with range of `[from. to)`
     */
    [[nodiscard]] constexpr buffer_view slice(size_type from, size_type to) const {
        if (from > to || to >= size_) {
            throw std::out_of_range("buffer position is out of range");
        }
        return {
            data_ + from, // NOLINT
            to - from,
        };
    }

    /**
     * @brief returns a value iterator that points to the beginning of this buffer.
     * @return an iterator of beginning
     */
    [[nodiscard]] constexpr iterator begin() const noexcept {
        return data_;
    }

    /// @copydoc begin()
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return const_iterator { begin() };
    }

    /**
     * @brief returns a value iterator that points to the ending of this buffer.
     * @return an iterator of ending
     */
    [[nodiscard]] constexpr iterator end() const noexcept {
        return data_ + size_; // NOLINT
    }

    /// @copydoc end()
    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return const_iterator { end() };
    }

    /**
     * @brief returns whether or not this buffer has contents.
     * @return true if this buffer is not empty
     * @return false if this buffer is empty
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return !empty();
    }

    /**
     * @brief returns the std::string_view of this buffer range.
     * @return the std::string_view of this buffer range
     */
    [[nodiscard]] explicit operator std::string_view() const noexcept {
        return std::string_view { data_, size_ };
    }

private:
    pointer data_ { nullptr };
    size_type size_ { 0 };
};

static_assert(std::is_trivially_copyable_v<buffer_view>);
static_assert(std::is_trivially_destructible_v<buffer_view>);

/**
 * @brief returns whether or not two buffer_view are equivalent.
 * @note This does not consider the buffer contents, only compares their buffer location.
 * @param left the left buffer_view
 * @param right the right buffer_view
 * @return true if they are equivalent
 * @return false otherwise
 */
constexpr bool operator==(buffer_view left, buffer_view right) noexcept {
    return left.data() == right.data()
        && left.size() == right.size();
}

/**
 * @brief returns whether or not two buffer_view are different.
 * @note This does not consider the buffer contents, only compares their buffer location.
 * @param left the left buffer_view
 * @param right the right buffer_view
 * @return true if they are different
 * @return false otherwise
 */
constexpr bool operator!=(buffer_view left, buffer_view right) noexcept {
    return !(left == right);
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, buffer_view value);

} // namespace takatori::util
