#pragma once

#include <string>

#include "region.h"

namespace takatori::datetime::parser {

struct parser_error {
    ::takatori::datetime::parser::region region {};
    std::string message {};
};

} // namespace takatori::datetime::parser
