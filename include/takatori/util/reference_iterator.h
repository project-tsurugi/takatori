#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

#include "reference_extractor.h"

namespace takatori::util {

template<class Extractor>
class reference_list_view;

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
    /// @brief the iterator type
    using iterator_type = reference_iterator;
    /// @brief the value type
    using value_type = typename extractor_type::value_type;
    /// @brief the difference type
    using difference_type = typename extractor_type::difference_type;
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
     * @param extractor the cursor extractor
     */
    explicit constexpr reference_iterator(cursor_type cursor, extractor_type extractor = {}) noexcept
        : cursor_(cursor)
        , extractor_(std::move(extractor))
    {}

    /**
     * @brief creates a new object.
     * @tparam U the extractor type of source iterator
     * @param other the source iterator
     * @param extractor the cursor extractor
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_constructible_v<
                            cursor_type,
                            typename reference_iterator<U>::cursor_type>>>
    constexpr reference_iterator(reference_iterator<U> other, extractor_type extractor = {}) noexcept // NOLINT
        : cursor_(other.cursor_)
        , extractor_(extractor)
    {}

    /**
     * @brief returns the value reference where this iterator pointing.
     * @return reference of the current value
     */
    constexpr reference operator*() const noexcept {
        return extractor_.get(cursor_);
    }

    /**
     * @brief returns pointer to the value where this iterator pointing.
     * @return pointer to the current value
     */
    constexpr pointer operator->() const noexcept {
        return std::addressof(operator*());
    }

    /**
     * @brief returns the value reference where the given offset from this iterator pointing.
     * @param offset the offset count from the current position
     * @return reference of the target value
     */
    constexpr reference operator[](difference_type offset) const noexcept {
        return extractor_.get(extractor_.advance(cursor_, offset));
    }

    /**
     * @brief increments this iterator position.
     * @return this
     */
    constexpr iterator_type& operator++() noexcept {
        cursor_ = extractor_.advance(cursor_, +1);
        return *this;
    }

    /**
     * @brief increments this iterator position.
     * @return the last position
     */
    constexpr iterator_type const operator++(int) noexcept { // NOLINT
        iterator_type r { *this };
        operator++();
        return r;
    }

    /**
     * @brief decrements this iterator position.
     * @return this
     */
    constexpr iterator_type& operator--() noexcept {
        cursor_ = extractor_.advance(cursor_, -1);
        return *this;
    }

    /**
     * @brief decrements this iterator position.
     * @return the last position
     */
    iterator_type const operator--(int) noexcept { // NOLINT
        iterator_type r { *this };
        operator--();
        return r;
    }

    /**
     * @brief increments this iterator position.
     * @param difference number of increments
     * @return this
     */
    iterator_type& operator+=(difference_type difference) noexcept {
        cursor_ = extractor_.advance(cursor_, difference);
        return *this;
    }

    /**
     * @brief increments the given iterator position.
     * @param iter the target iterator
     * @param difference number of increments
     * @return the moved iterator
     */
    friend iterator_type operator+(iterator_type iter, difference_type difference) noexcept {
        return iter += difference;
    }

    /**
     * @brief increments the given iterator position.
     * @param difference number of increments
     * @param iter the target iterator
     * @return the moved iterator
     */
    friend iterator_type operator+(difference_type difference, iterator_type iter) noexcept {
        return iter += difference;
    }

    /**
     * @brief decrements this iterator position.
     * @param difference number of decrements
     * @return this
     */
    iterator_type& operator-=(difference_type difference) noexcept {
        cursor_ = extractor_.advance(cursor_, -difference);
        return *this;
    }

    /**
     * @brief decrements the given iterator position.
     * @param iter the target iterator
     * @param difference number of decrements
     * @return the moved iterator
     */
    friend iterator_type operator-(iterator_type iter, difference_type difference) noexcept {
        return iter -= difference;
    }

    /**
     * @brief computes distance between the two iterators.
     * @param a the first iterator
     * @param b the second iterator
     * @return the distance from the first iterator to the second one
     */
    friend difference_type operator-(iterator_type a, iterator_type b) noexcept {
        return a.extractor_.distance(b.cursor_, a.cursor_);
    }

private:
    cursor_type cursor_;
    extractor_type extractor_;

    template<class U> friend class reference_iterator;

    friend class reference_list_view<Extractor>;

    template<class T1, class T2>
    friend constexpr bool operator==(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend constexpr bool operator!=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend constexpr bool operator<(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend constexpr bool operator>(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend constexpr bool operator<=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend constexpr bool operator>=(reference_iterator<T1> a, reference_iterator<T2> b) noexcept; // NOLINT(readability-redundant-declaration)
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
