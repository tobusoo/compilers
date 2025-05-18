#pragma once

#include <AST/BaseNode.hpp>
#include <AST/Expressions.hpp>
#include <AST/Formal.hpp>
#include <AST/Symbol.hpp>

class Feature;
using Features = ListNode<Feature*>;

class Feature : public BaseNode {
public:
    Feature() {};

    void Dump(std::ostream& stream, int n) = 0;
    void DumpWithTypes(std::ostream& stream, int n) = 0;
    virtual void Accept(sema::Visitor* v) {};
};

class Method : public Feature {
protected:
    Symbol* name;
    Formals* formals;
    Symbol* return_type;
    Expression* expr;

public:
    Method(Symbol* a1, Formals* a2, Symbol* a3, Expression* a4)
        : name(a1), formals(a2), return_type(a3), expr(a4) {};

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "method\n";
        name->Dump(stream, n + 2);
        formals->Dump(stream, n + 2);
        return_type->Dump(stream, n + 2);
        expr->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "method\n";
        name->Dump(stream, n + 2);
        formals->DumpWithTypes(stream, n + 2);
        return_type->Dump(stream, n + 2);
        expr->DumpWithTypes(stream, n + 2);
    }

    std::string GetName()
    {
        return name->get_string();
    }

    std::string GetReturnType()
    {
        return return_type->get_string();
    }

    Formals* GetFormals()
    {
        return formals;
    }

    Expression* GetExpr()
    {
        return expr;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Attr : public Feature {
protected:
    Symbol* name;
    Symbol* type_decl;
    Expression* init;

public:
    Attr(Symbol* a1, Symbol* a2, Expression* a3) : name(a1), type_decl(a2), init(a3) {};

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "attr\n";
        name->Dump(stream, n + 2);
        type_decl->Dump(stream, n + 2);
        init->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "attr\n";
        name->Dump(stream, n + 2);
        type_decl->Dump(stream, n + 2);
        init->DumpWithTypes(stream, n + 2);
    }

    std::string GetName()
    {
        return name->get_string();
    }

    std::string GetTypeDecl()
    {
        return type_decl->get_string();
    }

    Expression* GetExpr()
    {
        return init;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};