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
     * @param extractor the cursor extractor
     */
    explicit constexpr reference_list_view(cursor_type first, size_type size, extractor_type extractor = {}) noexcept
        : first_(first)
        , last_(extractor.advance(first_, static_cast<difference_type>(size)))
        , extractor_(std::move(extractor))
    {}

    /**
     * @brief creates a new instance.
     * @param first pointer to the first sequence data (inclusive)
     * @param last pointer to the last sequence data (exclusive)
     * @param extractor the cursor extractor
     * @attention if first > last, this consider the sequence is empty
     */
    explicit constexpr reference_list_view(cursor_type first, cursor_type last, extractor_type extractor = {}) noexcept
        : first_(first)
        , last_(last)
        , extractor_(std::move(extractor))
    {}

    /**
     * @brief creates a new instance.
     * @param first iterator of the first sequence data (inclusive)
     * @param last iterator of the last sequence data (exclusive)
     * @param extractor the cursor extractor
     * @attention if first > last, this consider the sequence is empty
     */
    explicit constexpr reference_list_view(iterator first, iterator last, extractor_type extractor = {}) noexcept
        : first_(first.cursor_)
        , last_(last.cursor_)
        , extractor_(std::move(extractor))
    {}

    /**
     * @brief creates a new instance.
     * @tparam Container the container type
     * @param container the container
     * @param extractor the cursor extractor
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
    explicit constexpr reference_list_view(Container& container, extractor_type extractor = {}) noexcept
        : first_(container.data())
        , last_(extractor.advance(first_, static_cast<difference_type>(container.size())))
        , extractor_(std::move(extractor))
    {}

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    [[nodiscard]] constexpr reference at(size_type position) const {
        auto iter = extractor_.advance(first_, static_cast<difference_type>(position));
        if (iter >= last_) {
            throw std::out_of_range("invalid position");
        }
        return extractor_.get(iter);
    }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    [[nodiscard]] constexpr reference operator[](size_type position) const {
        auto iter = extractor_.advance(first_, static_cast<difference_type>(position));
        return extractor_.get(iter);
    }

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] constexpr reference front() const {
        auto iter = first_;
        return extractor_.get(iter);
    }

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] constexpr reference back() const {
        auto iter = extractor_.advance(last_, -1);
        return extractor_.get(iter);
    }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool empty() const noexcept {
        return first_ == last_;
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] constexpr size_type size() const noexcept {
        auto distance = extractor_.distance(first_, last_);
        if (distance < 0) return 0;
        return static_cast<size_type>(distance);
    }

    /**
     * @brief returns a forward iterator which points the beginning of this sequence.
     * @return the iterator of beginning (inclusive)
     */
    [[nodiscard]] constexpr iterator begin() const noexcept {
        return iterator(first_, extractor_);
    }

    /**
     * @brief returns a forward iterator which points the ending of this sequence.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] constexpr iterator end() const noexcept {
        return iterator(last_, extractor_);
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
    cursor_type first_;
    cursor_type last_;
    extractor_type extractor_;

    template<class U>
    friend class reference_list_view;
};

/// @cond IMPL_DEFS
namespace impl {

template<class T>
using fit_extractor = std::conditional_t<
        std::is_pointer_v<T>,
        double_pointer_extractor<std::remove_pointer_t<T>>,
        pointer_extractor<T>>;

} // namespace impl
/// @endcond

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
template<class E>
reference_list_view(reference_iterator<E>, reference_iterator<E>)
-> reference_list_view<E>;

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

/**
 * @brief appends string representation of the given value.
 * @tparam E the extractor type
 * @param out the output stream
 * @param value the target value
 * @return the output stream
 */
template<class E>
inline std::ostream& operator<<(std::ostream& out, reference_list_view<E> const& value) {
    out << "{";
    bool cont = false;
    for (auto&& e: value) {
        if (cont) out << ", ";
        cont = true;
        out << e;
    }
    out << "}";
    return out;
}

} // namespace takatori::util
