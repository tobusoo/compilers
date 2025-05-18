#pragma once

#include <Sema/Visitor.hpp>

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sema {
using ClassMap = std::unordered_map<std::string, Class*>;
using InheritanceGraph = std::unordered_map<std::string, std::string>;
using StringSet = std::unordered_set<std::string>;
using StringVec = std::vector<std::string>;

bool IsBaseClass(std::string_view class_name);

class InheritanceGraphBuilder : public Visitor {
private:
    ClassMap class_map;
    InheritanceGraph inheritance_graph;
    StringSet to_erase;

    void insertBaseClasses(StringSet& visited);
    void checkCycles(const std::string& class_name, StringSet& visited, StringSet& recursion_stack);
    void topoSort(const std::string& class_name, StringSet& visited, StringVec& class_order);

public:
    InheritanceGraphBuilder();

    void Visit(Program* node) override;

    StringVec GetTopoSortedClasses();
    Class* GetClass(const std::string& name) const;
    InheritanceGraph& GetInheritanceGraph();
    ClassMap& GetClassMap();
};

} // namespace sema