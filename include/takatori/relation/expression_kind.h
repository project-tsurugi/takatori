#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <utility>

#include <cstdlib>

#include <takatori/util/enum_set.h>

namespace takatori::relation {

/**
 * @brief represents kind of relational expression.
 */
enum class expression_kind {
    // scan
    /// @brief obtains rows from an external relation by their key.
    find,
    /// @brief obtains rows an external relation by their key range.
    scan,

    // join
    /// @brief joins relations.
    join_relation,
    /// @brief joins input with rows in storage.
    join_find,
    /// @brief join input with rows in storage.
    join_scan,

    // tuple by tuple operations
    /// @brief adds columns into relation.
    project,
    /// @brief removes rows from relation.
    filter,
    /// @brief distribute copy of relation into the multiple successors.
    buffer,
    /// @brief adds a row ID column into relation.
    identify,

    // grouping operations for whole relation
    /// @brief aggregates columns in relation.
    aggregate_relation,
    /// @brief removes duplicated rows in relation.
    distinct_relation,
    /// @brief limits the number of rows in relation or its partitions
    limit_relation,

    // set operations for whole relation
    /// @brief obtains a multi-set union between two input relations.
    union_relation,
    /// @brief obtains a multi-set intersection of two input relations.
    intersection_relation,
    /// @brief obtains a multi-set difference between two input relations.
    difference_relation,

    // DML
    /// @brief emits rows in relation.
    emit,
    /// @brief edits rows on the table.
    write,

    // misc.
    /// @brief constructs relation from 2-dimensional scalar expressions.
    values,
    /// @brief re-defines all column identifiers in relations.
    escape,

    // dealing with group relations
    /// @brief join groups.
    join_group,
    /// @brief aggregates columns in group.
    aggregate_group,
    /// @brief obtains intersection of two groups.
    intersection_group,
    /// @brief obtains difference of two groups.
    difference_group,
    /// @brief flattens groups into rows.
    flatten_group,

    // communicate with exchange operators
    /// @brief obtains rows from an exchange.
    take_flat,
    /// @brief obtains groups from an exchange.
    take_group,
    /// @brief obtains groups from two or more exchanges.
    take_cogroup,
    /// @brief puts rows into an exchange.
    offer,

    /// @brief custom expression for compiler or third party extension.
    extension, // FIXME: impl
};

/// @brief a set of expression_kind.
using expression_kind_set = util::enum_set<
        expression_kind,
        expression_kind::find,
        expression_kind::extension>;

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
        case kind::find: return "find"sv;
        case kind::scan: return "scan"sv;
        case kind::join_relation: return "join_relation"sv;
        case kind::join_find: return "join_find"sv;
        case kind::join_scan: return "join_scan"sv;
        case kind::project: return "project"sv;
        case kind::filter: return "filter"sv;
        case kind::buffer: return "buffer"sv;
        case kind::identify: return "identify"sv;
        case kind::aggregate_relation: return "aggregate_relation"sv;
        case kind::distinct_relation: return "distinct_relation"sv;
        case kind::limit_relation: return "limit_relation"sv;
        case kind::union_relation: return "union_relation"sv;
        case kind::intersection_relation: return "intersection_relation"sv;
        case kind::difference_relation: return "difference_relation"sv;
        case kind::emit: return "emit"sv;
        case kind::write: return "write"sv;
        case kind::values: return "values"sv;
        case kind::escape: return "escape"sv;
        case kind::join_group: return "join_group"sv;
        case kind::aggregate_group: return "aggregate_group"sv;
        case kind::intersection_group: return "intersection_group"sv;
        case kind::difference_group: return "difference_group"sv;
        case kind::flatten_group: return "flatten_group"sv;
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

/**
 * @brief returns whether or not the kind of expression is available in intermediate execution plan.
 * @param value the target kind
 * @return true if the target expression is available in intermediate execution plan
 * @return false otherwise
 */
inline constexpr bool is_available_in_intermediate_plan(expression_kind value) {
    using kind = expression_kind;
    switch (value) {
        case kind::find:
        case kind::scan:
        case kind::join_relation:
        case kind::join_find:
        case kind::join_scan:
        case kind::project:
        case kind::filter:
        case kind::buffer:
        case kind::identify:
        case kind::aggregate_relation:
        case kind::distinct_relation:
        case kind::limit_relation:
        case kind::union_relation:
        case kind::intersection_relation:
        case kind::difference_relation:
        case kind::emit:
        case kind::write:
        case kind::values:
        case kind::escape:
            return true;

        case kind::join_group:
        case kind::aggregate_group:
        case kind::intersection_group:
        case kind::difference_group:
        case kind::flatten_group:
        case kind::take_flat:
        case kind::take_group:
        case kind::take_cogroup:
        case kind::offer:
            return false;

        // FIXME: impl
        case kind::extension:
            return false;
    }
    std::abort();
}

/**
 * @brief returns whether or not the kind of expression is available in step execution plan.
 * @param value the target kind
 * @return true if the target expression is available in step execution plan
 * @return false otherwise
 */
inline constexpr bool is_available_in_step_plan(expression_kind value) {
    using kind = expression_kind;
    switch (value) {
        case kind::find:
        case kind::scan:
        case kind::join_find:
        case kind::join_scan:
        case kind::project:
        case kind::filter:
        case kind::buffer:
        case kind::identify:
        case kind::emit:
        case kind::write:
        case kind::values:
        case kind::join_group:
        case kind::aggregate_group:
        case kind::intersection_group:
        case kind::difference_group:
        case kind::flatten_group:
        case kind::take_flat:
        case kind::take_group:
        case kind::take_cogroup:
        case kind::offer:
            return true;

        case kind::join_relation:
        case kind::aggregate_relation:
        case kind::distinct_relation:
        case kind::limit_relation:
        case kind::union_relation:
        case kind::intersection_relation:
        case kind::difference_relation:
        case kind::escape:
            return false;

        // FIXME: impl
        case kind::extension:
            return false;
    }
    std::abort();
}

} // namespace takatori::relation
