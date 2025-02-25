%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.hpp"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()

%}

%option warn nodefault batch noyywrap c++
%option yylineno
%option yyclass="CoolLexer"

%%

(?i:class)          return tok::kw_class;
(?i:else)           return tok::kw_else;
f(?i:alse)          return tok::kw_false;
(?:fi)              return tok::kw_fi;
(?:if)              return tok::kw_if;
(?:in)              return tok::kw_in;
(?:inherits)        return tok::kw_inherits;
(?:isvoid)          return tok::kw_isvoid;
(?:let)             return tok::kw_let;
(?:loop)            return tok::kw_loop;
(?:pool)            return tok::kw_pool;
(?:then)            return tok::kw_then;
(?:while)           return tok::kw_while;
(?:case)            return tok::kw_case;
(?:esac)            return tok::kw_esac;
(?:new)             return tok::kw_new;
(?:of)              return tok::kw_of;
(?:not)             return tok::kw_not;
t(?:rue)            return tok::kw_true;

\n                 lineno++;
[ \t]             {}
.                   Error("unrecognized character");

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}
