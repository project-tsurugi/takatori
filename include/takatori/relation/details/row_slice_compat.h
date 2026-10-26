#pragma once

#include <cstddef>
#include <optional>
#include <type_traits>

#include "row_slice.h"

namespace takatori::relation::details {

/**
 * @brief a constraint of the row count parameter in the compatibility constructors.
 * @details This only accepts integral types (except `bool`).
 *      Especially, this rejects `{}`, so that such the argument is passed to the constructors with row_slice,
 *      and it represents an empty row slice (unlimited) as same as `std::optional` in the previous API.
 *      On the other hand, `std::nullopt` or `std::optional` are no longer accepted.
 * @tparam T the parameter type
 * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
 */
template<class T>
using enable_if_compatible_row_count_t = std::enable_if_t<
        std::is_integral_v<T> && !std::is_same_v<T, bool>,
        std::nullptr_t>;

/**
 * @brief returns the row count of the given row slice, for the compatibility API.
 * @param slice the target row slice
 * @return the row count
 * @return empty if the row count is not specified
 * @throws std::domain_error if the row slice has the start row position other than immediate `0`
 * @throws std::domain_error if the row slice has a variable row count
 * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
 */
[[nodiscard]] std::optional<std::size_t> get_compatible_row_count(row_slice const& slice);

/**
 * @brief sets the row count of the given row slice, for the compatibility API.
 * @details This only changes the row count, and keeps the start row position.
 * @param slice the target row slice
 * @param count the row count, or empty to unlimited
 * @note deprecated: use row_slice() instead. This compatibility API will be removed after the migration.
 */
void set_compatible_row_count(row_slice& slice, std::optional<std::size_t> count) noexcept;

} // namespace takatori::relation::details
