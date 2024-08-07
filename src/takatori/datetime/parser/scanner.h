#pragma once

// manually include flex system header
#if !defined(FLEX_SCANNER)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage): Flex framework matter
#define yyFlexLexer _generated_takatori__datetime__parser__scanner_FlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#undef yyFlexLexerOnce

#endif // !defined(FLEX_SCANNER)

#include <string>

#include <takatori/datetime/parser/driver.h>
#include <takatori/datetime/parser/parser_generated.hpp>

namespace takatori::datetime::parser {

class scanner : _generated_takatori__datetime__parser__scanner_FlexLexer {
private:
    using super = _generated_takatori__datetime__parser__scanner_FlexLexer;

public:
    using parser_type = ::takatori::datetime::parser::parser_generated;
    using driver_type = ::takatori::datetime::parser::driver;
    using value_type = typename parser_type::symbol_type;
    using location_type = driver_type::location_type;

    explicit scanner(std::istream& input);

    // will be generated by flex
    [[nodiscard]] value_type next_token(driver_type& driver);

protected:
    void LexerError(char const* msg) override;

private:
    std::size_t npos = static_cast<std::size_t>(-1);

    std::size_t cursor_ {};
    std::size_t comment_begin_ { npos };

    void user_action() noexcept;
    [[nodiscard]] location_type location(bool eof = false) noexcept;

    std::string get_image(driver_type const& driver);
};

} // namespace takatori::datetime::parser
