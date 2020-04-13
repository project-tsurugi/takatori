#pragma once

#include <iostream>
#include <string_view>

#include <cstdint>

#include <fpdecimal/decimal.h>

#include "value_kind.h"
#include "simple_value.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::value {

/// @brief decimal value type.
using decimal_t = fpdecimal::Decimal;

/**
 * @brief represents decimal value as polymorphic value model.
 */
class decimal : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline value_kind tag = value_kind::decimal;

    /// @brief the entity type
    using entity_type = decimal_t;

    /// @brief the view type
    using view_type = decimal_t;

    /**
     * @brief creates a new instance.
     * @param value the decimal value
     */
    explicit decimal(entity_type value) noexcept;

    ~decimal() override = default;
    decimal(decimal const& other) = delete;
    decimal& operator=(decimal const& other) = delete;
    decimal(decimal&& other) noexcept = delete;
    decimal& operator=(decimal&& other) noexcept = delete;

    [[nodiscard]] value_kind kind() const noexcept override;
    [[nodiscard]] decimal* clone(util::object_creator creator) const& override;
    decimal* clone(util::object_creator creator) && override;

    /**
     * @brief returns the entity value.
     * @return the entity value
     */
    [[nodiscard]] view_type get() const noexcept;

    /// @copydoc get()
    explicit operator view_type() const noexcept;

protected:
    [[nodiscard]] bool equals(data const& other) const noexcept override;
    [[nodiscard]] std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;

    friend class util::object_creator;
};

/**
 * @brief returns whether or not the two elements are identical.
 * @note this tests identity of decimal values.
 * For examples, NaN is identical to the same NaN even if they are not equivalent,
 * and 0.1 is not identical to 0.10 but they are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a is identical to b
 * @return false otherwise
 */
bool operator==(decimal const& a, decimal const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a is not identical to b
 * @return false otherwise
 */
bool operator!=(decimal const& a, decimal const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, decimal const& value);

/**
 * @brief type_of for decimal.
 */
template<> struct type_of<decimal::tag> : util::meta_type<decimal> {};

} // namespace takatori::value

/// @brief provides hash code of takatori::value::decimal.
template<> struct std::hash<takatori::value::decimal> : hash<takatori::value::data> {};
