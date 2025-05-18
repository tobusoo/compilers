#pragma once

#include <AST/Class.hpp>

class Program : public BaseNode {
protected:
    Classes* classes;

public:
    Program() = default;
    Program(Classes* a1) : classes(a1) {};

    void Dump(std::ostream& stream, int n)
    {
        stream << pad(n) << "program\n";
        classes->Dump(stream, n + 2);
    }

    void DumpWithTypes(std::ostream& stream, int n)
    {
        stream << pad(n) << "line:" << line_number << '\n';
        stream << pad(n) << "program\n";
        classes->DumpWithTypes(stream, n + 2);
    }

    void Accept(sema::Visitor* v)
    {
        v->Visit(this);
    }

    void SetClasses(Classes* classes_)
    {
        classes = classes_;
    }

    Classes* GetClasses()
    {
        return classes;
    }
};