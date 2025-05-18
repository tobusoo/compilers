#include <AST/Symbol.hpp>

std::ostream& operator<<(std::ostream& s, const Symbol& sym)
{
    return s << sym.GetString();
}
