#pragma once

#include <iostream>
#include <list>

extern int node_lineno;
const char* pad(int n);

class BaseNode {
protected:
    size_t line_number;

public:
    BaseNode()
    {
        line_number = node_lineno;
    }

    size_t GetLineNumber() const noexcept
    {
        return line_number;
    }

    virtual ~BaseNode() = default;
    virtual void Dump(std::ostream& s, int pad) = 0;
    virtual void DumpWithTypes(std::ostream& s, int pad) = 0;
};

template <typename T>
class ListNode : public BaseNode {
public:
    using Container = std::list<T>;

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

    ListNode() : BaseNode()
    {
    }

    ListNode(T value) : v(1, value) {};

    ListNode(const ListNode<T>& l1, const ListNode<T>& l2) : v(l1.v)
    {
        for (auto&& i : static_cast<ListNode<T>>(l2)) {
            v.push_back(i);
        }
    }

    void PushBack(T&& value)
    {
        v.push_back(std::forward<T>(value));
    }

    void Dump(std::ostream& s, int n)
    {
        if (v.size() == 0) {
            return;
        }

        for (auto&& i : v) {
            i->Dump(s, n + 2);
        }
    }

    size_type size() const
    {
        return v.size();
    }

    bool empty() const
    {
        return v.empty();
    }

    auto find(T& value)
    {
        auto it = std::find(v.begin(), v.end(), value);
        return it;
    }

    void DumpWithTypes(std::ostream& s, int n)
    {
        if (v.size() == 0) {
            return;
        }

        for (auto&& i : v) {
            i->DumpWithTypes(s, n);
        }
    }

    // Iterators methods
    iterator begin() noexcept
    {
        return v.begin();
    }

    iterator end() noexcept
    {
        return v.end();
    }

    const_iterator cbegin() const noexcept
    {
        return v.cbegin();
    }

    const_iterator cend() const noexcept
    {
        return v.cend();
    }

    reverse_iterator rbegin() noexcept
    {
        return v.rbegin();
    }

    reverse_iterator rend() noexcept
    {
        return v.rend();
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return v.crbegin();
    }

    const_reverse_iterator crend() const noexcept
    {
        return v.crend();
    }
    // end Iterators methods

protected:
    Container v;
};