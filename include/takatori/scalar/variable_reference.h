#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::scalar {

/**
 * @brief variable reference expression.
 */
class variable_reference final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::variable_reference;

    /**
     * @brief creates a new object.
     * @param variable the descriptor of the target variable
     */
    explicit variable_reference(descriptor::variable variable) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit variable_reference(util::clone_tag_t, variable_reference const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit variable_reference(util::clone_tag_t, variable_reference&& other) noexcept;

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] variable_reference* clone() const& override;
    [[nodiscard]] variable_reference* clone() && override;

    /**
     * @brief returns the descriptor of target variable.
     * @return the destination type
     */
    [[nodiscard]] descriptor::variable& variable() noexcept;

    /// @copydoc variable()
    [[nodiscard]] descriptor::variable const& variable() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(variable_reference const& a, variable_reference const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(variable_reference const& a, variable_reference const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, variable_reference const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::variable variable_;
};

/**
 * @brief type_of for variable_reference.
 */
template<> struct type_of<variable_reference::tag> : util::meta_type<variable_reference> {};

} // namespace takatori::scalar
