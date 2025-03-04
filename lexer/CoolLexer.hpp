#pragma once
#include <cstring>
#include <fstream>
#include <iostream>

#undef yyFlexLexer
#include <FlexLexer.h>

#include "Token.hpp"

class CoolLexer final : public yyFlexLexer {
public:
    CoolLexer(std::istream& arg_yyin, std::ostream& arg_yyout)
        : yyFlexLexer{arg_yyin, arg_yyout}, lineno{1}, comment_level{0}
    {
    }

    size_t Line() noexcept
    {
        return lineno;
    }

    virtual tok::Token lex();

private:
    void Error(const char* msg) const noexcept;
    const char* EscapeCurrentYYText() const noexcept;

    size_t lineno;
    size_t comment_level;
};
