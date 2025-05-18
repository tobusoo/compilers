#pragma once

#include <Sema/Visitor.hpp>

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sema {

using ClassMap = std::unordered_map<std::string, Class*>;
using FeaturesMap = std::unordered_map<std::string, Feature*>;
using AttrsMap = std::unordered_map<std::string, Attr*>;
using MethodsMap = std::unordered_map<std::string, Method*>;
using InheritanceGraph = std::unordered_map<std::string, std::string>;

using FTable = std::unordered_map<std::string, FeaturesMap>;
using AttrsTable = std::unordered_map<std::string, AttrsMap>;
using MethodsTable = std::unordered_map<std::string, MethodsMap>;
using StringVec = std::vector<std::string>;
using StringSet = std::unordered_set<std::string>;

class SemanticChecker : public Visitor {
private:
    // Vec of Classes in topological order
    StringVec classes;
    // Map<class_name, Class*> including Base classes
    ClassMap class_map;

    InheritanceGraph inheritance_graph;

    AttrsTable attrs_table;
    MethodsTable methods_table;

    MethodsMap* current_methods = nullptr;
    AttrsMap* current_attrs = nullptr;

    std::string curr_filename;
    std::string curr_class;

    StringSet class_methods;
    StringSet current_formals;
    StringSet visited_class;

    bool CanInherit(const std::string& child, const std::string& parent);
    std::string JoinTypes(const std::string& typeA, const std::string& typeB);
    bool CanRedefineMethod(Method* node, Method* parent);

public:
    SemanticChecker(StringVec&& classes, ClassMap&& map, InheritanceGraph&& inh);

    void Visit(Program* node) override;
    void Visit(Class* node) override;
    void Visit(Attr* node) override;
    void Visit(Method* node) override;
    void Visit(Feature* node) override;
    void Visit(Formal* node) override;

    void Visit(Expression* node) override;

    void Visit(Case* node) override;
    void Visit(Assign* node) override;
    void Visit(StaticDispatch* node) override;
    void Visit(Dispatch* node) override;
    void Visit(Cond* node) override;
    void Visit(Loop* node) override;
    void Visit(TypeCase* node) override;
    void Visit(Block* node) override;
    void Visit(Let* node) override;
    void Visit(Plus* node) override;
    void Visit(Sub* node) override;
    void Visit(Mul* node) override;
    void Visit(Divide* node) override;
    void Visit(Neg* node) override;
    void Visit(Lt* node) override;
    void Visit(Eq* node) override;
    void Visit(Leq* node) override;
    void Visit(Comp* node) override;
    void Visit(IntConst* node) override;
    void Visit(BoolConst* node) override;
    void Visit(StringConst* node) override;
    void Visit(New* node) override;
    void Visit(IsVoid* node) override;
    void Visit(NoExpr* node) override;
    void Visit(Object* node) override;
};

}; // namespace sema
