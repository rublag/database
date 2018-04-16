#include <cstddef>
#include <iterator>
#include <utility>

template <typename T>
DList<T>::~DList()
{
    clear();
}

template <typename T>
DList<T>::DList()
{
    ;
}

template <typename T>
DList<T>::DList(const DList &rhs)
{
}

template <typename T>
DList<T>::DList(DList &&rhs)
{
    swap(*this, rhs);
}

template <typename T>
DList<T> &DList<T>::operator=(DList rhs) &
{
    swap(*this, rhs);
    return *this;
}

template <typename T>
bool DList<T>::empty() const
{
    return first == nullptr;
}

template <typename T>
typename DList<T>::iterator DList<T>::begin() const
{
    if(!first)
        return iterator(first, true);
    return iterator(first, false);
}

template <typename T>
typename DList<T>::iterator DList<T>::end() const
{
    return iterator(last, true);
}

template <typename T>
typename DList<T>::reference DList<T>::front()
{
    return *begin();
}

template <typename T>
typename DList<T>::reference DList<T>::back()
{
    return *(end()--);
}

template <typename T>
typename DList<T>::iterator DList<T>::insert(iterator p, rvalue_reference t)
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

    if(p != end())
    {
        p.pos->prev = node;
    }
    else
    {
        last = node;
    }

    return iterator(node);
}

template <typename T>
typename DList<T>::iterator DList<T>::insert(rvalue_reference t)
{
    return insert(begin(), std::move(t));
}

template <typename T>
typename DList<T>::iterator DList<T>::erase(typename DList<T>::iterator q)
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
    else
    {
        last = last->prev;
    }

    delete pos;
    return next;
}

template <typename T>
typename DList<T>::iterator DList<T>::erase(iterator q1, iterator q2)
{
    while(q1 != q2)
    {
        q1 = erase(q1);
    }
    return q2;
}

template <typename T>
void DList<T>::clear()
{
    erase(begin(), end());
}

template <typename T>
void swap(DList<T> &a, DList<T> &b)
{
    using std::swap;
    swap(a.first, b.first);
    swap(a.last, b.last);
}
