%{
#include <takatori/datetime/parser/scanner.h>
#include <takatori/datetime/parser/parser_generated.hpp>

#define YY_USER_ACTION user_action();

#define yyterminate() return parser_type::make_ERROR(location())

#undef YY_DECL
#define YY_DECL ::takatori::datetime::parser::scanner::value_type takatori::datetime::parser::scanner::next_token(::takatori::datetime::parser::driver& driver)
%}

%option noyywrap
%option c++
%option prefix="_generated_takatori__datetime__parser__scanner_"
%option yyclass="::takatori::datetime::parser::scanner"

digit [0-9]
dot "."

integer {digit}+
decimal {integer}{dot}{integer}

/* error handling */
ASCII   [\x00-\x7f]
UTF8_2  [\xc2-\xdf]
UTF8_3  [\xe0-\xef]
UTF8_4  [\xf0-\xf4]
U       [\x80-\xbf]

UTF8_CHAR {ASCII}|{UTF8_2}{U}|{UTF8_3}{U}{U}|{UTF8_4}{U}{U}{U}

%%

":" { return parser_type::make_COLON(location()); }
"+" { return parser_type::make_PLUS(location()); }
"-" { return parser_type::make_MINUS(location()); }

"T" { return parser_type::make_CHAR_T(location()); }
"Z" { return parser_type::make_CHAR_Z(location()); }
" " { return parser_type::make_WHITESPACE(location()); }

{integer} {
    auto token = get_image(driver);
    auto value = driver.parse_integer(token);
    if (value) {
        return parser_type::make_INTEGER(*value, location());
    }
    return parser_type::make_INVALID_FIELD(location());
}

{decimal} {
    auto token = get_image(driver);
    auto value = driver.parse_decimal(token);
    if (value) {
        return parser_type::make_DECIMAL(*value, location());
    }
    return parser_type::make_INVALID_FIELD(location());
}

{UTF8_CHAR} {
    return parser_type::make_ERROR(location());
}

[\x00-\xff] {
    return parser_type::make_ERROR(location());
}

<<EOF>> {
    return parser_type::make_END_OF_FILE(location(true));
}

%%