#pragma once

#include <optional>

#include "type_kind.h"
#include "data.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief decimal number type.
 */
class decimal : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline type_kind tag = type_kind::decimal;

    /// @brief the size type.
    using size_type = std::size_t;

    /**
     * @brief creates a new instance.
     * @param precision the max number of decimal digits in integral and fractional part,
     *                  or empty for flexible size decimals
     * @param scale the number of decimal digits in fractional part,
     *              which must be less than or equal to precision if it is defined
     */
    explicit constexpr decimal(std::optional<size_type> precision = {}, size_type scale = 0) noexcept;

    ~decimal() override = default;
    decimal(decimal const& other) = delete;
    decimal& operator=(decimal const& other) = delete;
    decimal(decimal&& other) noexcept = delete;
    decimal& operator=(decimal&& other) noexcept = delete;

    type_kind kind() const noexcept override;
    decimal* clone(util::object_creator creator) const& override;
    decimal* clone(util::object_creator creator) && override;

    /**
     * @brief returns the max number of decimal digits in integral and fractional part.
     * @return the max number of digits
     * @return empty if it is not defined
     */
    constexpr std::optional<size_type> precision() const noexcept;

    /**
     * @brief returns the number of digits in the fractional part.
     * @return the number of digits in the fractional part
     */
    constexpr size_type scale() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(decimal const& a, decimal const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(decimal const& a, decimal const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, decimal const& value);

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;
    
private:
    std::optional<size_type> precision_;
    size_type scale_;
};

constexpr
decimal::decimal(std::optional<size_type> precision, decimal::size_type scale) noexcept
    : precision_(std::move(precision))
    , scale_(scale)
{}

constexpr std::optional<decimal::size_type> decimal::precision() const noexcept {
    return precision_;
}

constexpr decimal::size_type
decimal::scale() const noexcept {
    return scale_;
}

template<> struct type_of<decimal::tag> : util::meta_type<decimal> {};

} // namespace takatori::type

namespace std {

/// @brief provides hash code of takatori::type::decimal.
template<> struct hash<takatori::type::decimal> : hash<takatori::type::data> {};

} // namespace std
