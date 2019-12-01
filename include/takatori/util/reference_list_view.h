#pragma once

#include <iterator>
#include <stdexcept>

#include "reference_extractor.h"
#include "reference_iterator.h"

namespace takatori::util {

/**
 * @brief a view of pointer list.
 * @tparam Extractor the extractor type
 */
template<class Extractor>
class reference_list_view {
public:
    /// @brief the cursor type
    using extractor_type = Extractor;

    /// @brief the value type
    using value_type = typename extractor_type::value_type;
    /// @brief the size type
    using size_type = std::size_t;
    /// @brief the difference type
    using difference_type = typename extractor_type::difference_type;
    /// @brief the L-value reference type
    using reference = typename extractor_type::reference;
    /// @brief the pointer type
    using pointer = typename extractor_type::pointer;
    /// @brief the cursor type type
    using cursor_type = typename extractor_type::cursor_type;

    /// @brief the iterator type
    using iterator = reference_iterator<extractor_type>;

    /**
     * @brief creates a new instance.
     * @param first pointer to the first sequence data (inclusive)
     * @param size the number of elements
     */
    explicit constexpr reference_list_view(cursor_type first, size_type size) noexcept;

    /**
     * @brief creates a new instance.
     * @param first pointer to the first sequence data (inclusive)
     * @param last pointer to the last sequence data (exclusive)
     * @attention if first > last, this consider the sequence is empty
     */
    explicit constexpr reference_list_view(cursor_type first, cursor_type last) noexcept;

    /**
     * @brief creates a new instance.
     * @tparam Container the container type
     * @param container the container
     */
    template<
            class Container,
            class = std::enable_if_t<
                    std::is_constructible_v<
                            cursor_type,
                            decltype(std::declval<Container&>().data())>
                    && std::is_convertible_v<
                            decltype(std::declval<Container&>().size()),
                            size_type>>>
    explicit constexpr reference_list_view(Container& container) noexcept;

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    constexpr reference at(size_type position) const;

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    constexpr reference operator[](size_type position) const;

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    constexpr reference front() const;

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    constexpr reference back() const;

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    constexpr bool empty() const noexcept;

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    constexpr size_type size() const noexcept;

    /**
     * @brief returns a forward iterator which points the beginning of this sequence.
     * @return the iterator of beginning (inclusive)
     */
    constexpr iterator begin() const noexcept;

    /**
     * @brief returns a forward iterator which points the ending of this sequence.
     * @return the iterator of ending (exclusive)
     */
    constexpr iterator end() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed beginning of this sequence.
     * @return the reversed iterator of beginning (inclusive)
     */
    constexpr std::reverse_iterator<iterator> rbegin() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed ending of this sequence.
     * @return the reversed iterator of ending (exclusive)
     */
    constexpr std::reverse_iterator<iterator> rend() const noexcept;

private:
    cursor_type first_;
    cursor_type last_;

    template<class U>
    friend class reference_list_view;
};

/// @private
namespace impl {

/// @private
template<class T>
using fit_extractor = std::conditional_t<
        std::is_pointer_v<T>,
        double_pointer_extractor<std::remove_pointer_t<T>>,
        pointer_extractor<T>>;

} // namespace impl

/// @private
template<class T>
reference_list_view(T*, std::size_t)
-> reference_list_view<
        std::enable_if_t<
                !std::is_pointer_v<T>,
                pointer_extractor<T>>>;

/// @private
template<class T>
reference_list_view(T* const*, std::size_t)
-> reference_list_view<
        std::enable_if_t<
                !std::is_pointer_v<T>,
                double_pointer_extractor<T>>>;

/// @private
template<class T>
reference_list_view(T*, T*)
-> reference_list_view<
        std::enable_if_t<
                !std::is_pointer_v<T>,
                pointer_extractor<T>>>;

/// @private
template<class T>
reference_list_view(T* const*, T* const*)
-> reference_list_view<
        std::enable_if_t<
                !std::is_pointer_v<T>,
                double_pointer_extractor<T>>>;

/// @private
template<class Container>
reference_list_view(Container& container) -> reference_list_view<
        std::enable_if_t<
                std::is_constructible_v<
                        typename impl::fit_extractor<typename Container::value_type>::cursor_type,
                        decltype(std::declval<Container&>().data())>
                && std::is_constructible_v<
                        decltype(std::declval<Container&>().size()),
                        std::size_t>
                && !std::is_pointer_v<typename impl::fit_extractor<typename Container::value_type>::value_type>,
                impl::fit_extractor<typename Container::value_type>>>;

template<class T>
inline constexpr
reference_list_view<T>::reference_list_view(cursor_type first, reference_list_view::size_type size) noexcept
    : first_(first)
    , last_(extractor_type::advance(first_, static_cast<difference_type>(size)))
{}

template<class T>
inline constexpr
reference_list_view<T>::reference_list_view(cursor_type first, cursor_type last) noexcept
    : first_(first)
    , last_(last)
{}

template<class Extractor>
template<class Container, class>
constexpr reference_list_view<Extractor>::reference_list_view(Container& container) noexcept
    : first_(container.data())
    , last_(extractor_type::advance(first_, static_cast<difference_type>(container.size())))
{}

template<class T>
inline constexpr typename reference_list_view<T>::reference
reference_list_view<T>::at(size_type position) const {
    auto iter = extractor_type::advance(first_, static_cast<difference_type>(position));
    if (iter >= last_) {
        throw std::out_of_range("invalid position");
    }
    return extractor_type::get(iter);
}

template<class T>
inline constexpr typename reference_list_view<T>::reference
reference_list_view<T>::operator[](size_type position) const {
    auto iter = extractor_type::advance(first_, static_cast<difference_type>(position));
    return extractor_type::get(iter);
}

template<class T>
inline constexpr typename reference_list_view<T>::reference
reference_list_view<T>::front() const {
    auto iter = first_;
    return extractor_type::get(iter);
}

template<class T>
inline constexpr typename reference_list_view<T>::reference
reference_list_view<T>::back() const {
    auto iter = extractor_type::advance(last_, -1);
    return extractor_type::get(iter);
}

template<class T>
inline constexpr bool
reference_list_view<T>::empty() const noexcept {
    return first_ == last_;
}

template<class T>
inline constexpr typename reference_list_view<T>::size_type
reference_list_view<T>::size() const noexcept {
    auto distance = std::distance(first_, last_);
    if (distance < 0) return 0;
    return static_cast<size_type>(distance);
}

template<class T>
inline constexpr typename reference_list_view<T>::iterator
reference_list_view<T>::begin() const noexcept {
    return iterator(first_);
}

template<class T>
inline constexpr typename reference_list_view<T>::iterator
reference_list_view<T>::end() const noexcept {
    return iterator(last_);
}

template<class T>
inline constexpr std::reverse_iterator<typename reference_list_view<T>::iterator>
reference_list_view<T>::rbegin() const noexcept {
    return std::make_reverse_iterator(end());
}

template<class T>
inline constexpr std::reverse_iterator<typename reference_list_view<T>::iterator>
reference_list_view<T>::rend() const noexcept {
    return std::make_reverse_iterator(begin());
}

} // namespace takatori::util
