#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "CoolLexer.hpp"
#include "Parser.hpp"

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

    for (auto token = lexer.lex(); token.kind; token = lexer.lex()) {
        std::cout << "line " << lexer.line() << ": <" << token.kind << ',';
        if (token.value_str != nullptr)
            std::cout << " '" << token.value_str << '\'';
        std::cout << ">\n";
    }

    return 0;
}
