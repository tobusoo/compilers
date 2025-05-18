#pragma once

#include <AST/Class.hpp>
#include <Util/Print.hpp>

#include <memory>
#include <set>
#include <string>
#include <unordered_map>

class InheritsGraph {
public:
    InheritsGraph()
    {
        g["Object"] = {"IO", "Int", "String", "Bool", "SELF_TYPE"};

        classes["Object"] = nullptr;
        classes["IO"] = nullptr;
        classes["Int"] = nullptr;
        classes["String"] = nullptr;
        classes["Bool"] = nullptr;
        classes["SELF_TYPE"] = nullptr;

        parents["IO"] = "Object";
        parents["Int"] = "Object";
        parents["String"] = "Object";
        parents["Bool"] = "Object";
        parents["SELF_TYPE"] = "Object";
    }

    // name and parent must be uniq. Check them by Contains method
    bool Insert(std::string name, std::string parent, Class* class_ptr)
    {
        g[parent].insert(name);
        parents[name] = parent;

        classes[name] = class_ptr;
        return true;
    }

    std::pair<Class*, bool> Contains(std::string name)
    {
        auto it = classes.find(name);
        if (it == classes.end())
            return {nullptr, false};

        return {(*it).second, true};
    }

    void Dump(std::ostream& out)
    {
        int n = 0;

        for (auto&& parent : g) {
            out << parent.first << ":\n";
            out << pad(n);

            for (auto&& child : parent.second) {
                out << pad(n + 2) << child << '\n';
            }
        }
    }

private:
    std::unordered_map<std::string, std::set<std::string>> g;

    std::unordered_map<std::string, Class*> classes;
    std::unordered_map<std::string, std::string> parents;
};
