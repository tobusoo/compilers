#pragma once

#include <AST/BaseNode.hpp>
#include <AST/Case.hpp>
#include <AST/Symbol.hpp>

#include <Sema/Visitor.hpp>

class Expression : public BaseNode {
public:
    Symbol* type = nullptr;
    Expression* parent = nullptr;

    Expression() {};

    virtual void SetBody(Expression* p)
    {
    }

    std::string GetType()
    {
        if (type == nullptr)
            return "UNKNOWN";
        return type->get_string();
    }

    void SetType(const std::string& str)
    {
        if (type == nullptr) {
            type = new Symbol(str);
        }

        *type = str;
    }

    void DumpType(std::ostream& stream, int n)
    {
        if (type) {
            stream << pad(n) << ": " << *type << '\n';
        } else {
            stream << pad(n) << ": no_type\n";
        }
    }

    virtual void Dump(std::ostream&, int) = 0;
    virtual void DumpWithTypes(std::ostream&, int) = 0;
    virtual void Accept(sema::Visitor* v) = 0;
};

using Expressions = ListNode<Expression*>;

class Assign : public Expression {
protected:
    Symbol* name;
    Expression* expr;

public:
    Assign(Symbol* name, Expression* expr) : name(name), expr(expr)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "assign\n";
        name->Dump(stream, n + 2);
        expr->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "assign\n";
        name->Dump(stream, n + 2);
        expr->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetExpr()
    {
        return expr;
    }

