#pragma once

#include <iterator>

namespace takatori::graph {

/**
 * @brief iterates over graph.
 * @tparam Iter the entity iterator type
 */
template<class Iter>
class graph_iterator {
public:
    /// @brief the iterator type.
    using iterator_type = graph_iterator;
    /// @brief the value type.
    using value_type = std::remove_pointer_t<typename std::iterator_traits<Iter>::value_type>;
    /// @brief the difference type.
    using difference_type = typename std::iterator_traits<Iter>::difference_type;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the iterator category tag.
    using iterator_category = std::bidirectional_iterator_tag;

    /**
     * @brief creates a new object.
     * @param iter the entity iterator
     */
    explicit graph_iterator(Iter iter) noexcept
        : iter_(iter)
    {}

    /**
     * @brief creates a new object.
     * @tparam U the entity iterator type of source iterator
     * @param other the source iterator
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_constructible_v<Iter, U>>>
    graph_iterator(graph_iterator<U> other) noexcept // NOLINT
        : iter_(other.iter_)
    {}

    /**
     * @brief returns the value reference where this iterator pointing.
     * @return reference of the current value
     */
    reference operator*() const noexcept {
        return **iter_;
    }

    /**
     * @brief returns pointer to the value where this iterator pointing.
     * @return pointer to the current value
     */
    pointer operator->() const noexcept {
        return *iter_;
    }

    /**
     * @brief increments this iterator position.
     * @return this
     */
    iterator_type& operator++() noexcept {
        ++iter_;
        return *this;
    }

    /**
     * @brief increments this iterator position.
     * @return the last position
     */
    iterator_type const operator++(int) noexcept { // NOLINT
        iterator_type r = *this;
        operator++();
        return r;
    }

    /**
     * @brief decrements this iterator position.
     * @return this
     */
    iterator_type& operator--() noexcept {
        --iter_;
        return *this;
    }

    /**
     * @brief decrements this iterator position.
     * @return the last position
     */
    iterator_type const operator--(int) noexcept { // NOLINT
        iterator_type r = *this;
        operator--();
        return r;
    }

    /**
     * @brief returns the entity iterator.
     * @return the entity iterator
     */
    [[nodiscard]] Iter unwrap() const noexcept {
        return iter_;
    }

private:
    Iter iter_;

    template<class U>
    friend class graph_iterator;
};

/// @private
template<class T>
graph_iterator(T) -> graph_iterator<T>;

/**
 * @brief returns whether or not the two iterator points the same element.
 * @tparam I1 entity iterator type of the first iterator
 * @tparam I2 entity iterator type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the two iterator points the same element
 * @return false otherwise
 */
template<class I1, class I2>
inline bool operator==(graph_iterator<I1> const& a, graph_iterator<I2> const& b) noexcept {
    return a.unwrap() == b.unwrap();
}

/**
 * @brief returns whether or not the two iterator points different elements.
 * @tparam I1 entity iterator type of the first iterator
 * @tparam I2 entity iterator type of the second iterator
 * @param a the first iterator
 * @param b the second iterator
 * @return true if the two iterator points different elements
 * @return false otherwise
 */
template<class I1, class I2>
inline bool operator!=(graph_iterator<I1> const& a, graph_iterator<I2> const& b) noexcept {
    return !(a == b);
}

} // namespace takatori::graph
