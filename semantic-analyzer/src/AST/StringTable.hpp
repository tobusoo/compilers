#pragma once

#include <AST/Symbol.hpp>

#include <iostream>
#include <list>

template <typename T>
class StringTable {
public:
    using Container = std::list<T*>;

protected:
    Container* tbl = nullptr;
    int index = 0;

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;

    // add the prefix of s of length maxchars
    T* add_string(char* s, int maxchars)
    {
        int len = std::strlen(s);

        if (tbl == nullptr) {
            auto e = new T(s, len, index++);
            tbl = new Container(1, e);
            return e;
        }

        for (auto&& i : *tbl) {
            if (i->equal_string(s, len)) {
                return i;
            }
        }

        auto e = new T(s, len, index++);
        tbl->push_back(e);

        return e;
    }

    // add the (null terminated) string s
    T* add_string(char* s)
    {
        constexpr int MAXSIZE = 1000000;
        return add_string(s, MAXSIZE);
    }

    // add the string representation of an integer
    T* add_int(int i)
    {
        static char* buf = new char[20];
        std::snprintf(buf, 20, "%d", i);
        return add_string(buf);
    }

    T* lookup(int index)
    {
        for (auto&& i : *tbl) {
            if (i->equal_index(index)) {
                return i;
            }
        }
        return nullptr;
    }

    T* lookup_string(char* s)
    {
        int len = std::strlen(s);

        for (auto&& i : *tbl) {
            if (i->equal_string(s, len)) {
                return i;
            }
        }
        return nullptr;
    }
    void print(std::ostream& out)
    {
        if (tbl == nullptr) {
            out << "nil\n";
            return;
        }

        out << "[";
        for (auto&& i : *tbl) {
            out << (*i).get_string() << " ";
        }
        out << "]\n";
    }

    // Iterators methods
    iterator begin() noexcept
    {
        return tbl->begin();
    }

    iterator end() noexcept
    {
        return tbl->end();
    }

    const_iterator cbegin() const noexcept
    {
        return tbl->cbegin();
    }

    const_iterator cend() const noexcept
    {
        return tbl->cend();
    }

    reverse_iterator rbegin() noexcept
    {
        return tbl->rbegin();
    }

    reverse_iterator rend() noexcept
    {
        return tbl->rbegin();
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return tbl->crend();
    }

    const_reverse_iterator crend() const noexcept
    {
        return tbl->crend();
    }
    // end Iterators methods
};

class IdEntry : public Symbol {
public:
    IdEntry(char* s, int l, int i) : Symbol(s, l, i) {};
};

class StringEntry : public Symbol {
public:
    StringEntry(char* s, int l, int i) : Symbol(s, l, i) {};
};

class IntEntry : public Symbol {
public:
    IntEntry(char* s, int l, int i) : Symbol(s, l, i) {};
};

class IdTable : public StringTable<IdEntry> {};
class StrTable : public StringTable<StringEntry> {};
class IntTable : public StringTable<IntEntry> {};

extern IdTable idtable;
extern IntTable inttable;
extern StrTable stringtable;
