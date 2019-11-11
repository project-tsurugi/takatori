#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace takatori::util {

/**
 * @brief extracts reference from pointer.
 * @tparam T the value type
 */
template<class T>
struct pointer_extractor {

    /// @brief the value type
    using value_type = T;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the cursor type.
    using cursor_type = std::add_pointer_t<std::add_const_t<pointer>>;

    /**
     * @brief extracts a reference on the cursor.
     * @param cursor the target cursor
     * @return the reference
     */
    static reference get(cursor_type cursor) noexcept { return *cursor; }

    /**
     * @brief extracts a reference on the cursor with the offset.
     * @param cursor the target cursor
     * @param offset
     * @return the reference
     */
    static reference get(cursor_type cursor, std::ptrdiff_t offset) noexcept { return cursor[offset]; } // NOLINT
};

/**
 * @brief extracts reference from pointer of pointer.
 * @tparam T the value type
 */
template<class T>
struct double_pointer_extractor {

    /// @brief the value type
    using value_type = T;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the cursor type.
    using cursor_type = std::add_pointer_t<std::add_const_t<pointer>>;

    /**
     * @brief extracts a reference on the cursor.
     * @param cursor the target cursor
     * @return the reference
     */
    static reference get(cursor_type cursor) noexcept { return **cursor; }

    /**
     * @brief extracts a reference on the cursor with the offset.
     * @param cursor the target cursor
     * @param offset
     * @return the reference
     */
    static reference get(cursor_type cursor, std::ptrdiff_t offset) noexcept { return *cursor[offset]; } // NOLINT
};

template<class Extractor>
class reference_iterator;

template<class T1, class T2>
inline constexpr bool operator==(reference_iterator<T1> a, reference_iterator<T2> b) noexcept;

template<class T1, class T2>
inline constexpr bool operator!=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept;

template<class T1, class T2>
inline constexpr bool operator<(reference_iterator<T1> a, reference_iterator<T2> b) noexcept;

template<class T1, class T2>
inline constexpr bool operator>(reference_iterator<T1> a, reference_iterator<T2> b) noexcept;

template<class T1, class T2>
inline constexpr bool operator<=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept;

template<class T1, class T2>
inline constexpr bool operator>=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept;

/**
 * @brief an iterator over pointer to references.
 * @tparam Extractor the reference resolver
 */
template<class Extractor>
class reference_iterator {
public:
    /// @brief the reference resolver type
    using extractor_type = Extractor;
    /// @brief the value type
    using value_type = typename extractor_type::value_type;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the pointer type.
    using pointer = typename extractor_type::pointer;
    /// @brief the reference type.
    using reference = typename extractor_type::reference;
    /// @brief the cursor type.
    using cursor_type = typename extractor_type::cursor_type;
    /// @brief the iterator category tag.
    using iterator_category = std::random_access_iterator_tag;

    /**
     * @brief creates a new object.
     * @param cursor the cursor of beginning of the iterator
     */
    explicit constexpr reference_iterator(cursor_type cursor) noexcept : cursor_(cursor) {}

    /**
     * @brief creates a new object.
     * @tparam U the extractor type of source iterator
     * @param other the source iterator
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_constructible_v<
                            cursor_type,
                            typename reference_iterator<U>::cursor_type>>>
    constexpr reference_iterator(reference_iterator<U> other) noexcept : cursor_(other.cursor_) {} // NOLINT

    /**
     * @brief returns the value reference where this iterator pointing.
     * @return reference of the current value
     */
    reference operator*() const noexcept { return extractor_type::get(cursor_); }

    /**
     * @brief returns pointer to the value where this iterator pointing.
     * @return pointer to the current value
     */
    pointer operator->() const noexcept { return std::addressof(operator*()); }

    /**
     * @brief returns the value reference where the given offset from this iterator pointing.
     * @param offset the offset count from the current position
     * @return reference of the target value
     */
    reference operator[](difference_type offset) const noexcept { return extractor_type::get(cursor_, offset); }

    /**
     * @brief increments this iterator position.
     * @return this
     */
    reference_iterator& operator++() noexcept { ++cursor_; return *this; } // NOLINT

    /**
     * @brief increments this iterator position.
     * @return the last position
     */
    reference_iterator const operator++(int) noexcept { return iterator(cursor_++); } // NOLINT

    /**
     * @brief decrements this iterator position.
     * @return this
     */
    reference_iterator& operator--() noexcept { --cursor_; return *this; } // NOLINT

