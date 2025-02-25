#pragma once
#include <fstream>
#include <iostream>

#undef yyFlexLexer
#include <FlexLexer.h>

#include "Parser.hpp"

class CoolLexer final : public yyFlexLexer {
public:
    CoolLexer(std::istream& arg_yyin, std::ostream& arg_yyout)
        : yyFlexLexer{arg_yyin, arg_yyout}, out{arg_yyout}, lineno{1}
    {
    }

    size_t line() noexcept
    {
        return lineno;
    }

    virtual tok::Token lex();

private:
    void Error(const char* msg) const;

    std::ostream& out;
    size_t lineno;
};
