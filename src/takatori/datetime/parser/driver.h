#pragma once

#include <optional>
#include <string>
#include <utility>
#include <variant>

#include <cstdint>

#include "parser.h"
#include "region.h"
#include "parser_error.h"
#include "parser_result_info.h"

namespace takatori::datetime::parser {

class driver {
public:
    using location_type = region;
    using success_type = parser::success_type;
    using error_type = parser::error_type;
    using result_type = parser::result_type;

    void success(success_type result);

    void error(error_type error);

    [[nodiscard]] result_type& result() noexcept;

    [[nodiscard]] result_type const& result() const noexcept;

    [[nodiscard]] std::optional<std::uint32_t> parse_integer(std::string const& token);

    [[nodiscard]] std::optional<std::pair<std::uint32_t, std::uint32_t>> parse_decimal(std::string const& token);

private:
    result_type result_ {};
};

} // namespace takatori::datetime::parser
