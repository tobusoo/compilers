#pragma once

#include <cstring>
#include <iostream>

const char* pad(int n);

static const std::string nil_str = "nil";

class Symbol {
protected:
    std::string str;
    // char* str; // string
    // int len;   // length of the string (without trailing \0)
    int index; // unique index for each string

public:
    Symbol(char* s, int l = 0, int i = 0) : index(i)
    {
        // str = new char[len + 1];
        // std::strncpy(str, s, len);
        // str[len] = '\0';
        str = s;
    }

    Symbol(const std::string& str_other) : str(str_other)
    {
    }

    int equal_string(char* s, int length) const
    {
        return str == s;
        // return (len == length) && (std::strncmp(str, s, len) == 0);
    }
    bool equal_index(int ind) const
    {
        return ind == index;
    }
    std::ostream& print(std::ostream& s) const
    {
        return s << "{" << str << ", " << str.size() << ", " << index << "}\n";
    }
    const std::string& get_string() const
    {
        return str;
    }
    size_t get_len() const
    {
        return str.size();
    }
    const std::string& GetString() const
    {
        return str.size() != 0 ? str : nil_str;
    }

    void Dump(std::ostream& s, int n)
    {
        s << pad(n) << GetString() << '\n';
    }
};

std::ostream& operator<<(std::ostream& s, const Symbol& sym);