    /**
     * @brief decrements this iterator position.
     * @return the last position
     */
    reference_iterator const operator--(int) noexcept { return iterator(cursor_--); } // NOLINT

    /**
     * @brief increments this iterator position.
     * @param difference number of increments
     * @return this
     */
    reference_iterator& operator+=(difference_type difference) noexcept {
        cursor_ += difference; // NOLINT
        return *this;
    }

    /**
     * @brief increments the given iterator position.
     * @param iter the target iterator
     * @param difference number of increments
     * @return the moved iterator
     */
    friend reference_iterator operator+(reference_iterator iter, difference_type difference) noexcept {
        return iter += difference;
    }

    /**
     * @brief increments the given iterator position.
     * @param difference number of increments
     * @param iter the target iterator
     * @return the moved iterator
     */
    friend reference_iterator operator+(difference_type difference, reference_iterator iter) noexcept {
        return iter += difference;
    }

    /**
     * @brief decrements this iterator position.
     * @param difference number of decrements
     * @return this
     */
    reference_iterator& operator-=(difference_type difference) noexcept {
        cursor_ -= difference; // NOLINT
        return *this;
    }

    /**
     * @brief decrements the given iterator position.
     * @param iter the target iterator
     * @param difference number of decrements
     * @return the moved iterator
     */
    friend reference_iterator operator-(reference_iterator iter, difference_type difference) noexcept {
        return iter -= difference;
    }

    /**
     * @brief computes distance between the two iterators.
     * @param a the first iterator
     * @param b the second iterator
     * @return the distance from the first iterator to the second one
     */
    friend difference_type operator-(reference_iterator a, reference_iterator b) noexcept {
        return a.cursor_ - b.cursor_; // NOLINT
    }

private:
    pointer const* cursor_;

    template<class U> friend class reference_iterator;

    template<class T1, class T2>
    friend inline constexpr bool operator==(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend inline constexpr bool operator!=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend inline constexpr bool operator<(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend inline constexpr bool operator>(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend inline constexpr bool operator<=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend inline constexpr bool operator>=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)
};

/// @private
template<class T>
reference_iterator(T*) -> reference_iterator<pointer_extractor<T>>;

/// @private
template<class T>
reference_iterator(T**) -> reference_iterator<double_pointer_extractor<T>>;

/// @private
template<class T>
reference_iterator(T* const*) -> reference_iterator<double_pointer_extractor<T>>;

/**
 * @brief returns whether or not the two iterator points the same position.
 * @tparam T1 value type of the first iterator
 * @tparam T2 value type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the two iterator points the same position
 * @return false otherwise
 */
template<class T1, class T2>
inline constexpr bool operator==(reference_iterator<T1> a, reference_iterator<T2> b) noexcept { return a.cursor_ == b.cursor_; }

/**
 * @brief returns whether or not the two iterator points the different position.
 * @tparam T1 value type of the first iterator
 * @tparam T2 value type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the two iterator points the different position
 * @return false otherwise
 */
template<class T1, class T2>
inline constexpr bool operator!=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept { return a.cursor_ != b.cursor_; }

/**
 * @brief returns whether or not the first iterator position is smaller than the second one.
 * @tparam T1 value type of the first iterator
 * @tparam T2 value type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the first iterator position is smaller than the second one
 * @return false otherwise
 */
template<class T1, class T2>
inline constexpr bool operator<(reference_iterator<T1> a, reference_iterator<T2> b) noexcept { return a.cursor_ < b.cursor_; }

/**
 * @brief returns whether or not the first iterator position is bigger than the second one.
 * @tparam T1 value type of the first iterator
 * @tparam T2 value type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the first iterator position is bigger than the second one
 * @return false otherwise
 */
template<class T1, class T2>
inline constexpr bool operator>(reference_iterator<T1> a, reference_iterator<T2> b) noexcept { return a.cursor_ > b.cursor_; }

/**
 * @brief returns whether or not the first iterator position is smaller than or equal to the second one.
 * @tparam T1 value type of the first iterator
 * @tparam T2 value type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the first iterator position is smaller than or equal to the second one
 * @return false otherwise
 */
template<class T1, class T2>
inline constexpr bool operator<=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept { return a.cursor_ <= b.cursor_; }

/**
 * @brief returns whether or not the first iterator position is bigger than or equal to the second one.
 * @tparam T1 value type of the first iterator
 * @tparam T2 value type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the first iterator position is bigger than or equal to the second one
 * @return false otherwise
 */
template<class T1, class T2>
inline constexpr bool operator>=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept { return a.cursor_ >= b.cursor_; }

} // namespace takatori::util
