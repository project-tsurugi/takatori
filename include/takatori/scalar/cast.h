#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "cast_loss_policy.h"

#include <takatori/type/data.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/enum_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/ownership_reference.h>

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

    /**
     * @brief creates a new object.
     * @param type the destination type
     * @param loss_policy the loss policy
     * @param operand the operand
     */
    explicit cast(
            std::shared_ptr<type::data const> type,
            loss_policy_type loss_policy,
            std::unique_ptr<expression> operand) noexcept;

    /**
     * @brief creates a new object.
     * @param type the destination type
     * @param loss_policy the loss policy
     * @param operand the operand
     * @attention this may take copies of given type and expression
     */
    explicit cast(
            type::data&& type,
            loss_policy_type loss_policy,
            expression&& operand);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit cast(util::clone_tag_t, cast const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit cast(util::clone_tag_t, cast&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] cast* clone() const& override;
    [[nodiscard]] cast* clone() && override;

    /**
     * @brief returns the destination type.
     * @return the destination type
     * @warning undefined behavior if the type is absent
     */
    [[nodiscard]] type::data const& type() const noexcept;

    /**
     * @brief returns the destination type.
     * @return the destination type
     * @return empty if the type is absent
     */
    [[nodiscard]] util::optional_ptr<type::data const> optional_type() const noexcept;

    /**
     * @brief returns the destination type for share its type.
     * @return the destination type for sharing
     * @return empty if the type is absent
     */
    [[nodiscard]] std::shared_ptr<type::data const> shared_type() const noexcept;

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
    [[nodiscard]] loss_policy_type loss_policy() const noexcept;

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
    [[nodiscard]] expression const& operand() const noexcept;

    /**
     * @brief returns the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_operand() noexcept;

    /// @copydoc optional_operand()
    [[nodiscard]] util::optional_ptr<expression const> optional_operand() const noexcept;

    /**
     * @brief releases the expression operand.
     * @return the expression operand
     * @return empty if the operand is absent
     */
    [[nodiscard]] std::unique_ptr<expression> release_operand() noexcept;

    /**
     * @brief sets the expression operand.
     * @param operand the replacement
     * @return this
     */
    cast& operand(std::unique_ptr<expression> operand) noexcept;

    /**
     * @brief returns ownership reference of the expression operand.
     * @return the expression operand
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_operand();

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
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::shared_ptr<type::data const> type_;
    loss_policy_type loss_policy_;
    std::unique_ptr<expression> operand_;
};

/**
 * @brief type_of for cast.
 */
template<> struct type_of<cast::tag> : util::meta_type<cast> {};

} // namespace takatori::scalar
