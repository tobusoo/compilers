#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "CoolLexer.hpp"
#include "Token.hpp"

void PrintToken(const tok::Token& token, const size_t lineno)
{
    auto kind = token.GetKind();
    auto kindStr = token.GetKindStr();
    auto value = token.GetValue();

    std::cout << "line " << lineno << ": <";
    std::cout << std::quoted(kindStr, '\'') << " id=" << kind;

    if (value != nullptr)
        std::cout << ", " << std::quoted(value, '\'');

    std::cout << ">\n";
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: coolc <source>\n";
        std::exit(EXIT_FAILURE);
    }

    std::ifstream ifs(argv[1]);
    if (ifs.fail()) {
        std::cerr << "Error opening file `" << argv[1] << "`\n";
        std::exit(EXIT_FAILURE);
    }

    CoolLexer lexer(ifs, std::cout);

    for (auto token = lexer.lex(); token.GetKind(); token = lexer.lex()) {
        PrintToken(token, lexer.Line());
    }

    return 0;
}
