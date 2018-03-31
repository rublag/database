#ifndef DLIST_H
#define DLIST_H

#include <cstddef>
#include <iterator>
#include <utility>

template <typename T>
class DList;

template <typename T>
void swap(DList<T> &a, DList<T> &b);

template <typename T>
class DList
{
private:
    struct Node;
    Node *first = nullptr;
    Node *last = nullptr;
    friend void swap<T>(DList<T> &a, DList<T> &b);
public:
    class Iterator;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using rvalue_reference = value_type&&;
    using pointer = value_type*;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    DList();
    DList(const DList &rhs);
    DList(DList &&rhs);
    DList &operator=(DList rhs) &;
    bool empty() const;
    iterator begin() const;
    iterator end() const;
    reference front();
    reference back();
    iterator insert(iterator p, rvalue_reference t);
    iterator insert(rvalue_reference t);
    iterator erase(iterator q);
    iterator erase(iterator q1, iterator q2);
    iterator clear();
};

template <typename T>
struct DList<T>::Node 
{
    Node *next;
    Node *prev;
    T data;
    Node(T &&data) : data(std::move(data)) {}
};

template <typename T>
class DList<T>::Iterator
{
private:
    friend class DList;
    Node *pos = nullptr;
    bool end = true;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    const Iterator &operator++();
    Iterator &operator++(int);
    const Iterator &operator--();
    Iterator &operator--(int);
    reference operator*() const;
    pointer operator->() const;
    bool operator==(const Iterator &rhs) const;
    bool operator!=(const Iterator &rhs) const;
    Iterator();
    Iterator(Node *pos, bool end = false);
};

template <typename T>
void swap(DList<T> &a, DList<T> &b);

#endif
