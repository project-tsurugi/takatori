#pragma once

#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/tree/tree_element_vector.h>
#include <takatori/descriptor/function.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/reference_vector.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::scalar {

/**
 * @brief function call expression.
 */
class function_call final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::function_call;

    /**
     * @brief creates a new object.
     * @tparam Copier the object copying policy (don't care)
     * @param function the descriptor of the target function
     * @param arguments the argument expressions
     */
    template<class Copier = util::clonable_copier<expression>>
    explicit function_call(
            descriptor::function function,
            util::reference_vector<expression, Copier> arguments) noexcept
        : function_(std::move(function))
        , arguments_(*this, std::move(arguments)) {}

    /**
     * @brief creates a new object.
     * @param function the descriptor of the target function
     * @param arguments the argument expressions
     * @attention this may take copies of given expressions
     */
    explicit function_call(
            descriptor::function function,
            std::initializer_list<util::rvalue_reference_wrapper<expression>> arguments = {});

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit function_call(function_call const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit function_call(function_call&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] function_call* clone(util::object_creator creator) const& override;
    function_call* clone(util::object_creator creator) && override;

    /**
     * @brief returns the descriptor of target function.
     * @return the destination type
     */
    descriptor::function& function() noexcept;

    /// @copydoc function()
    [[nodiscard]] descriptor::function const& function() const noexcept;

    /**
     * @brief returns the function arguments.
     * @return the function arguments
     */
    tree::tree_element_vector<expression>& arguments() noexcept;

    /// @copydoc arguments()
    [[nodiscard]] tree::tree_element_vector<expression> const& arguments() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(function_call const& a, function_call const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(function_call const& a, function_call const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, function_call const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    descriptor::function function_;
    tree::tree_element_vector<expression> arguments_;
};

/**
 * @brief type_of for function_call.
 */
template<> struct type_of<function_call::tag> : util::meta_type<function_call> {};

} // namespace takatori::scalar
