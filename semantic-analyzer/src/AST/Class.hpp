#pragma once

#include <AST/BaseNode.hpp>
#include <AST/Expressions.hpp>
#include <AST/Feature.hpp>
#include <AST/Symbol.hpp>

#include <Util/Print.hpp>

class Class;
using Classes = ListNode<Class*>;

class Class : public BaseNode {
protected:
    Symbol* name;
    Symbol* parent;
    Features* features;
    Symbol* filename;

public:
    friend Class* CreateObjectClass();

    Class(Symbol* a1, Symbol* a2, Features* a3, Symbol* a4)
        : name(a1), parent(a2), features(a3), filename(a4) {};

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "class\n";
        name->Dump(stream, n + 2);
        parent->Dump(stream, n + 2);
        stream << pad(n + 2) << "\"";
        print_escaped_string(stream, filename->get_string().c_str());
        stream << "\"\n" << pad(n + 2) << "(\n";
        features->Dump(stream, n + 2);
        stream << pad(n + 2) << ")\n";
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "class\n";
        name->Dump(stream, n + 2);
        parent->Dump(stream, n + 2);
        stream << pad(n + 2) << "\"";
        print_escaped_string(stream, filename->get_string().c_str());
        stream << "\"\n" << pad(n + 2) << "(\n";
        features->DumpWithTypes(stream, n + 2);
        stream << pad(n + 2) << ")\n";
    }

    std::string GetName()
    {
        return name->get_string();
    }

    std::string GetParent()
    {
        if (parent == nullptr)
            return "";
        return parent->get_string();
    }

    std::string GetFilename()
    {
        return filename->get_string();
    }

    Features* GetFeatures()
    {
        return features;
    }

    void Accept(sema::Visitor* v)
    {
        v->Visit(this);
    }
};