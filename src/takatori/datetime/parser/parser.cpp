#include "parser.h"

#include <sstream>

#include <takatori/datetime/parser/parser_generated.hpp>
#include <takatori/datetime/parser/scanner.h>
#include <takatori/datetime/parser/driver.h>

namespace takatori::datetime::parser {

parser &parser::set_debug(int level) noexcept {
    debug_ = level;
    return *this;
}

parser::result_type parser::operator()(std::string_view contents) const {
    std::istringstream input {
            std::string { contents },
    };
    driver driver {};
    scanner scanner { input };
    parser_generated parser { scanner, driver };

#if YYDEBUG
    parser.set_debug_level(static_cast<parser_generated::debug_level_type>(debug_));
    parser.set_debug_stream(std::cout);
#endif // YYDEBUG

    parser.parse();
    return std::move(driver.result());
}

parser::result_type parse(std::string_view contents, parser const& engine) {
    return engine(contents);
}

} // namespace takatori::datetime::parser
