#include <AST/API.hpp>

int node_lineno = 1;

Classes* nil_Classes()
{
    return new Classes();
}

Classes* single_Classes(Class* c)
{
    return new Classes(c);
}

Classes* append_Classes(Classes* c1, Classes* c2)
{
    return new Classes(*c1, *c2);
}

Features* nil_Features()
{
    return new Features();
}

Features* single_Features(Feature* f)
{
    return new Features(f);
}

Features* append_Features(Features* f1, Features* f2)
{
    return new Features(*f1, *f2);
}

Formals* nil_Formals()
{
    return new Formals();
}

Formals* single_Formals(Formal* f)
{
    return new Formals(f);
}

Formals* append_Formals(Formals* f1, Formals* f2)
{
    return new Formals(*f1, *f2);
}

Expressions* nil_Expressions()
{
    return new Expressions();
}

Expressions* single_Expressions(Expression* e)
{
    return new Expressions(e);
}

Expressions* append_Expressions(Expressions* e1, Expressions* e2)
{
    return new Expressions(*e1, *e2);
}

Cases* nil_Cases()
{
    return new Cases();
}

Cases* single_Cases(Case* c)
{
    return new Cases(c);
}

Cases* append_Cases(Cases* c1, Cases* c2)
{
    return new Cases(*c1, *c2);
}

Program* program(Classes* c)
{
    return new Program(c);
}

Class* class_(Symbol* name, Symbol* parent, Features* features, Symbol* filename)
{
    return new Class(name, parent, features, filename);
}

Feature* method(Symbol* name, Formals* formals, Symbol* return_type, Expression* expr)
{
    return new Method(name, formals, return_type, expr);
}

Feature* attr(Symbol* name, Symbol* type_decl, Expression* init)
{
    return new Attr(name, type_decl, init);
}

Formal* formal(Symbol* name, Symbol* type_decl)
{
    return new Formal(name, type_decl);
}

Case* branch(Symbol* name, Symbol* type_decl, Expression* expr)
{
    return new Case(name, type_decl, expr);
}

Expression* assign(Symbol* name, Expression* expr)
{
    return new Assign(name, expr);
}

Expression* static_dispatch(Expression* expr, Symbol* type_name, Symbol* name, Expressions* actual)
{
    return new StaticDispatch(expr, type_name, name, actual);
}

Expression* dispatch(Expression* expr, Symbol* name, Expressions* actual)
{
    return new Dispatch(expr, name, actual);
}

Expression* cond(Expression* pred, Expression* then_exp, Expression* else_exp)
{
    return new Cond(pred, then_exp, else_exp);
}

Expression* loop(Expression* pred, Expression* body)
{
    return new Loop(pred, body);
}

Expression* typcase(Expression* expr, Cases* cases)
{
    return new TypeCase(expr, cases);
}

Expression* block(Expressions* body)
{
    return new Block(body);
}

Expression* let(Symbol* identifier, Symbol* type_decl, Expression* init, Expression* body)
{
    return new Let(identifier, type_decl, init, body);
}

Expression* plus(Expression* e1, Expression* e2)
{
    return new Plus(e1, e2);
}

Expression* sub(Expression* e1, Expression* e2)
{
    return new Sub(e1, e2);
}

Expression* mul(Expression* e1, Expression* e2)
{
    return new Mul(e1, e2);
}

Expression* divide(Expression* e1, Expression* e2)
{
    return new Divide(e1, e2);
}

Expression* neg(Expression* e1)
{
    return new Neg(e1);
}

Expression* lt(Expression* e1, Expression* e2)
{
    return new Lt(e1, e2);
}

Expression* eq(Expression* e1, Expression* e2)
{
    return new Eq(e1, e2);
}

Expression* leq(Expression* e1, Expression* e2)
{
    return new Leq(e1, e2);
}

Expression* comp(Expression* e1)
{
    return new Comp(e1);
}

Expression* int_const(Symbol* token)
{
    return new IntConst(token);
}

Expression* bool_const(bool val)
{
    return new BoolConst(val);
}

Expression* string_const(Symbol* token)
{
    return new StringConst(token);
}

Expression* new_(Symbol* type_name)
{
    return new New(type_name);
}

Expression* isvoid(Expression* e1)
{
    return new IsVoid(e1);
}

Expression* no_expr()
{
    return new NoExpr();
}

Expression* object(Symbol* name)
{
    return new Object(name);
}
