#include "Token.hpp"

#include <unordered_map>

namespace tok {

static std::unordered_map<int, const char*> tokensMap = {
        {kw_class, "class"},
        {kw_else, "else"},
        {kw_fi, "fi"},
        {kw_if, "if"},
        {kw_in, "in"},
        {kw_inherits, "inherits"},
        {kw_isvoid, "isvoid"},
        {kw_let, "let"},
        {kw_loop, "loop"},
        {kw_pool, "pool"},
        {kw_then, "then"},
        {kw_while, "while"},
        {kw_case, "case"},
        {kw_esac, "esac"},
        {kw_new, "new"},
        {kw_of, "of"},
        {kw_not, "not"},

        {const_bool, "const_bool"},
        {const_integer, "const_integer"},
        {const_str, "const_str"},

        {op_assignment, "<-"},
        {op_arrow, "=>"},

        {op_plus, "+"},
        {op_minus, "-"},
        {op_mul, "*"},
        {op_div, "/"},

        {op_tilde, "~"},

        {op_lss, "<"},
        {op_leq, "<="},
        {op_eq, "="},

        {l_paren, "("},
        {r_paren, ")"},
        {l_brace, "{"},
        {r_brace, "{"},

        {period, "."},
        {colon, ":"},
        {semi, ";"},
        {comma, ","},
        {at, "@"},

        {identifier_type, "identifier_type"},
        {identifier_object, "identifier_object"},
        {identifier, "identifier"},
};

TokenKind Token::GetKind() const noexcept
{
    return kind;
}

const char* Token::GetKindStr() const noexcept
{
    auto str = tokensMap.find(kind);
    if (str == tokensMap.end())
        return "unknown kind of token";

    return (*str).second;
}

const char* Token::GetValue() const noexcept
{
    return value_str;
}

}; // namespace tok
