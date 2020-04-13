#pragma once

#include <initializer_list>
#include <bitset>
#include <stdexcept>
#include <type_traits>

#include "enum_set_enumerator.h"

namespace takatori::util {

/**
 * @brief a set of `enum` values.
 * @details This is designed for a **dense** enum type, whose each element is neighboring in underlying type value.
 *      And each enum element must not have the same underlying value.
 * @tparam Enum the enum type
 * @tparam First the first enum value (inclusive)
 * @tparam Last the last enum value (inclusive)
 * @attention If functions in this class receives parameter not in `[First, Last]` may occur undefined behavior.
 */
template<class Enum, Enum First, Enum Last>
class enum_set {
public:
    /// @brief the value type.
    using value_type = Enum;

    /// @brief index type.
    using index_type = std::size_t;

    /// @brief the iterator type.
    using iterator = enum_set_enumerator<Enum, First, Last>;

    /// @brief the const iterator type.
    using const_iterator = iterator;

    /// @brief the entity type.
    using entity_type = std::bitset<iterator::number_of_universe>;

    /// @brief the reference type.
    using reference = typename entity_type::reference;

    /**
     * @brief creates a new object.
     */
    constexpr enum_set() = default;

    /**
     * @brief creates a new object.
     * @param elements the initial elements
     */
    enum_set(std::initializer_list<value_type> elements) noexcept {
        for (auto e : elements) {
            insert(e);
        }
    }

    /**
     * @brief returns a reference of whether or not this contains the given element.
     * @details Developers can modify the returned reference, then it reflects to this container.
     * @param value the element
     * @return true if this contains the given element
     * @return false otherwise
     */
    reference at(value_type value) noexcept {
        return entries_[to_index(value)];
    }

    /**
     * @brief returns whether or not this contains the given element.
     * @param value the element
     * @return true if this contains the given element
     * @return false otherwise
     */
    constexpr bool at(value_type value) const noexcept {
        return entries_[to_index(value)];
    }

    /// @copydoc at(value_type) const
    constexpr bool contains(value_type value) const noexcept {
        return at(value);
    }

    /// @copydoc at(value_type)
    reference operator[](value_type value) noexcept {
        return at(value);
    }

    /// @copydoc at(value_type) const
    constexpr bool operator[](value_type value) const noexcept {
        return at(value);
    }

    /**
     * @brief returns whether or not this container does not have any elements.
     * @return true if this is empty
     * @return false otherwise
     */
    [[nodiscard]] bool empty() const noexcept {
        return entries_.none();
    }

    /**
     * @brief returns the number of elements in this container.
     * @return the number of elements
     */
    [[nodiscard]] std::size_t size() const noexcept {
        return entries_.count();
    }

    /**
     * @brief removes all elements in this container.
     */
    void clear() noexcept {
        entries_.reset();
    }

    /**
     * @brief inserts an element into this container.
     * @details if the element is already in this container, this operation does nothing.
     * @param value the element
     * @return iterator of the inserted position
     */
    iterator insert(value_type value) noexcept {
        auto index = to_index(value);
        entries_.set(index, true);
        return iterator { entries_, index };
    }

    /**
     * @brief erase an element from this container.
     * @details if the element is not in this container, this operation does nothing.
     * @param value the element
     * @return iterator of the next element position of the erased one
     */
    iterator erase(value_type value) noexcept {
        auto index = to_index(value);
        entries_.set(index, true);
        return iterator { entries_, index + 1 };
    }

    /**
     * @brief removes an element on the given position.
     * @param pos the target position
     * @return iterator of the next element position of the erased one
     * @warning undefined behavior if the given iterator does not point a valid element
     */
    iterator erase(const_iterator pos) {
        return erase(*pos);
    }

    /**
     * @brief returns an iterator which points the first element in this container.
     * @details If this container is empty, the returned iterator is as same as end().
     * @note The iterator of this container does not satisfy `std::iterator_traits`.
     * @return iterator of the first element position
     */
    const_iterator begin() const noexcept {
        return const_iterator { entries_, 0 };
    }

    /**
     * @brief returns a sentinel iterator.
     * @return a sentinel iterator
     */
    const_iterator end() const noexcept {
        return const_iterator { entries_, entries_.size() };
    }

