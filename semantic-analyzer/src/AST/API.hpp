#pragma once

#include <AST/Case.hpp>
#include <AST/Class.hpp>
#include <AST/Expressions.hpp>
#include <AST/Program.hpp>

#define yylineno curr_lineno

Classes* nil_Classes();
Classes* single_Classes(Class*);
Classes* append_Classes(Classes*, Classes*);
Features* nil_Features();
Features* single_Features(Feature*);
Features* append_Features(Features*, Features*);
Formals* nil_Formals();
Formals* single_Formals(Formal*);
Formals* append_Formals(Formals*, Formals*);
Expressions* nil_Expressions();
Expressions* single_Expressions(Expression*);
Expressions* append_Expressions(Expressions*, Expressions*);
Cases* nil_Cases();
Cases* single_Cases(Case*);
Cases* append_Cases(Cases*, Cases*);
Program* program(Classes*);
Class* class_(Symbol*, Symbol*, Features*, Symbol*);
Feature* method(Symbol*, Formals*, Symbol*, Expression*);
Feature* attr(Symbol*, Symbol*, Expression*);
Formal* formal(Symbol*, Symbol*);
Case* branch(Symbol*, Symbol*, Expression*);
Expression* assign(Symbol*, Expression*);
Expression* static_dispatch(Expression*, Symbol*, Symbol*, Expressions*);
Expression* dispatch(Expression*, Symbol*, Expressions*);
Expression* cond(Expression*, Expression*, Expression*);
Expression* loop(Expression*, Expression*);
Expression* typcase(Expression*, Cases*);
Expression* block(Expressions*);
Expression* let(Symbol*, Symbol*, Expression*, Expression*);
Expression* plus(Expression*, Expression*);
Expression* sub(Expression*, Expression*);
Expression* mul(Expression*, Expression*);
Expression* divide(Expression*, Expression*);
Expression* neg(Expression*);
Expression* lt(Expression*, Expression*);
Expression* eq(Expression*, Expression*);
Expression* leq(Expression*, Expression*);
Expression* comp(Expression*);
Expression* int_const(Symbol*);
Expression* bool_const(bool);
Expression* string_const(Symbol*);
Expression* new_(Symbol*);
Expression* isvoid(Expression*);
Expression* no_expr();
Expression* object(Symbol*);