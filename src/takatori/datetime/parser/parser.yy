%skeleton "lalr1.cc"
%require "3.6"

%defines
%define api.token.constructor true
%define api.value.type variant
%define api.value.automove true
%define api.namespace { takatori::datetime::parser }
%define api.parser.class { parser_generated }
%define api.token.prefix {}
%define parse.error detailed
%define parse.lac full

%code requires {

    #include <utility>

    #include <cstdint>

    #include <takatori/datetime/parser/region.h>
    #include <takatori/datetime/parser/parser_result_info.h>

    namespace takatori::datetime::parser {

    class scanner;
    class driver;

    using integer_type = std::uint32_t;
    using decimal_type = std::pair<std::uint32_t, std::uint32_t>;
    using sign_type = bool;

    using datetime_type = parser_result_info;
    using date_type = date_info;
    using time_type = time_info;
    using offset_type = zone_offset_info;

    } // namespace takatori::datetime::parser
}

%locations
%define api.location.type { ::takatori::datetime::parser::region }

%code {
    #include <takatori/datetime/parser/scanner.h>
    #include <takatori/datetime/parser/driver.h>

    namespace takatori::datetime::parser {

    static parser_generated::symbol_type yylex(scanner& scanner, driver& driver) {
        return scanner.next_token(driver);
    }

    void parser_generated::error(location_type const& location, std::string const& message) {
        driver.error({ location, message });
    }

    } // namespace takatori::datetime::parser
}

%param { ::takatori::datetime::parser::scanner& scanner }
%param { ::takatori::datetime::parser::driver& driver }

%token <integer_type> INTEGER "<INTEGER>"
%token <decimal_type> DECIMAL "<DECIMAL>"

%token COLON ":"
%token PLUS "+"
%token MINUS "-"

%token CHAR_T "T"
%token CHAR_Z "Z"
%token WHITESPACE "<WHITESPACE>"

%token ERROR "<ERROR>"
%token INVALID_FIELD "<INVALID_FIELD>"
%token END_OF_FILE 0 "<END_OF_FILE>"

%nterm program

%nterm <datetime_type> datetime
%nterm <date_type> date
%nterm <time_type> time
%nterm <offset_type> offset
%nterm <std::optional<offset_type>> offset_opt
%nterm <sign_type> sign;

%nterm date_time_separator

%start program

%%

program
    : datetime[r] END_OF_FILE
        {
            driver.success($r);
        }
    ;

datetime
    : date[d]
        {
            $$ = datetime_type { $d, {}, {} };
        }
    | time[t] offset_opt[o]
        {
            $$ = datetime_type { {}, $t, $o };
        }
    | date[d] date_time_separator time[t] offset_opt[o]
        {
            $$ = datetime_type { $d, $t, $o };
        }
    | offset[o]
        {
            $$ = datetime_type { {}, {}, $o };
        }
    ;

date
    : INTEGER[y] "-" INTEGER[m] "-" INTEGER[d]
        {
            $$ = date_type { $y, $m, $d };
        }
    ;

time
    : INTEGER[h] ":" INTEGER[m] ":" INTEGER[s]
        {
            $$ = time_type { $h, $m, $s, {} };
        }
    | INTEGER[h] ":" INTEGER[m] ":" DECIMAL[s]
        {
            auto [sec, nano] = $s;
            $$ = time_type { $h, $m, sec, time_type::subsecond_type { nano } };
        }
    ;

offset
    : sign[s] INTEGER[h]
        {
            auto sign = $s;
            auto field = $h;
            if (field > 100) {
                $$ = offset_type { sign, field / 100, field % 100 };
            } else {
                $$ = offset_type { sign, field, 0 };
            }
        }
    | sign[s] INTEGER[h] ":" INTEGER[m]
        {
            $$ = offset_type { $s, $h, $m };
        }
    | "Z"
        {
            $$ = offset_type {};
        }
    ;

offset_opt
    : offset[o]
        {
            $$ = $o;
        }
    | %empty
        {
            $$ = std::nullopt;
        }
    ;

sign
    : "+" { $$ = true; }
    | "-" { $$ = false; }
    ;

date_time_separator
    : "T"
    | WHITESPACE
    ;
