#include "scanner.h"

namespace takatori::datetime::parser {

scanner::scanner(std::istream& input) :
    super { std::addressof(input) }
{}

void scanner::LexerError(const char *msg) {
    // FIXME: impl
    super::LexerError(msg);
}

void scanner::user_action() noexcept {
    cursor_ += yyleng;
}

scanner::location_type scanner::location(bool eof) noexcept {
    return { cursor_ - (eof ? 0 : yyleng), cursor_ };
}

std::string scanner::get_image(driver const&) {
    std::string image {
            yytext,
            static_cast<std::size_t>(yyleng),
    };
    return image;
}


} // namespace takatori::datetime::parser
