#pragma once

#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/tree/tree_element_vector.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/reference_vector.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::scalar {

/**
 * @brief coalesce expression.
 */
class coalesce final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::coalesce;

    /**
     * @brief creates a new object.
     * @param alternatives the alternative expressions
     */
    explicit coalesce(util::reference_vector<expression> alternatives) noexcept;

    /**
     * @brief creates a new object.
     * @param alternatives the alternative expressions
     * @attention this may take copies of given expressions
     */
    explicit coalesce(std::initializer_list<util::rvalue_reference_wrapper<expression>> alternatives = {}); // NOLINT

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit coalesce(coalesce const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit coalesce(coalesce&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] coalesce* clone(util::object_creator creator) const& override;
    [[nodiscard]] coalesce* clone(util::object_creator creator) && override;

    /**
     * @brief returns the alternative expressions.
     * @return the alternative expressions
     */
    [[nodiscard]] tree::tree_element_vector<expression>& alternatives() noexcept;

    /// @copydoc alternatives()
    [[nodiscard]] tree::tree_element_vector<expression> const& alternatives() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(coalesce const& a, coalesce const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(coalesce const& a, coalesce const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, coalesce const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    tree::tree_element_vector<expression> alternatives_;
};

/**
 * @brief type_of for coalesce.
 */
template<> struct type_of<coalesce::tag> : util::meta_type<coalesce> {};

} // namespace takatori::scalar
