#pragma once

#include <iterator>
#include <stdexcept>

#include "reference_iterator.h"

namespace takatori::util {

/**
 * @brief a view of sequence.
 * @tparam T the element type
 */
template<class T>
class sequence_view {
public:
    /// @brief the value type
    using value_type = T;
    /// @brief the size type
    using size_type = std::size_t;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the L-value reference type
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the pointer type
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the iterator type
    using iterator = reference_iterator<pointer_extractor<value_type>>;

    /**
     * @brief creates a new empty instance.
     */
    constexpr sequence_view() = default;

    /**
     * @brief creates a new instance.
     * @param data pointer the head of sequence data
     * @param size the number of elements
     */
    explicit constexpr sequence_view(pointer data, size_type size = 1) noexcept
        : data_(data)
        , size_(size)
    {}

    /**
     * @brief creates a new instance.
     * @param first pointer to the first sequence data (inclusive)
     * @param last pointer to the last sequence data (exclusive)
     * @attention if first > last, this consider the sequence is empty
     */
    explicit constexpr sequence_view(pointer first, pointer last) noexcept
        : data_(first)
        , size_(first < last ? last - first : 0)
    {}

    /**
     * @brief creates a new instance.
     * @tparam Container the container type
     * @param container the container object
     */
    template<
            class Container,
            class = std::enable_if_t<
                    std::is_constructible_v<pointer, decltype(std::declval<Container>().data())>
                    && std::is_constructible_v<size_type, decltype(std::declval<Container>().size())>>
    >
    constexpr sequence_view(Container& container) noexcept // NOLINT
        : data_(container.data())
        , size_(container.size())
    {}

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    [[nodiscard]] constexpr reference at(size_type position) const {
        if (position >= size_) {
            throw std::out_of_range("invalid position");
        }
        return operator[](position);
    }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    constexpr reference operator[](size_type position) const {
        return *(data_ + position); // NOLINT
    }

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] constexpr reference front() const {
        return *data_; // NOLINT
    }

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] constexpr reference back() const {
        return *(data_ + (size_ - 1)); // NOLINT
    }

    /**
     * @brief returns pointer to the head of this sequence.
     * @return pointer to this sequence.
     */
    [[nodiscard]] constexpr pointer data() const noexcept {
        return data_;
    }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool empty() const noexcept {
        return size_ == 0;
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] constexpr size_type size() const noexcept {
        return size_;
    }


    /**
     * @brief returns a forward iterator which points the beginning of this sequence.
     * @return the iterator of beginning (inclusive)
     */
    [[nodiscard]] constexpr iterator begin() const noexcept {
        return iterator(data_);
    }

    /**
     * @brief returns a forward iterator which points the ending of this sequence.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] constexpr iterator end() const noexcept {
        return iterator(data_) + size_;
    }

    /**
     * @brief returns a backward iterator which points the reversed beginning of this sequence.
     * @return the reversed iterator of beginning (inclusive)
     */
    [[nodiscard]] constexpr std::reverse_iterator<iterator> rbegin() const noexcept {
        return std::make_reverse_iterator(end());
    }

    /**
     * @brief returns a backward iterator which points the reversed ending of this sequence.
     * @return the reversed iterator of ending (exclusive)
     */
    [[nodiscard]] constexpr std::reverse_iterator<iterator> rend() const noexcept {
        return std::make_reverse_iterator(begin());
    }

private:
    pointer data_ {};
    size_type size_ {};
};

/// @private
template<class T>
sequence_view(T*) -> sequence_view<T>;

/// @private
template<class T>
sequence_view(T*, std::size_t) -> sequence_view<T>;

/// @private
template<class T>
sequence_view(T*, T*) -> sequence_view<T>;

/// @private
template<class T>
sequence_view(T&) -> sequence_view<std::remove_pointer_t<decltype(std::declval<T>().data())>>;

} // namespace takatori::util
