#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace takatori::type {

/**
 * @brief represents kind of data type.
 */
enum class data_type_kind {
    // built-in types

    /// @brief boolean type.
    boolean,
    /// @brief 1-octet integral number type.
    int1,
    /// @brief 2-octets integral number type.
    int2,
    /// @brief 4-octets integral number type.
    int4,
    /// @brief 8-octets integral number type.
    int8,
    /// @brief 4-octets floating point number type.
    float4,
    /// @brief 8-octets floating point number type.
    float8,
    /// @brief decimal number type.
    decimal,
    /// @brief character type.
    character,
    /// @brief bit type.
    bit,
    /// @brief date type.
    date,
    /// @brief time of day type.
    time_of_day,
    /// @brief time point type.
    time_point,
    /// @brief time interval type.
    time_interval,

    // complex types

    /// @brief array type.
    array, // FIXME: impl
    /// @brief record type.
    record, // FIXME: impl

    // special types
    /**
     * @brief user defined type.
     * @attention this must not appear on runtime.
     */
    user, // FIXME: impl

    /// @brief unknown type, including nulls.
    unknown,

    /// @brief reference to table entry.
    row_reference, // FIXME: impl

    /// @brief row ID type.
    row_id, // FIXME: impl

    /**
     * @brief unresolved type, for placeholder values.
     * @attention this must not appear on runtime.
     */
    unresolved, // FIXME: impl

    /**
     * @brief compile error type.
     * @attention this must not appear on runtime.
     */
    error, // FIXME: impl
};

/**
 * @brief provides implementation type for the data_type_kind.
 * @tparam Kind the expression kind
 */
template<data_type_kind Kind> struct data_type_kind_type {};

/// @copydoc data_type_kind_type
template<data_type_kind Kind> using data_type_kind_type_t = typename data_type_kind_type<Kind>::type;

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(data_type_kind value) noexcept {
    using namespace std::string_view_literals;
    using kind = data_type_kind;
    switch (value) {
        case kind::boolean: return "bool"sv;
        case kind::int1: return "int1"sv;
        case kind::int2: return "int2"sv;
        case kind::int4: return "int4"sv;
        case kind::int8: return "int8"sv;
        case kind::float4: return "float4"sv;
        case kind::float8: return "float8"sv;
        case kind::decimal: return "decimal"sv;
        case kind::character: return "character"sv;
        case kind::bit: return "bit"sv;
        case kind::date: return "date"sv;
        case kind::time_of_day: return "time_of_day"sv;
        case kind::time_point: return "time_point"sv;
        case kind::time_interval: return "time_interval"sv;
        case kind::array: return "array"sv;
        case kind::record: return "record"sv;
        case kind::user: return "user"sv;
        case kind::unknown: return "unknown"sv;
        case kind::row_reference: return "row_reference"sv;
        case kind::row_id: return "row_id"sv;
        case kind::unresolved: return "unresolved"sv;
        case kind::error: return "error"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, data_type_kind value) {
    return out << to_string_view(value);
}

} // namespace takatori::type
