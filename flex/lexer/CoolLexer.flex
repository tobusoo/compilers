%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()

%}

white_space       [ \t]*
digit             [0-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit})
hex_digit         [0-9a-fA-F]
identifier        {alpha}{alpha_num}*
unsigned_integer  {digit}+
hex_integer       ${hex_digit}{hex_digit}*
exponent          e[+-]?{digit}+
i                 {unsigned_integer}
real              ({i}\.{i}?|{i}?\.{i}){exponent}?
string            \'([^'\n]|\'\')*\'
bad_string        \'([^'\n]|\'\')*

%x COMMENT

%option warn nodefault batch noyywrap c++
%option yylineno
%option yyclass="CoolLexer"

%%

"{"                 BEGIN(COMMENT);
<COMMENT>[^}\n]+    { /* skip*/ }
<COMMENT>\n         { lineno++; }
<COMMENT><<EOF>>    Error("EOF in comment");
<COMMENT>"}"        BEGIN(INITIAL);

and                  return TOKEN_AND;
array                return TOKEN_ARRAY;
begin                return TOKEN_BEGIN;
case                 return TOKEN_CASE;
const                return TOKEN_CONST;
div                  return TOKEN_DIV;
do                   return TOKEN_DO;
downto               return TOKEN_DOWNTO;
else                 return TOKEN_ELSE;
end                  return TOKEN_END;
file                 return TOKEN_FILE;
for                  return TOKEN_FOR;
function             return TOKEN_FUNCTION;
goto                 return TOKEN_GOTO;
if                   return TOKEN_IF;
in                   return TOKEN_IN;
label                return TOKEN_LABEL;
mod                  return TOKEN_MOD;
nil                  return TOKEN_NIL;
not                  return TOKEN_NOT;
of                   return TOKEN_OF;
packed               return TOKEN_PACKED;
procedure            return TOKEN_PROCEDURE;
program              return TOKEN_PROGRAM;
record               return TOKEN_RECORD;
repeat               return TOKEN_REPEAT;
set                  return TOKEN_SET;
then                 return TOKEN_THEN;
to                   return TOKEN_TO;
type                 return TOKEN_TYPE;
until                return TOKEN_UNTIL;
var                  return TOKEN_VAR;
while                return TOKEN_WHILE;
with                 return TOKEN_WITH;

"<="|"=<"            return TOKEN_LEQ;
"=>"|">="            return TOKEN_GEQ;
"<>"                 return TOKEN_NEQ;
"="                  return TOKEN_EQ;
".."                 return TOKEN_DOUBLEDOT;

{unsigned_integer}   return TOKEN_UNSIGNED_INTEGER;
{real}               return TOKEN_REAL;
{hex_integer}        return TOKEN_HEX_INTEGER;

{string}             return TOKEN_STRING;
{bad_string}         Error("unterminated string");

{identifier}         return TOKEN_IDENTIFIER;


[*/+\-,^.;:()\[\]]   return yytext[0];

{white_space}        { /* skip spaces */ }
\n                   lineno++;
.                    Error("unrecognized character");

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}
