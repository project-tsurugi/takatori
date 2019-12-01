#pragma once

#include <iterator>

namespace takatori::graph {

template<class Iter>
class graph_iterator;

template<class I1, class I2>
bool operator==(graph_iterator<I1> const& a, graph_iterator<I2> const& b) noexcept;

template<class I1, class I2>
bool operator!=(graph_iterator<I1> const& a, graph_iterator<I2> const& b) noexcept;

/**
 * @brief iterates over graph.
 * @tparam Iter the entity iterator type
 */
template<class Iter>
class graph_iterator {
public:
    /// @brief the iterator type.
    using iterator_type = graph_iterator;
    /// @brief the value type
    using value_type = std::remove_pointer_t<typename std::iterator_traits<Iter>::value_type>;
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
    explicit graph_iterator(Iter iter) noexcept;

    /**
     * @brief creates a new object.
     * @tparam U the entity iterator type of source iterator
     * @param other the source iterator
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_constructible_v<Iter, U>>>
    graph_iterator(graph_iterator<U> other) noexcept; // NOLINT

    /**
     * @brief returns the value reference where this iterator pointing.
     * @return reference of the current value
     */
    reference operator*() const noexcept;

    /**
     * @brief returns pointer to the value where this iterator pointing.
     * @return pointer to the current value
     */
    pointer operator->() const noexcept;

    /**
     * @brief increments this iterator position.
     * @return this
     */
    iterator_type& operator++() noexcept;

    /**
     * @brief increments this iterator position.
     * @return the last position
     */
    iterator_type const operator++(int) noexcept; // NOLINT

    /**
     * @brief decrements this iterator position.
     * @return this
     */
    iterator_type& operator--() noexcept;

    /**
     * @brief decrements this iterator position.
     * @return the last position
     */
    iterator_type const operator--(int) noexcept; // NOLINT

    /**
     * @brief returns the entity iterator.
     * @return the entity iterator
     */
    Iter unwrap() noexcept;

private:
    Iter iter_;

    template<class U>
    friend class graph_iterator;

    template<class I1, class I2>
    friend bool operator==(graph_iterator<I1> const& a, graph_iterator<I2> const& b) noexcept; // NOLINT

    template<class I1, class I2>
    friend bool operator!=(graph_iterator<I1> const& a, graph_iterator<I2> const& b) noexcept; // NOLINT
};

/// @private
template<class T>
graph_iterator(T) -> graph_iterator<T>;

template<class Iter>
inline
graph_iterator<Iter>::graph_iterator(Iter iter) noexcept
    : iter_(iter)
{}

template<class Iter>
template<class U, class>
inline
graph_iterator<Iter>::graph_iterator(graph_iterator<U> other) noexcept
    : iter_(other.iter_)
{}

template<class Iter>
inline typename graph_iterator<Iter>::reference
graph_iterator<Iter>::operator*() const noexcept {
    return **iter_;
}

template<class Iter>
inline typename graph_iterator<Iter>::pointer
graph_iterator<Iter>::operator->() const noexcept {
    return *iter_;
}

template<class Iter>
inline typename graph_iterator<Iter>::iterator_type&
graph_iterator<Iter>::operator++() noexcept {
    ++iter_;
    return *this;
}

template<class Iter>
inline typename graph_iterator<Iter>::iterator_type const // NOLINT
graph_iterator<Iter>::operator++(int) noexcept {
    iterator_type r = *this;
    operator++();
    return r;
}

template<class Iter>
inline typename graph_iterator<Iter>::iterator_type&
graph_iterator<Iter>::operator--() noexcept {
    --iter_;
    return *this;
}

template<class Iter>
inline typename graph_iterator<Iter>::iterator_type const // NOLINT
graph_iterator<Iter>::operator--(int) noexcept {
    iterator_type r = *this;
    operator--();
    return r;
}

template<class Iter>
Iter graph_iterator<Iter>::unwrap() noexcept {
    return iter_;
}

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
    return a.iter_ == b.iter_;
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
