#ifndef DLIST_H
#define DLIST_H

#include <cstddef>
#include <iterator>
#include <utility>

template <typename T>
class DList
{
private:
    struct Node;
    Node *first = nullptr;
    Node *last = nullptr;
    friend void swap(DList &a, DList &b);
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const reference;
    using rvalue_reference = value_type&&;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    class Iterator;

    DList() {}

    DList(const DList &rhs)
    {
    }

    DList(DList &&rhs)
    {
        swap(*this, rhs);
    }

    DList &operator=(DList rhs) &
    {
        swap(*this, rhs);
        return *this;
    }

    bool empty()
    {
        return first == nullptr;
    }

    iterator begin()
    {
        if(empty())
            return end();
        return iterator(first);
    }

    iterator end()
    {
        return iterator(last, true);
    }

    reference front()
    {
        return *begin();
    }

    reference back()
    {
        return *(end()--);
    }

    iterator insert(iterator p, rvalue_reference t)
    {
        auto pos = p.pos;
        auto node = new Node(std::move(t));

        if(p != end())
        {
            node->next = pos;
        }
        if(p != begin())
        {
            auto tmp = p;
            --tmp;
            node->prev = tmp.pos;
        }

        if(p != end())
        {
            p.pos->prev = node;
        }
        else
        {
            last = node;
        }

        if(p != begin())
        {
            auto tmp = p;
            --tmp;
            tmp.pos->next = node;
        }
        else
        {
            first = node;
        }
        return iterator(node);
    }

    iterator erase(iterator q)
    {
        auto pos = q.pos;
        auto next = q;
        ++next;

        if(q != begin())
        {
            pos->prev->next = pos->next;
        }
        else
        {
            first = first->next;
        }

        if(next != end())
        {
            next.pos->prev = pos->prev;
        }

        delete pos;
        return next;
    }

    iterator erase(iterator q1, iterator q2)
    {
        for(auto q = erase(q1); q != q2; q = erase(q))
        {
            ;
        }
        return q2;
    }

    iterator clear()
    {
        erase(begin(), end());
    }
};

void swap(DList &a, DList &b)
{
    using std::swap;
    swap(a.first, b.first);
    swap(a.last, b.last);
}

template <typename T>
class DList<T>::Iterator
{
private:
    friend class DList;
    Node *pos = nullptr;
    bool end = true;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    const Iterator &operator++()
    {
        if(pos->next == nullptr)
            end = true;
        else
            pos = pos->next;
        return *this;
    }

    Iterator &operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    const Iterator &operator--()
    {
        if(end)
            end = false;
        else
            pos = pos->prev;
        return *this;
    }

    Iterator &operator--(int)
    {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    reference operator*()
    {
        return pos->data;
    }

    pointer operator->()
    {
        return &operator*();
    }

    bool operator==(const_reference rhs) const
    {
        return pos == rhs.pos && end == rhs.end;
    }

    bool operator!=(const_reference rhs) const
    {
        return !operator==(rhs);
    }

    Iterator() {}
    Iterator(Node *pos, bool end = false) : pos(pos) {}
};

#endif
