#include <AST/StringTable.hpp>
#include <Parser/Common.hpp>
#include <Sema/Errors.hpp>

extern Classes* parse_results;
extern Program* ast_root;
extern IdTable idtable;
extern IntTable inttable;
extern StrTable stringtable;

extern int curr_lineno;
extern int parse_errors;

const char* curr_filename;
std::FILE* token_file = stdin;

extern int cool_yyparse();

namespace parser {

bool Parse(const char* filename)
{
    curr_lineno = 1;
    curr_filename = filename;

    token_file = std::fopen(filename, "r");
    if (token_file == nullptr) {
        std::cerr << "Error: can't open file " << sema::quoted(filename) << std::endl;
        return false;
    }

    cool_yyparse();
    if (parse_errors != 0)
        return false;

    std::fclose(token_file);
    return true;
}

Program* GetProgram()
{
    return ast_root;
}
}; // namespace parser