    /**
     * @brief returns the intersection of two sets.
     * @param a the first set
     * @param b the second set
     * @return the intersection
     */
    friend enum_set operator&(enum_set const& a, enum_set const& b) noexcept {
        return enum_set { a.entries_ & b.entries_ };
    }

    /**
     * @brief returns the union of two sets.
     * @param a the first set
     * @param b the second set
     * @return the union
     */
    friend enum_set operator|(enum_set const& a, enum_set const& b) noexcept {
        return enum_set { a.entries_ | b.entries_ };
    }

    /// @copydoc operator|()
    friend enum_set operator+(enum_set const& a, enum_set const& b) noexcept {
        return a | b;
    }

    /**
     * @brief returns the difference of two sets.
     * @param a the first set
     * @param b the second set
     * @return the difference
     */
    friend enum_set operator-(enum_set const& a, enum_set const& b) noexcept {
        return enum_set { a.entries_ & ~b.entries_ };
    }

    /**
     * @brief retains elements only in the given set.
     * @param other the target set
     * @return this
     */
    enum_set& operator&=(enum_set const& other) noexcept {
        entries_ &= other.entries_;
        return *this;
    }

    /**
     * @brief adds all elements in the given set into this set.
     * @param other the target set
     * @return this
     */
    enum_set& operator|=(enum_set const& other) noexcept {
        entries_ |= other.entries_;
        return *this;
    }

    /// @copydoc operator|=()
    enum_set& operator+=(enum_set const& other) noexcept {
        return operator|(other);
    }

    /**
     * @brief removes all elements in the given set from this set.
     * @param other the target set
     * @return this
     */
    enum_set& operator-=(enum_set const& other) noexcept {
        entries_ &= ~other.entries_;
        return *this;
    }

    /**
     * @brief returns whether or not the two sets contains same entries.
     * @param a the first value
     * @param b the second value
     * @return true if a = b
     * @return false otherwise
     */
    friend constexpr bool operator==(enum_set const& a, enum_set const& b) noexcept {
        return a.entries_ == b.entries_;
    }

    /**
     * @brief returns whether or not the two sets contains same entries.
     * @param a the first value
     * @param b the second value
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(enum_set const& a, enum_set const& b) noexcept {
        return !(a == b);
    }

    /**
     * @brief compares two sets in lexicographic order.
     * @param a the first value
     * @param b the second value
     * @return true if a < b
     * @return false otherwise
     */
    friend constexpr bool operator<(enum_set const& a, enum_set const& b) noexcept {
        return a.entries_ < b.entries_;
    }

    /**
     * @brief compares two sets in lexicographic order.
     * @param a the first value
     * @param b the second value
     * @return true if a > b
     * @return false otherwise
     */
    friend constexpr bool operator>(enum_set const& a, enum_set const& b) noexcept {
        return a.entries_ > b.entries_;
    }

    /**
     * @brief compares two sets in lexicographic order.
     * @param a the first value
     * @param b the second value
     * @return true if a <= b
     * @return false otherwise
     */
    friend constexpr bool operator<=(enum_set const& a, enum_set const& b) noexcept {
        return a.entries_ <= b.entries_;
    }

    /**
     * @brief compares two sets in lexicographic order.
     * @param a the first value
     * @param b the second value
     * @return true if a >= b
     * @return false otherwise
     */
    friend constexpr bool operator>=(enum_set const& a, enum_set const& b) noexcept {
        return a.entries_ >= b.entries_;
    }

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, enum_set const& value) {
        out << "{";
        bool cont = false;
        for (auto e : value) {
            if (cont) out << ", ";
            cont = true;
            out << e;
        }
        out << "}";
        return out;
    }

private:
    std::bitset<iterator::number_of_universe> entries_ {};

    explicit constexpr enum_set(std::bitset<iterator::number_of_universe> entries) noexcept
        : entries_(entries)
    {}

    static constexpr index_type to_index(value_type value) noexcept {
        return static_cast<index_type>(
                static_cast<std::underlying_type_t<value_type>>(value)
                - static_cast<std::underlying_type_t<value_type>>(First));
    }
};

} // namespace takatori::util
