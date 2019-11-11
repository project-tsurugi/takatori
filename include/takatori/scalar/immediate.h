#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "expression_traits.h"

#include "takatori/descriptor/element_descriptor.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::scalar {

/**
 * @brief immediate expression.
 */
class immediate final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::immediate;

    immediate() = delete;
    ~immediate() override = default;
    immediate(immediate const& other) = delete;
    immediate& operator=(immediate const& other) = delete;
    immediate(immediate&& other) noexcept = delete;
    immediate& operator=(immediate&& other) noexcept = delete;

    /**
     * @brief creates a new object.
     * @param value the descriptor of the immediate value
     * @param type the value type
     */
    immediate(
            descriptor::value_descriptor value,
            descriptor::type_descriptor type) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    immediate(immediate const& other, util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    immediate(immediate&& other, util::object_creator creator) noexcept;

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;;

    expression_kind kind() const noexcept override;
    immediate* clone(util::object_creator creator) const& override;
    immediate* clone(util::object_creator creator) && override;

    /**
     * @brief returns the descriptor of indicating value.
     * @return the value
     */
    descriptor::value_descriptor const& value() const noexcept;

    /**
     * @brief sets a descriptor of value.
     * @param value the value
     * @return this
     */
    immediate& value(descriptor::value_descriptor value) noexcept;

    /**
     * @brief returns the descriptor of value type.
     * @return the type
     */
    descriptor::type_descriptor const& type() const noexcept;

    /**
     * @brief sets a descriptor of value type.
     * @param type the type
     * @return this
     */
    immediate& type(descriptor::type_descriptor type) noexcept;

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
    friend inline bool operator!=(immediate const& a, immediate const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, immediate const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::value_descriptor value_;
    descriptor::type_descriptor type_;
    parent_type* parent_ {};
};

/**
 * @brief expression_kind_type for immediate.
 */
template<> struct expression_kind_type<immediate::tag> : util::meta_type<immediate> {};

} // namespace takatori::scalar
