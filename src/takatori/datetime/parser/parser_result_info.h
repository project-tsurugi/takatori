#pragma once

#include <optional>

#include <takatori/datetime/conversion_info.h>

namespace takatori::datetime::parser {

/**
 * @brief represents datetime information.
 */
struct parser_result_info {

    /// @brief the date information.
    std::optional<date_info> date;

    /// @brief the time of day information.
    std::optional<time_info> time;

    /// @brief the timezone offset information.
    std::optional<zone_offset_info> offset;
};

} // namespace takatori::datetime::parser
