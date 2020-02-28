#pragma once

#include <iostream>
#include <utility>

namespace takatori::document {

/**
 * @brief a position on the document.
 */
class position {
public:
    /// @brief the unknown position.
    static inline constexpr std::size_t unknown = 0;

    /**
     * @brief creates a new instance, which represents "unknown position".
     */
    constexpr position() = default;

    /**
     * @brief creates a new instance.
     * @param line_number the 1-origin line number, or 0 represents "unknown line number"
     * @param column_number the 1-origin column number, or 0 represents "unknown column number"
     */
    constexpr position(std::size_t line_number, std::size_t column_number) noexcept
        : line_number_(line_number)
        , column_number_(line_number == unknown ? unknown : column_number)
    {}

    /**
     * @brief returns the line number.
     * @return the line number
     * @return unknown if it is not clear
     */
    constexpr std::size_t line_number() const noexcept {
        return line_number_;
    }

    /**
     * @brief returns the column number.
     * @return the column number
     * @return unknown if it is not clear
     */
    constexpr std::size_t column_number() const noexcept {
        return column_number_;
    }

    /**
     * @brief returns whether or not the line number is known.
     * @return true if the line number is valid
     * @return false if the line number is unknown
     */
    explicit constexpr operator bool() const noexcept {
        return line_number_ != unknown;
    }

private:
    std::size_t line_number_ {};
    std::size_t column_number_ {};
};

/**
 * @brief returns whether or not the two positions are equivalent.
 * @param a the first position
 * @param b the second position
 * @return
 */
inline constexpr bool operator==(position const& a, position const& b) noexcept {
    return a.line_number() == b.line_number()
        && a.column_number() == b.column_number();
}

/**
 * @brief returns whether or not the two positions are different.
 * @param a the first position
 * @param b the second position
 * @return
 */
inline constexpr bool operator!=(position const& a, position const& b) noexcept {
    return !(a == b);
}

/**
 * @brief returns whether or not the first position is less than the second position.
 * @note the unknown position is less than the all other known positions
 * @param a the first position
 * @param b the second position
 * @return true if a < b
 * @return false otherwise
 */
inline constexpr bool operator<(position const& a, position const& b) noexcept {
    if (a.line_number() < b.line_number()) {
        return true;
    }
    if (a.line_number() > b.line_number()) {
        return false;
    }
    return a.column_number() < b.column_number();
}

/**
 * @brief returns whether or not the first position is greater than the second position.
 * @note the unknown position is less than the all other known positions
 * @param a the first position
 * @param b the second position
 * @return true if a > b
 * @return false otherwise
 */
inline constexpr bool operator>(position const& a, position const& b) noexcept {
    return b < a;
}

/**
 * @brief returns whether or not the first position is less than or equal to the second position.
 * @note the unknown position is less than the all other known positions
 * @param a the first position
 * @param b the second position
 * @return true if a <= b
 * @return false otherwise
 */
inline constexpr bool operator<=(position const& a, position const& b) noexcept {
    return !(b < a);
}

/**
 * @brief returns whether or not the first position is greater than or equal to the second position.
 * @note the unknown position is less than the all other known positions
 * @param a the first position
 * @param b the second position
 * @return true if a >= b
 * @return false otherwise
 */
inline constexpr bool operator>=(position const& a, position const& b) noexcept {
    return !(a < b);
}

/**
 * @brief prints information of the given document into the output stream.
 * @param out the target output stream
 * @param value the target document
 * @return the written output stream
 */
inline std::ostream& operator<<(std::ostream& out, position const& value) {
    if (auto ln = value.line_number(); ln == position::unknown) {
        out << "unknown";
    } else {
        out << ln;
        if (auto cn = value.column_number(); cn != position::unknown) {
            out << ":" << cn;
        }
    }
    return out;
}

} // namespace takatori::document
