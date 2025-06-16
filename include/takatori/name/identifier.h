#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

#include <takatori/document/region.h>

namespace takatori::name {

/**
 * @brief represents an identifier.
 */
class identifier {
public:
    /// @brief the token representation type.
    using token_type = std::string;

    /**
     * @brief creates an invalid identifier.
     */
    constexpr identifier() noexcept = default;

    /**
     * @brief creates a new instance.
     * @param token the token
     */
    identifier(token_type token) noexcept; // NOLINT

    /**
     * @brief creates a new instance from the string_view like value.
     * @tparam StringViewLike the type that accepts `std::string { it }`
     * @param token the token
     */
    template<
            class StringViewLike,
            class = std::enable_if_t<std::is_constructible_v<token_type, StringViewLike>, StringViewLike>> // NOLINT(*-avoid-c-arrays)
    identifier(StringViewLike const& token) : // NOLINT
        token_ { token }
    {}
    /**
     * @brief returns the token of this identifier.
     * @return the token
     * @return empty if this identifier is not valid.
     */
    [[nodiscard]] token_type const& token() const noexcept;

    /**
     * @brief sets the token of this identifier.
     * @param token the new token
     * @return this
     */
    identifier& token(token_type token) noexcept;

    /**
     * @brief returns the document region of this element.
     * @return the document region
     */
    [[nodiscard]] document::region& region() noexcept;

    /// @copydoc region()
    [[nodiscard]] document::region const& region() const noexcept;

    /**
     * @brief returns whether or not a token is present.
     * @return true if a token is present
     * @return false if otherwise
     */
    [[nodiscard]] explicit operator bool() const noexcept;

private:
    token_type token_ {};
    document::region region_ {};
};

/**
 * @brief returns whether or not the two identifiers are equivalent.
 * @param a the first identifier
 * @param b the second identifier
 * @return true if they are equivalent
 * @return false otherwise
 */
bool operator==(identifier const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the two identifiers are different.
 * @param a the first identifier
 * @param b the second identifier
 * @return true if they are different
 * @return false otherwise
 */
bool operator!=(identifier const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the first identifier is less than the second identifier.
 * @param a the first identifier
 * @param b the second identifier
 * @return true if a < b
 * @return false otherwise
 */
bool operator<(identifier const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the first identifier is greater than the second identifier.
 * @param a the first identifier
 * @param b the second identifier
 * @return true if a > b
 * @return false otherwise
 */
bool operator>(identifier const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the first identifier is less than or equal to the second identifier.
 * @note the unknown identifier is less than the all other known identifiers
 * @param a the first identifier
 * @param b the second identifier
 * @return true if a <= b
 * @return false otherwise
 */
bool operator<=(identifier const& a, identifier const& b) noexcept;

/**
 * @brief returns whether or not the first identifier is greater than or equal to the second identifier.
 * @note the unknown identifier is less than the all other known identifiers
 * @param a the first identifier
 * @param b the second identifier
 * @return true if a >= b
 * @return false otherwise
 */
bool operator>=(identifier const& a, identifier const& b) noexcept;

/**
 * @brief prints information of the given element into the output stream.
 * @param out the target output stream
 * @param value the target element
 * @return the written output stream
 */
std::ostream& operator<<(std::ostream& out, identifier const& value);

} // namespace takatori::name

/**
 * @brief std::hash specialization for takatori::name::identifier.
 */
template<>
struct std::hash<takatori::name::identifier> {
    /**
     * @brief compute hash of the given object.
     * @param v the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::name::identifier const& v) const noexcept;
};
