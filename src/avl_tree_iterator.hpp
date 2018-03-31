template <typename T, bool less(const T& t1, const T& t2)>
const typename AvlTree<T, less>::RecordIterator &AvlTree<T, less>::RecordIterator::operator++()
{
    if(!inn && pos->other)
    {
        inn = pos->other;
    }
    else if(inn && inn->next)
    {
        inn = inn->next;
    }
    else
    {
        return nextNode();
    }
    return *this;
}

template <typename T, bool less(const T& t1, const T& t2)>
const typename AvlTree<T, less>::RecordIterator &AvlTree<T, less>::RecordIterator::nextNode()
{
    auto tmp = nextNodePtr();
    if(!tmp)
        end = true;
    else
    {
        pos = tmp;
        inn = nullptr;
    }
    return *this;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::RecordIterator::reference AvlTree<T, less>::RecordIterator::operator*()
{
    if(inn)
        return inn->data;
    return pos->data;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::RecordIterator::pointer AvlTree<T, less>::RecordIterator::operator->()
{
    return &pos->data;
}

template <typename T, bool less(const T& t1, const T& t2)>
bool AvlTree<T, less>::RecordIterator::operator==(const typename AvlTree<T, less>::RecordIterator &rhs) const
{
    return (pos == rhs.pos) && (end == rhs.end);
}

template <typename T, bool less(const T& t1, const T& t2)>
bool AvlTree<T, less>::RecordIterator::operator!=(const typename AvlTree<T, less>::RecordIterator &rhs) const
{
    return !operator==(rhs);
}

template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::RecordIterator::RecordIterator() {}

template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::RecordIterator::RecordIterator(typename AvlTree<T, less>::Node *pos, typename AvlTree<T, less>::Node::Inner *inn, bool atEnd) : pos(pos), inn(inn), end(atEnd) {}

template <typename T, bool less(const T& t1, const T& t2)>
bool AvlTree<T, less>::RecordIterator::atEnd() const
{
    return end;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::Node *AvlTree<T, less>::RecordIterator::nextNodePtr()
{
    auto start = pos;
    if(!start)
        return nullptr;

    auto cur = start;

    if(cur->right)
    {
        cur = cur->right;
        while(cur->left)
            cur = cur->left;
        return cur;
    }

    auto p = cur->parent;
    while(p && cur == p->right)
    {
        cur = p;
        p = p->parent;
    }
    return p;
}
