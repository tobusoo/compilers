#pragma once

#include <AST/BaseNode.hpp>

#include <Sema/Visitor.hpp>

class Case;
class Symbol;
class Expression;

using Cases = ListNode<Case*>;

class Case : public BaseNode {
public:
    Symbol* name;
    Symbol* type_decl;
    Expression* expr;

    Case(Symbol* a1, Symbol* a2, Expression* a3);
    void Dump(std::ostream& stream, int n);
    void DumpWithTypes(std::ostream& stream, int n);
    void Accept(sema::Visitor* v);
    std::string GetName();
    std::string GetTypeDecl();
    Expression* GetExpr();
};
