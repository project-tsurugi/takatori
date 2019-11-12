#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace takatori::scalar {

/**
 * @brief represents kind of expression.
 */
enum class expression_kind {

    // primary expressions

    /// @brief immediate expression.
    immediate,

    /// @brief a reference of variable.
    variable_reference,

    // unary expressions

    /// @brief standard unary expression.
    unary,

    /// @brief type casting.
    cast,

    // binary expressions

    /// @brief standard binary expression.
    binary,

    /// @brief comparison expression.
    compare,

    // complex expressions

    /// @brief pattern matching expression.
    match,

    /// @brief conditional expression (a.k.a. case expression).
    conditional,

    /// @brief coalesce expression.
    coalesce,

    /// @brief expression with local variable declarations.
    let,

    /// @brief function call expression.
    function_call,

    // for complex types

    /// @brief array construction expression.
    array_construct, // FIXME: impl

    /// @brief array length expression.
    array_length, // FIXME: impl

    /// @brief array element extraction expression.
    array_get, // FIXME: impl

    /// @brief array element comparison with quantification expresion.
    array_compare_quantification, // FIXME: impl

    /// @brief record construction expression.
    record_construct, // FIXME: impl

    /// @brief record element extraction expression.
    record_get, // FIXME: impl

    // special expressions

    /// @brief custom expression for compiler or third party extension.
    extension, // FIXME: impl
};

/**
 * @brief provides implementation type for the expression_kind.
 * @tparam Kind the expression kind
 */
template<expression_kind Kind> struct type_of {};

/// @copydoc type_of
template<expression_kind Kind> using type_of_t = typename type_of<Kind>::type;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(expression_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = expression_kind;
    switch (value) {
        case kind::immediate: return "immediate"sv;
        case kind::variable_reference: return "variable_reference"sv;
        case kind::unary: return "unary"sv;
        case kind::cast: return "cast"sv;
        case kind::binary: return "binary"sv;
        case kind::compare: return "compare"sv;
        case kind::match: return "match"sv;
        case kind::conditional: return "conditional"sv;
        case kind::coalesce: return "coalesce"sv;
        case kind::let: return "let"sv;
        case kind::function_call: return "function_call"sv;
        case kind::array_construct: return "array_construct"sv;
        case kind::array_length: return "array_length"sv;
        case kind::array_get: return "array_get"sv;
        case kind::array_compare_quantification: return "array_compare_quantification"sv;
        case kind::record_construct: return "record_construct"sv;
        case kind::record_get: return "record_get"sv;
        case kind::extension: return "extension"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, expression_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::scalar
