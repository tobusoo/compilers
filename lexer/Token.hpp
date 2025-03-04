#pragma once

namespace tok {

enum TokenKind {
    // Keywords
    kw_class = 1,
    kw_else,
    kw_fi,
    kw_if,
    kw_in,
    kw_inherits,
    kw_isvoid,
    kw_let,
    kw_loop,
    kw_pool,
    kw_then,
    kw_while,
    kw_case,
    kw_esac,
    kw_new,
    kw_of,
    kw_not,

    // Consts
    const_bool,
    const_integer,
    const_str,

    // Assignment Operator
    op_assignment, // '<-'
    op_arrow,      // '=>'

    // Binary Operators
    op_plus,  // '+'
    op_minus, // '-'
    op_mul,   // '*'
    op_div,   // '/'

    // Unary Operators
    op_tilde, // '~'

    // Comp Operators
    op_lss, // '<'
    op_leq, // '<='
    op_eq,  // '='

    // Punctuation
    l_paren, // '('
    r_paren, // ')'
    l_brace, // '{'
    r_brace, // '}'

    period, // '.'
    colon,  // ':'
    semi,   // ';'
    comma,  // ','
    at,     // '@'

    // Identifiers
    identifier_type,
    identifier_object,
    identifier,

    NUM_TOKENS
}; // enum TokenKind

class Token {
public:
    Token(TokenKind kind, const char* value = nullptr) : kind{kind}, value_str{value} {};

    TokenKind GetKind() const noexcept;
    const char* GetKindStr() const noexcept;
    const char* GetValue() const noexcept;

private:
    TokenKind kind;
    const char* value_str;
};

}; // namespace tok
