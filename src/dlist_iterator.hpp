template <typename T>
const typename DList<T>::Iterator &DList<T>::Iterator::operator++()
{
    if(pos->next == nullptr)
        end = true;
    else
        pos = pos->next;
    return *this;
}

template <typename T>
typename DList<T>::Iterator &DList<T>::Iterator::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T>
const typename DList<T>::Iterator &DList<T>::Iterator::operator--()
{
    if(end)
        end = false;
    else
        pos = pos->prev;
    return *this;
}

template <typename T>
typename DList<T>::Iterator &DList<T>::Iterator::operator--(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

template <typename T>
typename DList<T>::reference DList<T>::Iterator::operator*() const
{
    return pos->data;
}

template <typename T>
typename DList<T>::pointer DList<T>::Iterator::operator->() const
{
    return &operator*();
}

template <typename T>
bool DList<T>::Iterator::operator==(const Iterator &rhs) const
{
    return pos == rhs.pos && end == rhs.end;
}

template <typename T>
bool DList<T>::Iterator::operator!=(const Iterator &rhs) const
{
    return !operator==(rhs);
}

template <typename T>
DList<T>::Iterator::Iterator() {}

template <typename T>
DList<T>::Iterator::Iterator(Node *pos, bool end) : pos(pos), end(end) {}

template <typename T>
bool DList<T>::Iterator::atEnd()
{
    return end;
}
