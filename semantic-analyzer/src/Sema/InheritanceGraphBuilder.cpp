#include <Sema/Errors.hpp>
#include <Sema/InheritanceGraphBuilder.hpp>

#include <AST/Program.hpp>

#include <iomanip>
#include <unordered_set>

namespace sema {

bool IsBaseClass(std::string_view class_name)
{
    if (class_name == "Object") {
        return true;
    }
    if (class_name == "IO") {
        return true;
    }
    if (class_name == "Int") {
        return true;
    }
    if (class_name == "String") {
        return true;
    }
    if (class_name == "Bool") {
        return true;
    }
    return false;
}

bool CantInherits(std::string_view class_name)
{
    if (class_name == "Int") {
        return true;
    }
    if (class_name == "String") {
        return true;
    }
    if (class_name == "Bool") {
        return true;
    }
    return false;
}

InheritanceGraphBuilder::InheritanceGraphBuilder()
{
    class_map.insert({"Object", nullptr});
    class_map.insert({"IO", nullptr});
    class_map.insert({"Int", nullptr});
    class_map.insert({"String", nullptr});
    class_map.insert({"Bool", nullptr});

    inheritance_graph["IO"] = "Object";
    inheritance_graph["Int"] = "Object";
    inheritance_graph["String"] = "Object";
    inheritance_graph["Bool"] = "Object";
}

void InheritanceGraphBuilder::insertBaseClasses(StringSet& visited)
{
    visited.insert("Object");
    visited.insert("IO");
    visited.insert("Int");
    visited.insert("String");
    visited.insert("Bool");
}

void InheritanceGraphBuilder::Visit(Program* node)
{
    auto classes = node->GetClasses();
    for (auto&& class_ : *classes) {
        auto class_name = class_->GetName();
        auto parent_name = class_->GetParent();
        curr_filename = class_->GetFilename();

        if (CantInherits(parent_name)) {
            error(curr_filename,
                  Line(class_),
                  "class " + sema::quoted(class_name) + " inherits from forbidden class "
                          + sema::quoted(parent_name));
            continue;
        }

        auto it = class_map.insert({class_name, class_});
        if (it.second == false) {
            auto line = class_->GetLineNumber();
            error(curr_filename, Line(class_), "redefinition of class " + sema::quoted(class_name));
            continue;
        }

        inheritance_graph[class_name] = parent_name;
    }
}

void InheritanceGraphBuilder::checkCycles(
        const std::string& class_name, StringSet& visited, StringSet& recursion_stack)
{
    if (to_erase.contains(class_name)) {
        return;
    }

    if (recursion_stack.contains(class_name)) {
        auto class_ = GetClass(class_name);
        auto parent_name = class_->GetParent();
        error(curr_filename,
              Line(class_),
              "inheritance cycle detected involving class " + sema::quoted(class_name) + " and "
                      + sema::quoted(parent_name));
        to_erase.insert(class_name);
        to_erase.insert(parent_name);
    }

    auto it = visited.insert(class_name);
    if (it.second == false) {
        return;
    }
    recursion_stack.insert(class_name);

    auto class_it = inheritance_graph.find(class_name);
    auto parent = class_it->second;

    if (class_it != inheritance_graph.end()) {
        auto parent_class = GetClass(parent);
        if (!IsBaseClass(parent) && parent_class == nullptr) {
            auto class_ = GetClass(class_name);
            auto parent_name = class_->GetParent();

            error(curr_filename,
                  Line(class_),
                  "parent class " + sema::quoted(parent_name) + " not defined for class "
                          + sema::quoted(class_name));
            to_erase.insert(class_name);
            to_erase.insert(parent_name);
        }
    }

    checkCycles(parent, visited, recursion_stack);
    recursion_stack.erase(class_name);
}

void InheritanceGraphBuilder::topoSort(
        const std::string& class_name, StringSet& visited, StringVec& class_order)
{
    visited.insert(class_name);

    if (to_erase.contains(class_name)) {
        return;
    }

    auto class_it = inheritance_graph.find(class_name);
    if (class_it != inheritance_graph.end()) {
        auto parent_name = class_it->second;

        if (class_map.contains(parent_name) && !visited.contains(parent_name)) {
            topoSort(parent_name, visited, class_order);
        }
    }

    class_order.push_back(class_name);
}

StringVec InheritanceGraphBuilder::GetTopoSortedClasses()
{
    StringVec class_order;

    StringSet visited;
    StringSet recursion_stack;
    insertBaseClasses(visited);

    for (const auto& [class_name, _] : class_map) {
        checkCycles(class_name, visited, recursion_stack);
    }

    visited.clear();
    insertBaseClasses(visited);

    for (const auto& [class_name, _] : class_map) {
        if (!visited.contains(class_name)) {
            topoSort(class_name, visited, class_order);
        }
    }

    return class_order;
}

Class* InheritanceGraphBuilder::GetClass(const std::string& name) const
{
    auto it = class_map.find(name);
    if (it == class_map.end()) {
        return nullptr;
    }
    return it->second;
}

ClassMap& InheritanceGraphBuilder::GetClassMap()
{
    return class_map;
}

InheritanceGraph& InheritanceGraphBuilder::GetInheritanceGraph()
{
    return inheritance_graph;
}

} // namespace sema