#include <AST/BaseClasses.hpp>

#include <AST/Feature.hpp>
#include <AST/StringTable.hpp>

extern IdTable idtable;
extern IntTable inttable;
extern StrTable stringtable;

Symbol* Sym(char* str)
{
    return idtable.lookup_string(str);
}

void InitBaseClasses()
{
    idtable.add_string("String");
    idtable.add_string("Int");
    idtable.add_string("Object");
    idtable.add_string("Bool");
    idtable.add_string("IO");
    idtable.add_string("SELF_TYPE");
    idtable.add_string("UNKNOWN");

    // Object
    idtable.add_string("abort");
    idtable.add_string("type_name");
    idtable.add_string("copy");

    // IO
    idtable.add_string("out_string");
    idtable.add_string("out_int");
    idtable.add_string("in_string");
    idtable.add_string("in_int");
    idtable.add_string("x");

    // String
    idtable.add_string("length");
    idtable.add_string("substr");
    idtable.add_string("i");
    idtable.add_string("l");
    idtable.add_string("concat");
    idtable.add_string("s");
}

Class* CreateObjectClass()
{
    auto nil_formals = new Formals;
    auto no_expr = new NoExpr;

    auto abort_method = new Method(Sym("abort"), nil_formals, Sym("Object"), no_expr);
    auto type_name_method = new Method(Sym("type_name"), nil_formals, Sym("String"), no_expr);
    auto copy_method = new Method(Sym("copy"), nil_formals, Sym("SELF_TYPE"), no_expr);

    auto features = new Features(abort_method);
    features->PushBack(type_name_method);
    features->PushBack(copy_method);

    return new Class(Sym("Object"), nullptr, features, Sym("UNKNOWN"));
}

Class* CreateIOClass()
{
    auto nil_formals = new Formals;
    auto no_expr = new NoExpr;

    auto out_string_method = new Method(
            Sym("out_string"),
            new Formals(new Formal(Sym("x"), Sym("String"))),
            Sym("SELF_TYPE"),
            no_expr);
    auto out_int_method = new Method(
            Sym("out_int"),
            new Formals(new Formal(Sym("x"), Sym("Int"))),
            Sym("SELF_TYPE"),
            no_expr);
    auto in_string_method = new Method(Sym("in_string"), nil_formals, Sym("String"), no_expr);
    auto in_int_method = new Method(Sym("in_int"), nil_formals, Sym("Int"), no_expr);

    auto features = new Features(out_string_method);
    features->PushBack(out_int_method);
    features->PushBack(in_string_method);
    features->PushBack(in_int_method);

    return new Class(Sym("IO"), Sym("Object"), features, Sym("UNKNOWN"));
}

Class* CreateIntClass()
{
    auto nil_features = new Features;
    return new Class(Sym("Int"), Sym("Object"), nil_features, Sym("UNKNOWN"));
}

Class* CreateStringClass()
{
    auto nil_formals = new Formals;
    auto no_expr = new NoExpr;

    auto substr_formals = new Formals(new Formal(Sym("i"), Sym("Int")));
    substr_formals->PushBack(new Formal(Sym("l"), Sym("Int")));

    auto length_method = new Method(Sym("length"), nil_formals, Sym("Int"), no_expr);
    auto concat_method = new Method(
            Sym("concat"),
            new Formals(new Formal(Sym("s"), Sym("String"))),
            Sym("String"),
            no_expr);
    auto in_string_method = new Method(Sym("substr"), substr_formals, Sym("String"), no_expr);

    auto features = new Features(length_method);
    features->PushBack(concat_method);
    features->PushBack(in_string_method);

    return new Class(Sym("String"), Sym("Object"), features, Sym("UNKNOWN"));
}

Class* CreateBoolClass()
{
    auto nil_features = new Features;
    return new Class(Sym("Bool"), Sym("Object"), nil_features, Sym("UNKNOWN"));
}