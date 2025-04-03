%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Token.hpp"
#include "CoolLexer.hpp"

#undef YY_DECL
#define	YY_DECL tok::Token CoolLexer::lex()

#undef YY_NULL
#define YY_NULL tok::Token(tok::TokenKind(0))
%}


%option warn batch noyywrap c++
%option yyclass="CoolLexer"

%x COMMENT STRING

white_space         [ \t\f\b\r]*
digit               [0-9]
alpha               [A-Za-z_]
alpha_num           ({alpha}|{digit})
float               ({digit}+\.{digit}*|{digit}*\.{digit}+)

%%

--.*$                   { }
"*)"                    Error("unmatched *)");
"(*"                    { BEGIN(COMMENT); comment_level = 1; }
<COMMENT>"(*"           { comment_level++; }
<COMMENT>"*)"           { 
                            comment_level--; 
                            if (comment_level == 0)
                                BEGIN(INITIAL);
                        }
<COMMENT>\n             { lineno++; }
<COMMENT>.              { }
<COMMENT><<EOF>>        Error("EOF in comment");

"\""                    { BEGIN(STRING); yymore(); }
<STRING>\n              Error("wrong newline in string");
<STRING><<EOF>>         Error("EOF in string");
<STRING>[^\\\"\n]*      { yymore(); }
<STRING>\\[^\n]         { yymore(); }
<STRING>\\\n            { lineno++; yymore(); }
<STRING>"\""            { BEGIN(INITIAL); return tok::Token(tok::const_str, EscapeCurrentYYText()); }

(?i:class)          return tok::Token(tok::kw_class);
(?i:else)           return tok::Token(tok::kw_else);
(?:fi)              return tok::Token(tok::kw_fi);
(?:if)              return tok::Token(tok::kw_if);
(?:in)              return tok::Token(tok::kw_in);
(?:inherits)        return tok::Token(tok::kw_inherits);
(?:isvoid)          return tok::Token(tok::kw_isvoid);
(?:let)             return tok::Token(tok::kw_let);
(?:loop)            return tok::Token(tok::kw_loop);
(?:pool)            return tok::Token(tok::kw_pool);
(?:while)           return tok::Token(tok::kw_while);
(?:case)            return tok::Token(tok::kw_case);
(?:esac)            return tok::Token(tok::kw_esac);
(?:new)             return tok::Token(tok::kw_new);
(?:of)              return tok::Token(tok::kw_of);
(?:not)             return tok::Token(tok::kw_not);

f(?:alse)           return tok::Token(tok::const_bool, "false");
t(?:rue)            return tok::Token(tok::const_bool, "true");
{digit}+            return tok::Token(tok::const_integer, YYText());

"<-"                return tok::Token(tok::op_assignment);
"=>"                return tok::Token(tok::op_arrow);

"+"                 return tok::Token(tok::op_plus);
"-"                 return tok::Token(tok::op_minus);
"*"                 return tok::Token(tok::op_mul);
"/"                 return tok::Token(tok::op_div);

"~"                 return tok::Token(tok::op_tilde);

"<"                 return tok::Token(tok::op_lss);
"<="                return tok::Token(tok::op_leq);
"="                 return tok::Token(tok::op_eq);

"("                 return tok::Token(tok::l_paren);
")"                 return tok::Token(tok::r_paren);
"{"                 return tok::Token(tok::l_brace);
"}"                 return tok::Token(tok::r_brace);

"."                 return tok::Token(tok::period);
":"                 return tok::Token(tok::colon);
";"                 return tok::Token(tok::semi);
","                 return tok::Token(tok::comma);
"@"                 return tok::Token(tok::at);

[a-z]{alpha_num}*   return tok::Token(tok::identifier_object, YYText());
[A-Z]{alpha_num}*   return tok::Token(tok::identifier_type, YYText());


{float}             Error("floating point number are not supporting");

\n                  lineno++;
{white_space}       { }
.                   Error("unrecognized character");

%%

void CoolLexer::Error(const char* msg) const noexcept
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}
 
const char* CoolLexer::EscapeCurrentYYText() const noexcept
{
    char* str = yytext;
    char* out = yytext;

    str++;
    while (*(str + 1)) {
        if (*str == '\\') {
            str++;
            switch (*str) {
            case 'n':
                *out++ = '\n';
                break;
            case 't':
                *out++ = '\t';
                break;
            case 'f':
                *out++ = '\f';
                break;
            case 'b':
                *out++ = '\b';
                break;
            default:
                *out++ = *str;
            }
        } else {
            *out++ = *str;
        }
        str++;
    }
    *out = '\0';
    return yytext;
}