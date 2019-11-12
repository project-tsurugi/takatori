#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "cast_loss_policy.h"

#include "takatori/type/data.h"

#include "takatori/util/enum_tag.h"
#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::scalar {

/**
 * @brief type casting expression.
 */
class cast final : public expression {
public:
    /// @brief policy type of lossy operation.
    using loss_policy_type = cast_loss_policy;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::cast;

    cast() = delete;
    ~cast() override = default;
    cast(cast const& other) = delete;
    cast& operator=(cast const& other) = delete;
    cast(cast&& other) noexcept = delete;
    cast& operator=(cast&& other) noexcept = delete;

    /**
     * @brief creates a new object.
     * @param type the destination type
     * @param loss_policy the loss policy
     * @param operand the operand
     */
    explicit cast(
            std::shared_ptr<type::data const> type,
            loss_policy_type loss_policy,
            util::unique_object_ptr<expression> operand) noexcept;

    /**
     * @brief creates a new object.
     * @param type the destination type
     * @param loss_policy the loss policy
     * @param operand the operand
     * @attention this may take copies of given type and expression
     */
    cast(
            type::data&& type,
            loss_policy_type loss_policy,
            expression&& operand);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    cast(cast const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    cast(cast&& other, util::object_creator creator);

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;

    expression_kind kind() const noexcept override;
    cast* clone(util::object_creator creator) const& override;
    cast* clone(util::object_creator creator) && override;

    /**
     * @brief returns the destination type.
     * @return the destination type
     * @warning undefined behavior if the type is absent
     */
    type::data const& type() const noexcept;

    /**
     * @brief returns the destination type.
     * @return the destination type
     * @return empty if the type is absent
     */
    util::optional_ptr<type::data const> optional_type() const noexcept;

    /**
     * @brief returns the destination type for share its type.
     * @return the destination type for sharing
     * @return empty if the type is absent
     */
    std::shared_ptr<type::data const> shared_type() const noexcept;

    /**
     * @brief sets a destination type.
     * @param type the destination type
     * @return this
     */
    cast& type(std::shared_ptr<type::data const> type) noexcept;

    /**
     * @brief returns the action kind for decrease accuracy.
     * @return policy of lossy operation
     */
    loss_policy_type loss_policy() const noexcept;

    /**
     * @brief sets a policy of lossy operation.
     * @param loss_policy the policy
     * @return this
     */
    cast& loss_policy(loss_policy_type loss_policy) noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @warning undefined behavior if the operand is absent
     */
    expression& operand() noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @warning undefined behavior if the operand is absent
     */
    expression const& operand() const noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    util::optional_ptr<expression> optional_operand() noexcept;

    /// @copydoc optional_operand()
    util::optional_ptr<expression const> optional_operand() const noexcept;

    /**
     * @brief releases the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    util::unique_object_ptr<expression> release_operand() noexcept;

    /**
     * @brief sets the expression operand.
     * @param operand the replacement
     * @return this
     */
    cast& operand(util::unique_object_ptr<expression> operand) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(cast const& a, cast const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(cast const& a, cast const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, cast const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::shared_ptr<type::data const> type_;
    loss_policy_type loss_policy_;
    util::unique_object_ptr<expression> operand_;
    parent_type* parent_ {};
};

/**
 * @brief type_of for cast.
 */
template<> struct type_of<cast::tag> : util::meta_type<cast> {};

} // namespace takatori::scalar
