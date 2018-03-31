template <typename T, bool less(const T& t1, const T& t2)>
class AvlTree<T, less>::RecordIterator
{
private:
    Node *pos = nullptr;
    bool end = false;
    typename Node::Inner *inn = nullptr;

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
            auto node = nextNode();
            if(!node)
                end = true;
            else
            {
                pos = node;
                inn = nullptr;
            }
        }
        return *this;
    }

    const RecordIterator &nextName()
    {
        auto tmp = nextNode();
    }

    RecordIterator &operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    const RecordIterator &operator--()
    {
        if(end)
            end = false;
        else if(inn && inn->prev)
            inn = inn->prev;
        else if(inn)
            inn = nullptr;
        else
        {
            pos = prevNode(pos);
            inn = nullptr;
        }
        return *this;
    }

    RecordIterator &prevName()
    {
        auto tmp = prevNode(pos);
    }

    RecordIterator &operator--(int)
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

    RecordIterator(Node *pos, typename Node::Inner *inn = nullptr) : pos(pos), inn(inn) {}

private:
    Node *nextNode()
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

    Node *prevNode()
    {
        auto start = pos;
        if(!start)
            return nullptr;

        auto cur = start;

        if(cur->left)
        {
            cur = cur->left;
            while(cur->right)
                cur = cur->right;
            return cur;
        }

        auto p = cur->parent;
        while(p && cur == p->left)
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