    std::string GetName()
    {
        return name->GetString();
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class StaticDispatch : public Expression {
protected:
    Expression* expr;
    Symbol* type_name;
    Symbol* name;
    Expressions* actual;

public:
    StaticDispatch(Expression* a1, Symbol* a2, Symbol* a3, Expressions* a4)
        : expr(a1), type_name(a2), name(a3), actual(a4)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "static_dispatch\n";
        expr->Dump(stream, n + 2);
        type_name->Dump(stream, n + 2);
        name->Dump(stream, n + 2);
        actual->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "static_dispatch\n";
        expr->DumpWithTypes(stream, n + 2);
        type_name->Dump(stream, n + 2);
        name->Dump(stream, n + 2);
        actual->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetExpr()
    {
        return expr;
    }

    Expressions* GetActual()
    {
        return actual;
    }

    std::string GetTypeName()
    {
        return type_name->GetString();
    }

    std::string GetName()
    {
        return name->GetString();
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Dispatch : public Expression {
protected:
    Expression* expr;
    Symbol* name;
    Expressions* actual;

public:
    Dispatch(Expression* a1, Symbol* a2, Expressions* a3) : expr(a1), name(a2), actual(a3)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "dispatch\n";
        expr->Dump(stream, n + 2);
        name->Dump(stream, n + 2);
        actual->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "dispatch\n";
        expr->DumpWithTypes(stream, n + 2);
        name->Dump(stream, n + 2);
        actual->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    std::string GetName()
    {
        return name->GetString();
    }

    Expression* GetExpr()
    {
        return expr;
    }

    Expressions* GetActual()
    {
        return actual;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Cond : public Expression {
protected:
    Expression* pred;
    Expression* then_exp;
    Expression* else_exp;

public:
    Cond(Expression* a1, Expression* a2, Expression* a3) : pred(a1), then_exp(a2), else_exp(a3)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "cond\n";
        pred->Dump(stream, n + 2);
        then_exp->Dump(stream, n + 2);
        else_exp->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "cond\n";
        pred->DumpWithTypes(stream, n + 2);
        then_exp->DumpWithTypes(stream, n + 2);
        else_exp->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetPred()
    {
        return pred;
    }

    Expression* GetThen()
    {
        return then_exp;
    }

    Expression* GetElse()
    {
        return else_exp;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Loop : public Expression {
protected:
    Expression* pred;
    Expression* body;

public:
    Loop(Expression* a1, Expression* a2) : pred(a1), body(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "loop\n";
        pred->Dump(stream, n + 2);
        body->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "loop\n";
        pred->DumpWithTypes(stream, n + 2);
        body->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetPred()
    {
        return pred;
    }

    Expression* Getbody()
    {
        return body;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class TypeCase : public Expression {
protected:
    Expression* expr;
    Cases* cases;

public:
    TypeCase(Expression* a1, Cases* a2) : expr(a1), cases(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "typecase\n";
        expr->Dump(stream, n + 2);
        cases->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "typecase\n";
        expr->DumpWithTypes(stream, n + 2);
        cases->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetExpr()
    {
        return expr;
    }

    Cases* GetCases()
    {
        return cases;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Block : public Expression {
protected:
    Expressions* body;

public:
    Block(Expressions* a1) : body(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "block\n";
        body->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "block\n";
        body->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expressions* GetExprs()
    {
        return body;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Let : public Expression {
protected:
    Symbol* identifier;
    Symbol* type_decl;
    Expression* init;
    Expression* body;

public:
    Let(Symbol* a1, Symbol* a2, Expression* a3, Expression* a4)
        : identifier(a1), type_decl(a2), init(a3), body(a4)
    {
    }

    void SetBody(Expression* e) override
    {
        e->parent = this;
        body = e;
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "let\n";
        identifier->Dump(stream, n + 2);
        type_decl->Dump(stream, n + 2);
        init->Dump(stream, n + 2);
        body->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "let\n";
        identifier->Dump(stream, n + 2);
        type_decl->Dump(stream, n + 2);
        init->DumpWithTypes(stream, n + 2);
        body->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    std::string GetID()
    {
        return identifier->GetString();
    }

    std::string GetTypeDecl()
    {
        return type_decl->GetString();
    }

    Expression* GetInit()
    {
        return init;
    }

    Expression* GetBody()
    {
        return body;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Plus : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Plus(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "plus\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "plus\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Sub : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Sub(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "sub\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "sub\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Mul : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Mul(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "mul\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "mul\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Divide : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Divide(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "divide\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "divide\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Neg : public Expression {
protected:
    Expression* e1;

public:
    Neg(Expression* a1) : e1(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "neg\n";
        e1->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "neg\n";
        e1->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetExpr()
    {
        return e1;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Lt : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Lt(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "lt\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "lt\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Eq : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Eq(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "eq\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "eq\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Leq : public Expression {
protected:
    Expression* e1;
    Expression* e2;

public:
    Leq(Expression* a1, Expression* a2) : e1(a1), e2(a2)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "leq\n";
        e1->Dump(stream, n + 2);
        e2->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "leq\n";
        e1->DumpWithTypes(stream, n + 2);
        e2->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetLeftExpr()
    {
        return e1;
    }

    Expression* GetRightExpr()
    {
        return e2;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Comp : public Expression {
protected:
    Expression* e1;

public:
    Comp(Expression* a1) : e1(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "comp\n";
        e1->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "comp\n";
        e1->DumpWithTypes(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetExpr()
    {
        return e1;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class IntConst : public Expression {
protected:
    Symbol* token;

public:
    IntConst(Symbol* a1) : token(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "int_const\n";
        token->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "int_const\n";
        token->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class BoolConst : public Expression {
protected:
    bool val;

public:
    BoolConst(bool a1) : val(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "bool_const\n";
        stream << pad(n + 2) << val << '\n';
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "bool_const\n";
        stream << pad(n + 2) << val << '\n';
        DumpType(stream, n);
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class StringConst : public Expression {
protected:
    Symbol* token;

public:
    StringConst(Symbol* a1) : token(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "string_const\n";
        token->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "string_const\n";
        token->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class New : public Expression {
protected:
    Symbol* type_name;

public:
    New(Symbol* a1) : type_name(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "new\n";
        type_name->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "new\n";
        type_name->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    std::string GetTypeName()
    {
        return type_name->GetString();
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class IsVoid : public Expression {
protected:
    Expression* e1;

public:
    IsVoid(Expression* a1) : e1(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "isvoid\n";
        e1->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "isvoid\n";
        e1->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    Expression* GetExpr()
    {
        return e1;
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class NoExpr : public Expression {
public:
    NoExpr()
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "no_expr\n";
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "no_expr\n";
        DumpType(stream, n);
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};

class Object : public Expression {
protected:
    Symbol* name;

public:
    Object(Symbol* a1) : name(a1)
    {
    }

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "object\n";
        name->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "object\n";
        name->Dump(stream, n + 2);
        DumpType(stream, n);
    }

    std::string GetName()
    {
        return name->GetString();
    }

    void Accept(sema::Visitor* v) override
    {
        v->Visit(this);
    }
};