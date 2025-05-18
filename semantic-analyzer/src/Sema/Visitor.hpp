#pragma once

#include <string>

extern int curr_lineno;

class Program;
class Class;
class Method;
class Attr;
class Formal;
class Case;
class Assign;
class StaticDispatch;
class Dispatch;
class Cond;
class Loop;
class TypeCase;
class Block;
class Let;
class Plus;
class Sub;
class Mul;
class Divide;
class Neg;
class Lt;
class Eq;
class Leq;
class Comp;
class IntConst;
class Case;
class BoolConst;
class StringConst;
class New;
class IsVoid;
class NoExpr;
class Object;
class Feature;
class Expression;

namespace sema {

class Visitor {
protected:
    std::string curr_filename;

public:
    virtual void Visit(Program* node) {};
    virtual void Visit(Class* node) {};
    virtual void Visit(Method* node) {};
    virtual void Visit(Attr* node) {};
    virtual void Visit(Formal* node) {};

    virtual void Visit(Expression* node) {};

    virtual void Visit(Case* node) {};
    virtual void Visit(Assign* node) {};
    virtual void Visit(StaticDispatch* node) {};
    virtual void Visit(Dispatch* node) {};
    virtual void Visit(Cond* node) {};
    virtual void Visit(Feature* node) {};
    virtual void Visit(Loop* node) {};
    virtual void Visit(TypeCase* node) {};
    virtual void Visit(Block* node) {};
    virtual void Visit(Let* node) {};
    virtual void Visit(Plus* node) {};
    virtual void Visit(Sub* node) {};
    virtual void Visit(Mul* node) {};
    virtual void Visit(Divide* node) {};
    virtual void Visit(Neg* node) {};
    virtual void Visit(Lt* node) {};
    virtual void Visit(Eq* node) {};
    virtual void Visit(Leq* node) {};
    virtual void Visit(Comp* node) {};
    virtual void Visit(IntConst* node) {};
    virtual void Visit(BoolConst* node) {};
    virtual void Visit(StringConst* node) {};
    virtual void Visit(New* node) {};
    virtual void Visit(IsVoid* node) {};
    virtual void Visit(NoExpr* node) {};
    virtual void Visit(Object* node) {};
};

} // namespace sema