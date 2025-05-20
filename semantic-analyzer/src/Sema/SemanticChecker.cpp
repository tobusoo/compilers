#include <Sema/Errors.hpp>
#include <Sema/SemanticChecker.hpp>

#include <AST/BaseClasses.hpp>
#include <AST/Class.hpp>
#include <AST/Program.hpp>

#include <cassert>

namespace sema {

SemanticChecker::SemanticChecker(StringVec&& classes, ClassMap&& map, InheritanceGraph&& inh)
    : classes(std::forward<StringVec>(classes)),
      class_map(std::forward<ClassMap>(map)),
      inheritance_graph(std::forward<InheritanceGraph>(inh))
{
    InitBaseClasses();
    auto object_class = CreateObjectClass();
    auto int_class = CreateIntClass();
    auto bool_class = CreateBoolClass();
    auto string_class = CreateStringClass();
    auto io_class = CreateIOClass();

    // Object
    class_map["Object"] = object_class;
    for (auto&& feature : *object_class->GetFeatures()) {
        auto attr = dynamic_cast<Attr*>(feature);
        if (attr == nullptr) {
            auto method = dynamic_cast<Method*>(feature);
            assert(method);
            auto method_name = method->GetName();
            methods_table["Object"][method_name] = method;
        } else {
            auto attr_name = attr->GetName();
            attrs_table["Object"][attr_name] = attr;
        }
    }
    attrs_table["Object"]["self"] = new Attr(new Symbol("self"), new Symbol("SELF_TYPE"), nullptr);

    ClassMap base_classes;
    base_classes["Int"] = int_class;
    base_classes["Bool"] = bool_class;
    base_classes["String"] = string_class;
    base_classes["IO"] = io_class;
    visited_class.insert("Object");
    visited_class.insert("Int");
    visited_class.insert("Bool");
    visited_class.insert("String");
    visited_class.insert("IO");

    for (auto&& [class_name, entry] : base_classes) {
        auto parent = entry->GetParent();
        if (parent != "") {
            methods_table[class_name] = methods_table[parent];
            attrs_table[class_name] = attrs_table[parent];
        }

        auto features = entry->GetFeatures();
        if (features->empty())
            continue;

        for (auto&& feature : *features) {
            auto attr = dynamic_cast<Attr*>(feature);
            if (attr == nullptr) {
                auto method = dynamic_cast<Method*>(feature);
                assert(method);
                auto method_name = method->GetName();
                methods_table[class_name][method_name] = method;
            } else {
                auto attr_name = attr->GetName();
                attrs_table[class_name][attr_name] = attr;
            }
        }
    }

    class_map.insert(base_classes.begin(), base_classes.end());
}

bool SemanticChecker::CanInherit(const std::string& child, const std::string& parent)
{
    if (child == parent || parent == "Object") {
        return true;
    }

    std::string current = child;
    while (current != "Object") {
        auto it = inheritance_graph.find(current);
        if (it == inheritance_graph.end())
            break;

        auto current_parent = it->second;
        if (current_parent == parent)
            return true;

        current = current_parent;
    }

    return false;
}

std::string SemanticChecker::JoinTypes(const std::string& typeA, const std::string& typeB)
{
    if (typeA == "SELF_TYPE")
        return typeB;
    if (typeB == "SELF_TYPE")
        return typeA;

    std::string current = typeA;
    while (current != "Object") {
        if (CanInherit(typeB, current)) {
            return current;
        }
        auto it = inheritance_graph.find(current);
        if (it == inheritance_graph.end())
            break;
        current = it->second;
    }

    return "Object";
}

void SemanticChecker::Visit(Program* node)
{
    for (auto&& entry : classes) {
        class_methods.clear();
        visited_class.insert(entry);
        Visit(class_map[entry]);
        class_methods.clear();
    }

    if (visited_class.find("Main") == visited_class.end()) {
        error("Program must contain Main class!");
    }
}

void SemanticChecker::Visit(Class* node)
{
    auto class_name = node->GetName();
    auto parent_name = node->GetParent();
    curr_filename = node->GetFilename();
    curr_class = class_name;

    // inherits features
    if (parent_name != "") {
        // features_table[class_name] = features_table[parent_name];
        methods_table[class_name] = methods_table[parent_name];
        attrs_table[class_name] = attrs_table[parent_name];
    }

    auto features = node->GetFeatures();
    if (features->empty()) {
        return;
    }

    // current_features = &features_table[class_name];
    current_methods = &methods_table[class_name];
    current_attrs = &attrs_table[class_name];

    for (auto&& feature : *features) {
        Visit(feature);
    }

    if (class_name == "Main") {
        auto it = current_methods->find("main");
        if (it == current_methods->end()) {
            error(curr_filename, Line(node), "class \"Main\" must contain \"main\" method");
            return;
        }

        auto method = (*it).second;
        if (!method->GetFormals()->empty()) {
            error(curr_filename, Line(node), "method \"main\" can't contain formal parameters");
        }
    } else if (class_name != "Main" && current_methods->contains("main")) {
        error(curr_filename,
              Line(node),
              "class " + sema::quoted(class_name) + " can't contain \"main\" method");
    }
}

void SemanticChecker::Visit(Feature* node)
{
    auto attr = dynamic_cast<Attr*>(node);
    if (attr) {
        Visit(attr);
    } else {
        Visit(dynamic_cast<Method*>(node));
    }
}

void SemanticChecker::Visit(Attr* node)
{
    auto name = node->GetName();
    auto type_decl = node->GetTypeDecl();
    if (name == "self") {
        error(curr_filename, Line(node), "\"self\" cannot be the name of a attribute");
        return;
    }

    if (!class_map.contains(type_decl)) {
        error(curr_filename,
              Line(node),
              "using undefined type " + sema::quoted(type_decl) + " for attribute "
                      + sema::quoted(name));
        return;
    }

    if (current_attrs->contains(name)) {
        error(curr_filename, Line(node), "redefiniton of " + sema::quoted(name) + " attribute");
        return;
    }

    auto body = node->GetExpr();
    Visit(body);
    auto body_type = body->GetType();

    if (body_type != "UNKNOWN" && body_type != "no_expr" && !CanInherit(body_type, type_decl)) {
        error(curr_filename,
              Line(node),
              "type declaration mismatch: expected " + type_decl + ", got " + body_type);
        return;
    }

    current_attrs->insert({name, node});
}

void SemanticChecker::Visit(Formal* node)
{
    auto name = node->GetName();
    auto type_decl = node->GetTypeDecl();

    if (name == "self") {
        error(curr_filename, Line(node), "\"self\" cannot be the name of a formal parameter");
        return;
    }
    if (current_formals.contains(name)) {
        error(curr_filename, Line(node), "redifinition of formal parameter " + sema::quoted(name));
        return;
    }
    if (!class_map.contains(type_decl)) {
        error(curr_filename,
              Line(node),
              "using undefined type " + sema::quoted(type_decl) + " for formal parameter "
                      + sema::quoted(name));
        return;
    }

    // Convert Formal to Feature
    auto sym_name = Sym(const_cast<char*>(name.c_str()));
    auto sym_type_decl = Sym(const_cast<char*>(type_decl.c_str()));
    auto feature = new Attr(sym_name, sym_type_decl, new NoExpr());

    current_attrs->insert_or_assign(name, feature);
    current_formals.insert(name);
}

bool SemanticChecker::CanRedefineMethod(Method* node, Method* parent)
{
    auto name = node->GetName();
    auto return_type = node->GetReturnType();
    auto parent_return_type = parent->GetReturnType();

    if (parent_return_type != return_type) {
        error(curr_filename,
              Line(node),
              "while redefinition of inherited method " + sema::quoted(name)
                      + " : return type mismatch: expected " + sema::quoted(parent_return_type)
                      + ", got " + sema::quoted(return_type));
        return false;
    }

    auto formals = node->GetFormals();
    auto parent_formals = parent->GetFormals();
    if (formals->size() != parent_formals->size()) {
        error(curr_filename,
              Line(node),
              "while redefinition of inherited method " + sema::quoted(name)
                      + " wrong number of arguments: expected "
                      + std::to_string(parent_formals->size()) + ", got "
                      + std::to_string(formals->size()));
        return false;
    }

    size_t idx = 0;
    bool res = true;

    auto p_formal = parent_formals->begin();
    for (auto&& arg : *formals) {
        Visit(arg);
        auto arg_type = arg->GetTypeDecl();
        auto formal_type = (*p_formal)->GetTypeDecl();

        if (arg_type != formal_type) {
            error(curr_filename,
                  Line(arg),
                  "argument " + std::to_string(idx + 1) + " type mismatch: expected "
                          + sema::quoted(formal_type) + ", got " + sema::quoted(arg_type));
            res = false;
        }
        p_formal++;
        idx++;
    }

    return res;
}

void SemanticChecker::Visit(Method* node)
{
    auto name = node->GetName();
    auto return_type = node->GetReturnType();
    if (return_type != "SELF_TYPE" && !class_map.contains(return_type)) {
        error(curr_filename,
              Line(node),
              "using undefined type " + sema::quoted(return_type) + " as return type for method "
                      + sema::quoted(name) + " of class " + sema::quoted(curr_class));
        return;
    }
    if (class_methods.contains(name)) {
        error(curr_filename,
              Line(node),
              "redefinition of method " + sema::quoted(name) + " in class "
                      + sema::quoted(curr_class));
        return;
    }

    //
    auto prev_current_attrs = current_attrs;
    auto copy_current_attrs = *current_attrs;
    current_attrs = &copy_current_attrs;
    //

    auto it = current_methods->find(name);
    if (it != current_methods->end() && !CanRedefineMethod(node, (*it).second)) {
        return;
    }

    current_formals.clear();
    auto formals = node->GetFormals();
    for (auto&& formal : *formals) {
        Visit(formal);
    }
    current_formals.clear();

    auto body = node->GetExpr();
    Visit(body);
    auto body_type = body->GetType();

    // method returns SELF_TYPE -> body_type = Class type
    if (body_type == "SELF_TYPE" && return_type != "SELF_TYPE") {
        body_type = class_map[curr_class]->GetName();
    }
    if (body_type != "UNKNOWN" && !CanInherit(body_type, return_type)) {
        error(curr_filename,
              Line(node),
              "return type mismatch: expected " + sema::quoted(return_type) + ", got "
                      + sema::quoted(body_type));
    }

    current_methods->insert({name, node});
    class_methods.insert(name);

    //
    current_attrs = prev_current_attrs;
    //
}

void SemanticChecker::Visit(Assign* node)
{
    auto var_name = node->GetName();

    if (var_name == "self") {
        error(curr_filename, Line(node), "Cannot assign to \"self\"");
        return;
    }

    auto expr = node->GetExpr();
    Visit(expr);

    auto it = current_attrs->find(var_name);
    if (it == current_attrs->end()) {
        error(curr_filename, Line(node), "undefined variable" + sema::quoted(var_name));
        return;
    }

    std::string var_type;
    auto feature = dynamic_cast<Attr*>((*it).second);
    if (feature != nullptr) {
        var_type = feature->GetTypeDecl();
    } else {
        error("unreachable " + std::to_string(__LINE__));
    }

    auto expr_type = expr->GetType();
    if (expr_type == "UNKNOWN") {
        return;
    }

    if (!CanInherit(expr_type, var_type)) {
        error(curr_filename,
              Line(node),
              "type mismatch in assignment: expected " + sema::quoted(var_type) + ", got "
                      + sema::quoted(expr_type));
    }
}

void SemanticChecker::Visit(Dispatch* node)
{
    auto name = node->GetName();
    auto expr = node->GetExpr();
    Visit(expr);
    auto expr_type = expr->GetType();

    if (expr_type == "UNKNOWN") {
        return;
    }

    Method* method = nullptr;
    if (expr_type == "SELF_TYPE") {
        auto method_it = current_methods->find(name);
        if (method_it == current_methods->end()) {
            error(curr_filename, Line(node), "using undefined method " + sema::quoted(name));
            return;
        }
        method = (*method_it).second;
    } else /* distpatch with defined variable */ {
        if (!visited_class.contains(expr_type)) {
            //
            auto prev_current_attrs = current_attrs;
            auto prev_current_methods = current_methods;

            //
            Visit(class_map[expr_type]);
            visited_class.insert(expr_type);
            //
            current_attrs = prev_current_attrs;
            current_methods = prev_current_methods;
            //
        }

        auto& methods = methods_table[expr_type];
        if (methods.size() == 0) {
            error("unreachable " + std::to_string(__LINE__));
        }

        auto method_it = methods.find(name);
        if (method_it == methods.end()) {
            error(curr_filename,
                  Line(node),
                  "dispatch to undefined method " + sema::quoted(name) + " in class "
                          + sema::quoted(expr_type));
            return;
        }
        method = (*method_it).second;
    }

    auto actuals = node->GetActual();
    auto formals = method->GetFormals();
    if (actuals->size() != formals->size()) {
        error(curr_filename,
              Line(node),
              "wrong number of arguments for method "
                      + sema::quoted(/*object->GetName() + "." +*/ name) + ": expected "
                      + std::to_string(formals->size()) + ", got "
                      + std::to_string(actuals->size()));
        return;
    }

    size_t idx = 0;
    auto formal = formals->begin();
    int failed_args = 0;

    for (auto&& actual : *actuals) {
        Visit(actual);
        auto actual_type = actual->GetType();
        auto formal_type = (*formal)->GetTypeDecl();

        if (!CanInherit(actual_type, formal_type)) {
            error(curr_filename,
                  Line(actual),
                  "in " + sema::quoted(/*object->GetName() + "." +*/ name) + " method argument "
                          + std::to_string(idx + 1) + " type mismatch: expected "
                          + sema::quoted(formal_type) + ", got " + sema::quoted(actual_type));
            failed_args++;
        }
        formal++;
        idx++;
    }

    if (failed_args > 0) {
        return;
    }

    auto ret_type = method->GetReturnType();
    if (ret_type == "SELF_TYPE") {
        if (expr_type == "SELF_TYPE")
            expr_type = curr_class;
        node->SetType(expr_type);
    } else {
        node->SetType(ret_type);
    }
}

void SemanticChecker::Visit(StaticDispatch* node)
{
    auto method_name = node->GetName();
    auto type_name = node->GetTypeName();
    auto expr = node->GetExpr();
    auto actuals = node->GetActual();

    if (!class_map.contains(type_name)) {
        error(curr_filename,
              Line(node),
              "static dispatch with undefined class " + sema::quoted(type_name));
        return;
    }

    Visit(expr);
    auto expr_type = expr->GetType();
    if (!CanInherit(expr_type, type_name)) {
        error(curr_filename,
              Line(node),
              "class " + sema::quoted(expr_type) + " is not an ancestor of class "
                      + sema::quoted(type_name));
        return;
    }

    auto methods_it = methods_table.find(type_name);
    if (methods_it == methods_table.end()) {
        error("unreachable " + std::to_string(__LINE__));
        return;
    }
    auto methods = (*methods_it).second;

    auto it = methods.find(method_name);
    if (it == methods.end()) {
        error(curr_filename,
              Line(node),
              "static dispatch to undefined method " + sema::quoted(method_name) + " in class "
                      + type_name);
        return;
    }

    auto method = (*it).second;
    auto formals = method->GetFormals();

    if (actuals->size() != formals->size()) {
        error(curr_filename,
              Line(node),
              "wrong number of arguments for method " + sema::quoted(type_name + "." + method_name)
                      + ": expected " + std::to_string(formals->size()) + ", got "
                      + std::to_string(actuals->size()));
        return;
    }

    size_t idx = 0;
    auto formal = formals->begin();
    for (auto&& actual : *actuals) {
        Visit(actual);
        auto actual_type = actual->GetType();
        auto formal_type = (*formal)->GetTypeDecl();

        if (!CanInherit(actual_type, formal_type)) {
            error(curr_filename,
                  Line(actual),
                  "Argument " + std::to_string(idx + 1) + " type mismatch: expected "
                          + sema::quoted(formal_type) + ", got " + sema::quoted(actual_type));
        }
        formal++;
        idx++;
    }

    auto ret_type = method->GetReturnType();
    if (ret_type == "SELF_TYPE") {
        node->SetType(expr_type);
    } else {
        node->SetType(ret_type);
    }
}

void SemanticChecker::Visit(Case* node)
{
    //
    auto prev_current_method = current_methods;
    auto copy_current_methods = *current_methods;
    current_methods = &copy_current_methods;
    auto prev_current_attrs = current_attrs;
    auto copy_current_attrs = *current_attrs;
    current_attrs = &copy_current_attrs;
    //

    auto name = node->GetName();
    auto type_decl = node->GetTypeDecl();
    auto expr = node->GetExpr();

    if (!class_map.contains(type_decl)) {
        error(curr_filename, Line(node), "undefined type: " + sema::quoted(type_decl));
        //
        current_methods = prev_current_method;
        current_attrs = prev_current_attrs;
        //
        return;
    }

    // Convert Case to Feature
    auto sym_name = Sym(const_cast<char*>(name.c_str()));
    auto sym_type_decl = Sym(const_cast<char*>(type_decl.c_str()));
    auto feature = new Attr(sym_name, sym_type_decl, new NoExpr());
    current_attrs->insert_or_assign(name, feature);

    Visit(expr);

    //
    current_methods = prev_current_method;
    current_attrs = prev_current_attrs;
    //
}

void SemanticChecker::Visit(TypeCase* node)
{
    auto expr = node->GetExpr();
    Visit(expr);

    std::string res_type;
    int count = 0;

    StringSet case_types;

    auto cases = *node->GetCases();
    for (auto&& case_ : cases) {
        Visit(case_);
        auto case_expr_type = case_->GetExpr()->GetType();
        auto case_type_decl = case_->GetTypeDecl();

        if (case_expr_type == "UNKNOWN")
            continue;
        if (case_types.contains(case_type_decl)) {
            sema::error(
                    curr_filename,
                    Line(node),
                    "duplicate " + sema::quoted(case_type_decl) + " branch in case statement");
            continue;
        }

        if (count == 0) {
            res_type = case_expr_type;
        } else {
            res_type = JoinTypes(res_type, case_expr_type);
        }
        count++;
        case_types.insert(case_type_decl);
    }

    node->SetType(res_type);
}

void SemanticChecker::Visit(Cond* node)
{
    auto pred = node->GetPred();
    auto then = node->GetThen();
    auto else_ = node->GetElse();

    Visit(pred);
    Visit(then);
    Visit(else_);

    auto pred_type = pred->GetType();
    auto then_type = then->GetType();
    auto else_type = else_->GetType();

    if (pred_type == "UNKNOWN")
        return;

    if (pred_type != "Bool") {
        error(curr_filename, Line(node), "condition must be Bool, got " + sema::quoted(pred_type));
        return;
    }

    node->SetType(JoinTypes(then_type, else_type));
}

void SemanticChecker::Visit(Loop* node)
{
    auto pred = node->GetPred();
    auto body = node->Getbody();

    Visit(pred);
    Visit(body);

    auto pred_type = pred->GetType();
    if (pred_type != "Bool") {
        error(curr_filename,
              Line(node),
              "loop condition must be Bool, got " + sema::quoted(pred_type));
    }
    node->SetType("Object");
}

void SemanticChecker::Visit(Block* node)
{
    auto exprs = node->GetExprs();

    //
    auto prev_current_attrs = current_attrs;
    auto copy_current_attrs = *current_attrs;
    current_attrs = &copy_current_attrs;
    //

    Expression* last_expr;
    for (auto&& expr : *exprs) {
        Visit(expr);
        last_expr = expr;
    }
    node->SetType(last_expr->GetType());

    //
    current_attrs = prev_current_attrs;
    //
}

void SemanticChecker::Visit(Let* node)
{
    auto name = node->GetID();
    auto type_decl = node->GetTypeDecl();
    auto init = node->GetInit();
    auto body = node->GetBody();

    if (name == "self") {
        error(curr_filename, Line(node), "\"self\" cannot be bound in a \"let\" expression");
        return;
    }

    if (!class_map.contains(type_decl)) {
        error(curr_filename, Line(node), "undefined type: " + sema::quoted(type_decl));
        return;
    }

    if (init && !dynamic_cast<NoExpr*>(init)) {
        Visit(init);
        auto init_type = init->GetType();

        if (!CanInherit(init_type, type_decl)) {
            error(curr_filename,
                  Line(node),
                  "type mismatch in let init: expected " + sema::quoted(type_decl) + ", got "
                          + sema::quoted(init_type));
        }
    }

    // Convert Let to Feature
    auto sym_name = Sym(const_cast<char*>(name.c_str()));
    auto sym_type_decl = Sym(const_cast<char*>(type_decl.c_str()));
    auto feature = new Attr(sym_name, sym_type_decl, new NoExpr());
    current_attrs->insert_or_assign(name, feature);

    Visit(body);
    auto body_type = body->GetType();
    if (body_type == "UNKNOWN") {
        return;
    }

    node->SetType(body_type);
}

void SemanticChecker::Visit(Plus* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();

    if (lhs_type == "UNKNOWN" || rhs_type == "UNKNOWN") {
        return;
    }

    if (lhs_type != "Int" || rhs_type != "Int") {
        error(curr_filename,
              Line(node),
              "operands of \"+\" must be Int: got " + sema::quoted(lhs_type) + " + "
                      + sema::quoted(rhs_type));
        return;
    }

    node->SetType(lhs->GetType());
}

void SemanticChecker::Visit(Sub* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();

    if (lhs_type == "UNKNOWN" || rhs_type == "UNKNOWN") {
        return;
    }

    if (lhs_type != "Int" || rhs_type != "Int") {
        error(curr_filename,
              Line(node),
              "operands of \"-\" must be Int: got " + sema::quoted(lhs_type) + " - "
                      + sema::quoted(rhs_type));
        return;
    }

    node->SetType(lhs->GetType());
}

void SemanticChecker::Visit(Mul* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();

    if (lhs_type == "UNKNOWN" || rhs_type == "UNKNOWN") {
        return;
    }

    if (lhs_type != "Int" || rhs_type != "Int") {
        error(curr_filename,
              Line(node),
              "operands of \"*\" must be Int: got " + sema::quoted(lhs_type) + " * "
                      + sema::quoted(rhs_type));
        return;
    }

    node->SetType(lhs->GetType());
}

void SemanticChecker::Visit(Divide* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();

    if (lhs_type == "UNKNOWN" || rhs_type == "UNKNOWN") {
        return;
    }

    if (lhs_type != "Int" || rhs_type != "Int") {
        error(curr_filename,
              Line(node),
              "operands of \"/\" must be Int: got " + sema::quoted(lhs_type) + " / "
                      + sema::quoted(rhs_type));
        return;
    }

    node->SetType(lhs->GetType());
}

void SemanticChecker::Visit(Neg* node)
{
    auto expr = node->GetExpr();
    Visit(expr);
    auto expr_type = expr->GetType();

    if (expr_type == "UNKNOWN")
        return;

    if (expr_type != "Int") {
        error(curr_filename,
              Line(node),
              "operand of \"~\" must be Int: got " + sema::quoted(expr_type));
        return;
    }

    node->SetType("Int");
}

void SemanticChecker::Visit(Lt* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();

    if (lhs_type != rhs_type || lhs_type != "Int") {
        error(curr_filename,
              Line(node),
              "operands of \"<\" not must be Int: got " + sema::quoted(lhs_type) + " < "
                      + sema::quoted(rhs_type));
        return;
    }

    node->SetType("Bool");
}

void SemanticChecker::Visit(Eq* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();

    auto baseFunc
            = [](std::string& str) { return str == "Int" || str == "String" || str == "Bool"; };

    if (lhs_type != rhs_type && (baseFunc(lhs_type) || baseFunc(rhs_type))) {
        error(curr_filename,
              Line(node),
              "operands of \"=\" must be comparable: got " + sema::quoted(lhs_type) + " = "
                      + sema::quoted(rhs_type));
        return;
    }

    node->SetType("Bool");
}

void SemanticChecker::Visit(Leq* node)
{
    auto lhs = node->GetLeftExpr();
    auto rhs = node->GetRightExpr();
    Visit(lhs);
    Visit(rhs);

    auto lhs_type = lhs->GetType();
    auto rhs_type = rhs->GetType();
    if (lhs_type != rhs_type || lhs_type != "Int") {
        error(curr_filename,
              Line(node),
              "operands of \"<=\" not must be Int: got " + sema::quoted(lhs_type)
                      + " <= " + sema::quoted(rhs_type));
        return;
    }

    node->SetType("Bool");
}

void SemanticChecker::Visit(Comp* node)
{
    auto expr = node->GetExpr();
    Visit(expr);

    auto expr_type = expr->GetType();
    if (expr_type != "Bool") {
        error(curr_filename, Line(node), "operand of NOT not must be Bool");
        return;
    }

    node->SetType("Bool");
}

void SemanticChecker::Visit(IntConst* node)
{
    node->SetType("Int");
}

void SemanticChecker::Visit(BoolConst* node)
{
    node->SetType("Bool");
}

void SemanticChecker::Visit(StringConst* node)
{
    node->SetType("String");
}

void SemanticChecker::Visit(New* node)
{
    auto type_name = node->GetTypeName();
    auto it = class_map.find(type_name);
    if (it == class_map.end()) {
        error(curr_filename, Line(node), "new with undefined class " + sema::quoted(type_name));
        return;
    }

    node->SetType(type_name);
}

void SemanticChecker::Visit(IsVoid* node)
{
    auto expr = node->GetExpr();
    Visit(node->GetExpr());
    node->SetType("Bool");
}

void SemanticChecker::Visit(NoExpr* node)
{
    node->SetType("no_expr");
};

void SemanticChecker::Visit(Object* node)
{
    auto name = node->GetName();
    auto it = current_attrs->find(name);
    if (it == current_attrs->end()) {
        error(curr_filename, Line(node), "undefined variable " + sema::quoted(name));
        return;
    }

    std::string var_type;
    auto feature = dynamic_cast<Attr*>((*it).second);
    if (feature != nullptr) {
        var_type = feature->GetTypeDecl();
    } else {
        error("unreachable " + std::to_string(__LINE__));
    }

    node->SetType(var_type);
}

void SemanticChecker::Visit(Expression* node)
{
    auto assign = dynamic_cast<Assign*>(node);
    if (assign) {
        Visit(assign);
        node->SetType(assign->GetExpr()->GetType());
        return;
    }

    auto static_dispatch = dynamic_cast<StaticDispatch*>(node);
    if (static_dispatch) {
        Visit(static_dispatch);
        node->SetType(static_dispatch->GetType());
        return;
    }

    auto dispatch = dynamic_cast<Dispatch*>(node);
    if (dispatch) {
        Visit(dispatch);
        node->SetType(dispatch->GetType());
        return;
    }

    auto cond = dynamic_cast<Cond*>(node);
    if (cond) {
        Visit(cond);
        node->SetType(cond->GetType());
        return;
    }

    auto loop = dynamic_cast<Loop*>(node);
    if (loop) {
        Visit(loop);
        node->SetType(loop->GetType());
        return;
    }

    auto typ_case = dynamic_cast<TypeCase*>(node);
    if (typ_case) {
        Visit(typ_case);
        node->SetType(typ_case->GetType());
        return;
    }

    auto block = dynamic_cast<Block*>(node);
    if (block) {
        Visit(block);
        node->SetType(block->GetType());
        return;
    }

    auto let = dynamic_cast<Let*>(node);
    if (let) {
        Visit(let);
        node->SetType(let->GetType());
        return;
    }

    auto plus = dynamic_cast<Plus*>(node);
    if (plus) {
        Visit(plus);
        node->SetType(plus->GetType());
        return;
    }

    auto sub = dynamic_cast<Sub*>(node);
    if (sub) {
        Visit(sub);
        node->SetType(sub->GetType());
        return;
    }

    auto mul = dynamic_cast<Mul*>(node);
    if (mul) {
        Visit(mul);
        node->SetType(mul->GetType());
        return;
    }

    auto div = dynamic_cast<Divide*>(node);
    if (div) {
        Visit(div);
        node->SetType(div->GetType());
        return;
    }

    auto neg = dynamic_cast<Neg*>(node);
    if (neg) {
        Visit(neg);
        node->SetType(neg->GetType());
        return;
    }

    auto lt = dynamic_cast<Lt*>(node);
    if (lt) {
        Visit(lt);
        node->SetType(lt->GetType());
        return;
    }

    auto eq = dynamic_cast<Eq*>(node);
    if (eq) {
        Visit(eq);
        node->SetType(eq->GetType());
        return;
    }

    auto leq = dynamic_cast<Leq*>(node);
    if (leq) {
        Visit(leq);
        node->SetType(leq->GetType());
        return;
    }

    auto comp = dynamic_cast<Comp*>(node);
    if (comp) {
        Visit(comp);
        node->SetType(comp->GetType());
        return;
    }

    auto int_const = dynamic_cast<IntConst*>(node);
    if (int_const) {
        Visit(int_const);
        node->SetType(int_const->GetType());
        return;
    }

    auto bool_const = dynamic_cast<BoolConst*>(node);
    if (bool_const) {
        Visit(bool_const);
        node->SetType(bool_const->GetType());
        return;
    }

    auto string_const = dynamic_cast<StringConst*>(node);
    if (string_const) {
        Visit(string_const);
        node->SetType(string_const->GetType());
        return;
    }

    auto new_ = dynamic_cast<New*>(node);
    if (new_) {
        Visit(new_);
        node->SetType(new_->GetType());
        return;
    }

    auto is_void = dynamic_cast<IsVoid*>(node);
    if (is_void) {
        Visit(is_void);
        node->SetType(is_void->GetType());
        return;
    }

    auto no_expr = dynamic_cast<NoExpr*>(node);
    if (no_expr) {
        Visit(no_expr);
        node->SetType(no_expr->GetType());
        return;
    }

    auto object = dynamic_cast<Object*>(node);
    if (object) {
        Visit(object);
        node->SetType(object->GetType());
        return;
    }

    error("unreachable " + std::to_string(__LINE__));
};

}; // namespace sema
