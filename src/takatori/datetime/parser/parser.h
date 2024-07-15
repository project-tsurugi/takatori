#pragma once

#include <optional>
#include <string_view>
#include <variant>

#include <takatori/util/either.h>

#include "parser_error.h"
#include "parser_result_info.h"

namespace takatori::datetime::parser {

class parser {
public:
    using success_type = parser_result_info;
    using error_type = parser_error;
    using result_type = ::takatori::util::either<error_type, success_type>;

    parser& set_debug(int level = 1) noexcept;

    [[nodiscard]] result_type operator()(std::string_view contents) const;

private:
    int debug_ {};
};

parser::result_type parse(std::string_view contents, parser const& engine = {});

} // namespace takatori::datetime::parser
