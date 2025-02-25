%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.hpp"
#include "CoolLexer.hpp"

#undef YY_DECL
#define	YY_DECL tok::Token CoolLexer::lex()

#undef YY_NULL
#define YY_NULL tok::Token(tok::TokenKind(0))
%}

%option warn nodefault batch noyywrap c++
%option yyclass="CoolLexer"

white_space         [ \t\f\b\r]*
digit               [0-9]
alpha               [A-Za-z_]
alpha_num           ({alpha}|{digit})

%%

(?i:class)          return tok::Token(tok::kw_class, YYText());
(?i:else)           return tok::Token(tok::kw_else, YYText());
(?:fi)              return tok::Token(tok::kw_fi, YYText());
(?:if)              return tok::Token(tok::kw_if, YYText());
(?:in)              return tok::Token(tok::kw_in, YYText());
(?:inherits)        return tok::Token(tok::kw_inherits, YYText());
(?:isvoid)          return tok::Token(tok::kw_isvoid, YYText());
(?:let)             return tok::Token(tok::kw_let, YYText());
(?:loop)            return tok::Token(tok::kw_loop, YYText());
(?:pool)            return tok::Token(tok::kw_pool, YYText());
(?:while)           return tok::Token(tok::kw_while, YYText());
(?:case)            return tok::Token(tok::kw_case, YYText());
(?:esac)            return tok::Token(tok::kw_esac, YYText());
(?:new)             return tok::Token(tok::kw_new, YYText());
(?:of)              return tok::Token(tok::kw_of, YYText());
(?:not)             return tok::Token(tok::kw_not, YYText());

f(?:alse)           return tok::Token(tok::const_bool, YYText());
t(?:rue)            return tok::Token(tok::const_bool, YYText());
{digit}+            return tok::Token(tok::const_integer, YYText());

"<-"                return tok::Token(tok::op_assignment, YYText());
"=>"                return tok::Token(tok::op_arrow, YYText());

"+"                 return tok::Token(tok::op_plus, YYText());
"-"                 return tok::Token(tok::op_minus, YYText());
"*"                 return tok::Token(tok::op_mul, YYText());
"/"                 return tok::Token(tok::op_div, YYText());

"~"                 return tok::Token(tok::op_tilde, YYText());

"<"                 return tok::Token(tok::op_lss, YYText());
"<="                return tok::Token(tok::op_leq, YYText());
"="                 return tok::Token(tok::op_eq, YYText());


"["                 return tok::Token(tok::l_square, YYText());
"]"                 return tok::Token(tok::r_square, YYText());
"("                 return tok::Token(tok::l_paren, YYText());
")"                 return tok::Token(tok::r_paren, YYText());
"{"                 return tok::Token(tok::l_brace, YYText());
"}"                 return tok::Token(tok::r_brace, YYText());

"."                 return tok::Token(tok::period, YYText());
":"                 return tok::Token(tok::colon, YYText());
";"                 return tok::Token(tok::semi, YYText());
","                 return tok::Token(tok::comma, YYText());
"@"                 return tok::Token(tok::at, YYText());

[a-z]{alpha_num}*   return tok::Token(tok::identifier_type, YYText());
[A-Z]{alpha_num}*   return tok::Token(tok::identifier_object, YYText());
_{alpha_num}*       return tok::Token(tok::identifier, YYText());


\n                  lineno++;
{white_space}       { }
.                   Error("unrecognized character");

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}