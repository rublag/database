template <typename T, bool less(const T& t1, const T& t2)>
class AvlTree<T, less>::RecordIterator
{
    friend class AvlTree<T, less>;
private:
    Node *pos = nullptr;
    typename Node::Inner *inn = nullptr;
    bool end = false;

public:
    // Iterator traits:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;
    const RecordIterator &operator++()
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

    const RecordIterator &nextNode()
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

    reference operator*()
    {
        if(inn)
            return inn->data;
        return pos->data;
    }

    pointer operator->()
    {
        return &pos->data;
    }

    bool operator==(const RecordIterator &rhs) const
    {
        return (pos == rhs.pos) && (end == rhs.end);
    }

    bool operator!=(const RecordIterator &rhs) const
    {
        return !operator==(rhs);
    }

    RecordIterator() {}

    RecordIterator(Node *pos, typename Node::Inner *inn = nullptr, bool atEnd = false) : pos(pos), inn(inn), end(atEnd) {}

    bool atEnd() const
    {
        return end;
    }

private:
    Node *nextNodePtr()
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
};


template <typename T, bool less(const T&, const T&)>
class AvlTree<T, less>::Node::Iterator
{
    Node *node = nullptr;
    Inner *pos = nullptr;
    bool end = false;

public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    const Iterator &operator++()
    {
        if(!pos && node->other)
            pos = node->other;
        else if(pos && pos->next)
            pos = pos->next;
        else
            end = true;

        return (*this);
    }

    Iterator &operator++(int)
    {
        auto tmp = (*this);
        ++(*this);
        return tmp;
    }

    const Iterator &operator--()
    {
        if(end)
            end = false;
        else if(pos && pos->prev)
            pos = pos->prev;
        else
            pos = nullptr;
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
        if(!pos)
            return node->data;
        return pos->data;
    }

    pointer operator->()
    {
        return &(operator*());
    }

    bool operator==(const Iterator &rhs) const
    {
        return node == rhs.node && end == rhs.end && ((end && rhs.end) || pos == rhs.pos);
    }

    bool operator!=(const Iterator &rhs) const
    {
        return !operator==(rhs);
    }

    Iterator() {}

    Iterator(Node *node) : node(node) {}
};
