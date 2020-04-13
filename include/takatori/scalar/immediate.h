#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/type/data.h>
#include <takatori/value/data.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::scalar {

/**
 * @brief immediate expression.
 */
class immediate final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::immediate;

    /**
     * @brief creates a new object.
     * @param value the immediate value
     * @param type the value type
     */
    explicit immediate(
            std::shared_ptr<value::data const> value,
            std::shared_ptr<type::data const> type) noexcept;

    /**
     * @brief creates a new object.
     * @param value the immediate value
     * @param type the value type
     * @attention this may take a copy of given value and type
     */
    explicit immediate(
            value::data&& value,
            type::data&& type);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit immediate(immediate const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit immediate(immediate&& other, util::object_creator creator) noexcept;

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] immediate* clone(util::object_creator creator) const& override;
    immediate* clone(util::object_creator creator) && override;

    /**
     * @brief returns the immediate value.
     * @return the immediate value
     * @warning undefined behavior if the value is absent
     */
    [[nodiscard]] value::data const& value() const noexcept;

    /**
     * @brief returns the immediate value.
     * @return the immediate value
     * @return empty if the value is absent
     */
    [[nodiscard]] util::optional_ptr<value::data const> optional_value() const noexcept;

    /**
     * @brief returns the immediate value for share its value.
     * @return the immediate value for sharing
     * @return empty if the value is absent
     */
    [[nodiscard]] std::shared_ptr<value::data const> shared_value() const noexcept;

    /**
     * @brief sets a immediate value.
     * @param value the immediate value
     * @return this
     */
    immediate& value(std::shared_ptr<value::data const> value) noexcept;

    /**
     * @brief returns the value type.
     * @return the value type
     * @warning undefined behavior if the type is absent
     */
    [[nodiscard]] type::data const& type() const noexcept;

    /**
     * @brief returns the value type.
     * @return the value type
     * @return empty if the type is absent
     */
    [[nodiscard]] util::optional_ptr<type::data const> optional_type() const noexcept;

    /**
     * @brief returns the value type for share its type.
     * @return the value type for sharing
     * @return empty if the type is absent
     */
    [[nodiscard]] std::shared_ptr<type::data const> shared_type() const noexcept;

    /**
     * @brief sets a value type.
     * @param type the value type
     * @return this
     */
    immediate& type(std::shared_ptr<type::data const> type) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(immediate const& a, immediate const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(immediate const& a, immediate const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, immediate const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::shared_ptr<value::data const> value_;
    std::shared_ptr<type::data const> type_;
};

/**
 * @brief type_of for immediate.
 */
template<> struct type_of<immediate::tag> : util::meta_type<immediate> {};

} // namespace takatori::scalar
