#pragma once

#include <AST/BaseNode.hpp>
#include <AST/Expressions.hpp>
#include <AST/Symbol.hpp>

class Formal;
using Formals = ListNode<Formal*>;

class Formal : public BaseNode {
protected:
    Symbol* name;
    Symbol* type_decl;

public:
    Formal(Symbol* a1, Symbol* a2) : name(a1), type_decl(a2) {};

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "formal\n";
        name->Dump(stream, n + 2);
        type_decl->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "formal\n";
        name->Dump(stream, n + 2);
        type_decl->Dump(stream, n + 2);
    }

    std::string GetName()
    {
        return name->get_string();
    }

    std::string GetTypeDecl()
    {
        return type_decl->get_string();
    }

    void Accept(sema::Visitor* v)
    {
        v->Visit(this);
    }
};