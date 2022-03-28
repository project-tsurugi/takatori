#pragma once

#include <functional>
#include <iostream>
#include <initializer_list>
#include <string_view>
#include <vector>

#include "identifier.h"

#include <takatori/document/region.h>

namespace takatori::name {

/**
 * @brief represents a symbol name.
 */
class name {
public:
    /// @brief the identifier segment type.
    using identifier_type = identifier;
    /// @brief the identifier sequence type.
    using sequence_type = std::vector<identifier_type>;

    /// @brief the value type
    using value_type = identifier_type;
    /// @brief the size type
    using size_type = std::size_t;
    /// @brief the difference type
    using difference_type = typename sequence_type::difference_type;
    /// @brief the L-value reference type
    using reference = typename sequence_type::reference;
    /// @brief the L-value reference type
    using const_reference = typename sequence_type::const_reference;
    /// @brief the pointer type
    using pointer = typename sequence_type::pointer;
    /// @brief the iterator type
    using iterator = typename sequence_type::const_iterator;

    /**
     * @brief creates an invalid identifier.
     */
    constexpr name() noexcept = default;

    /**
     * @brief creates a new instance.
     * @param identifiers the identifier sequence
     */
    name(std::vector<identifier_type> identifiers) noexcept; // NOLINT

    /**
     * @brief creates a new instance.
     * @param identifiers the identifier sequence
     */
    name(std::initializer_list<std::string_view> identifiers);

    /**
     * @brief returns the sequence of identifiers.
     * @return the identifier sequence
     * @return empty if this name is empty
     */
    [[nodiscard]] std::vector<identifier_type> const& identifiers() const noexcept;

    /**
     * @brief sets the sequence of identifiers.
     * @param identifiers the identifier sequence
     * @return this
     */
    name& identifiers(std::vector<identifier_type> identifiers) noexcept;

    /**
     * @brief returns the document region of this element.
     * @return the document region
     */
    [[nodiscard]] document::region& region() noexcept;

    /// @copydoc region()
    [[nodiscard]] document::region const& region() const noexcept;

    /**
     * @brief returns whether or not the identifier sequence is empty.
     * @return true the identifier sequence is empty
     * @return false otherwise
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief returns an element at the position.
     * @param position the element index on this sequence
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    [[nodiscard]] const_reference operator[](size_type position) const;

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] const_reference front() const;

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] const_reference back() const;

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] size_type size() const noexcept;

    /**
     * @brief returns a forward iterator which points the beginning of this sequence.
     * @return the iterator of beginning (inclusive)
     */
    [[nodiscard]] iterator begin() const noexcept;

    /**
     * @brief returns a forward iterator which points the ending of this sequence.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] iterator end() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed beginning of this sequence.
     * @return the reversed iterator of beginning (inclusive)
     */
    [[nodiscard]] std::reverse_iterator<iterator> rbegin() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed ending of this sequence.
     * @return the reversed iterator of ending (exclusive)
     */
    [[nodiscard]] std::reverse_iterator<iterator> rend() const noexcept;

    /**
     * @brief returns whether or not this represents a well-formed name.
     * @return true if the name is well-formed
     * @return false if the name is empty
     */
    [[nodiscard]] explicit operator bool() const noexcept;

private:
    std::vector<identifier_type> identifiers_ {};
    document::region region_ {};
};

/**
 * @brief returns whether or not the two names are equivalent.
 * @param a the first name
 * @param b the second name
 * @return true if they are equivalent
 * @return false otherwise
 */
bool operator==(name const& a, name const& b) noexcept;

/**
 * @brief returns whether or not the two names are equivalent.
 * @param a the first name
 * @param b the second name
 * @return true if they are equivalent
 * @return false otherwise
 */
bool operator==(name const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the two names are equivalent.
 * @param a the first name
 * @param b the second name
 * @return true if they are equivalent
 * @return false otherwise
 */
bool operator==(identifier const& a, name const& b) noexcept;

/**
 * @brief returns whether or not the two names are different.
 * @param a the first name
 * @param b the second name
 * @return true if they are different
 * @return false otherwise
 */
bool operator!=(name const& a, name const& b) noexcept;

/**
 * @brief returns whether or not the two names are different.
 * @param a the first name
 * @param b the second name
 * @return true if they are different
 * @return false otherwise
 */
bool operator!=(name const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the two names are different.
 * @param a the first name
 * @param b the second name
 * @return true if they are different
 * @return false otherwise
 */
bool operator!=(identifier const& a, name const& b) noexcept;

/**
 * @brief prints information of the given element into the output stream.
 * @param out the target output stream
 * @param value the target element
 * @return the written output stream
 */
std::ostream& operator<<(std::ostream& out, name const& value);

} // namespace takatori::name

/**
 * @brief std::hash specialization for takatori::name::identifier.
 */
template<>
struct std::hash<takatori::name::name> {
    /**
     * @brief compute hash of the given object.
     * @param v the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::name::name const& v) const noexcept;
};
