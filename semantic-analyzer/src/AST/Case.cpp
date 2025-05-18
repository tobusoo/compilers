#include <AST/Case.hpp>
#include <AST/Expressions.hpp>

Case::Case(Symbol* a1, Symbol* a2, Expression* a3) : name(a1), type_decl(a2), expr(a3)
{
}

void Case::Dump(std::ostream& stream, int n)
{
    stream << pad(n) << "branch\n";
    name->Dump(stream, n + 2);
    type_decl->Dump(stream, n + 2);
    expr->Dump(stream, n + 2);
}

void Case::DumpWithTypes(std::ostream& stream, int n)
{
    stream << pad(n) << "line:" << line_number << '\n';
    stream << pad(n) << "branch\n";
    name->Dump(stream, n + 2);
    type_decl->Dump(stream, n + 2);
    expr->DumpWithTypes(stream, n + 2);
}

void Case::Accept(sema::Visitor* v)
{
    v->Visit(this);
}

std::string Case::GetName()
{
    return name->GetString();
}

std::string Case::GetTypeDecl()
{
    return type_decl->GetString();
}

Expression* Case::GetExpr()
{
    return expr;
}
