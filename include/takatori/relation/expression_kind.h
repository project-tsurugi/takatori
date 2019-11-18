#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace takatori::relation {

/**
 * @brief represents kind of relational expression.
 */
enum class expression_kind {

    // scan
    scan_full,
    scan_point,
    scan_prefix,
    scan_range,

    // join
    logical_join_map,
    logical_join_loop,
    logical_join_merge,

    // join with scan
    join_scan_point,
    join_scan_prefix,
    join_scan_range,

    // tuple by tuple operations
    select,
    where,
    buffer,

    // logical grouping operations
    logical_aggregate,
    logical_distinct,
    logical_limit,

    // logical set operations
    logical_intersect,
    logical_difference,

    // DML
    emit,
    logical_emit_sorted,
    insert,
    update,
    delete_,
    overwrite,

    // misc.
    logical_union,
    logical_escape,

    // join (on step execution plan)
    join_broadcast_map,
    join_broadcast_loop,
    join_group,

    // grouping  (on step execution plan)
    aggregate_group,
    distinct_group,
    limit_group,
    intersect_group,
    difference_group,
    flatten,

    // communicate with exchange operators
    take_flat,
    take_group,
    take_cogroup,
    offer,

    // FIXME: impl

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
        case kind::scan_full: return "scan_full"sv;
        case kind::scan_point: return "scan_point"sv;
        case kind::scan_prefix: return "scan_prefix"sv;
        case kind::scan_range: return "scan_range"sv;
        case kind::logical_join_map: return "logical::join_map"sv;
        case kind::logical_join_loop: return "logical::join_loop"sv;
        case kind::logical_join_merge: return "logical::join_merge"sv;
        case kind::join_scan_point: return "join_scan_point"sv;
        case kind::join_scan_prefix: return "join_scan_prefix"sv;
        case kind::join_scan_range: return "join_scan_range"sv;
        case kind::select: return "select"sv;
        case kind::where: return "where"sv;
        case kind::buffer: return "buffer"sv;
        case kind::logical_aggregate: return "logical::aggregate"sv;
        case kind::logical_distinct: return "logical::distinct"sv;
        case kind::logical_limit: return "logical::limit"sv;
        case kind::logical_intersect: return "logical::intersect"sv;
        case kind::logical_difference: return "logical::difference"sv;
        case kind::emit: return "emit"sv;
        case kind::logical_emit_sorted: return "logical::emit_sorted"sv;
        case kind::insert: return "insert"sv;
        case kind::update: return "update"sv;
        case kind::delete_: return "delete_"sv;
        case kind::overwrite: return "overwrite"sv;
        case kind::logical_union: return "logical::union"sv;
        case kind::logical_escape: return "logical::escape"sv;
        case kind::join_broadcast_map: return "join_broadcast_map"sv;
        case kind::join_broadcast_loop: return "join_broadcast_loop"sv;
        case kind::join_group: return "join_group"sv;
        case kind::aggregate_group: return "aggregate_group"sv;
        case kind::distinct_group: return "distinct_group"sv;
        case kind::limit_group: return "limit_group"sv;
        case kind::intersect_group: return "intersect_group"sv;
        case kind::difference_group: return "difference_group"sv;
        case kind::flatten: return "flatten"sv;
        case kind::take_flat: return "take_flat"sv;
        case kind::take_group: return "take_group"sv;
        case kind::take_cogroup: return "take_cogroup"sv;
        case kind::offer: return "offer"sv;
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

} // namespace takatori::relation
