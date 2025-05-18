#include <Sema/Errors.hpp>

#include <AST/BaseNode.hpp>

#include <cstdlib>
#include <iostream>

namespace sema {

static const int max_err_count = 20;
static int err_count = 0;

std::string quoted(const std::string& str)
{
    return "\"" + str + "\"";
}

std::string get_full_path(std::string_view filename)
{
    char resolved_path[256];
    if (realpath(filename.data(), resolved_path))
        return resolved_path;
    else
        return filename.data();
}

void error(std::string_view filename, int line, const std::string& msg)
{
    std::cerr << "\033[1m" << get_full_path(filename) << ':';
    std::cerr << line << ':';
    std::cerr << " \033[31m" << "error:" << "\033[0m " << msg << '\n';

    err_count++;
    if (err_count == max_err_count) {
        std::cerr << "To many errors (" << max_err_count << ")\n";
        std::cerr << "Abort\n";
        exit(err_count);
    }
}

void error(const std::string& msg)
{
    std::cerr << "\033[1m\033[31m" << "error:" << "\033[0m " << msg << '\n';

    err_count++;
    if (err_count == max_err_count) {
        std::cerr << "To many errors (" << max_err_count << ")\n";
        std::cerr << "Abort\n";
        exit(err_count);
    }
}

size_t Line(BaseNode* node)
{
    return node->GetLineNumber();
}

}; // namespace sema