#include <cassert>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <utility>

template <typename T, bool less(const T& t1, const T& t2)>
class AvlTree
{
private:
    struct Node;
    Node *root;

public:
    class Iterator;
    using iterator = Iterator;
    AvlTree();
    AvlTree(const AvlTree&) = delete;
    AvlTree &operator=(const AvlTree&) = delete;
    AvlTree(AvlTree&&) = delete;
    AvlTree &operator=(AvlTree&&) = delete;

    ~AvlTree();

    iterator insert(T&& data);
    iterator remove(const T& data);
    T *search(const T& data);

private:
    void rotateLeftSmall(Node *);
    void rotateLeftBig(Node *);
    void rotateRightSmall(Node *);
    void rotateRightBig(Node *);
    void rotateLeft(Node *);
    void rotateRight(Node *);
    void insertFixup(Node *);
    void deleteLeaf(Node *);
    void deleteNode(Node *);
    Node *getNearestNodeByKey(const T& key);
    Node *getNodeByKey(const T& key);
};

template <typename T, bool less(const T& t1, const T& t2)>
struct AvlTree<T, less>::Node
{
    class Iterator;
    struct Inner
    {
        Inner *prev;
        Inner *next;
        T data;

        Inner(const Inner&) = delete;
        Inner &operator=(const Inner&) = delete;
        Inner() = delete;
        Inner(Inner&&) = delete;
        Inner &operator=(Inner&&) = delete;
        Inner(T &&other) : data(std::move(other))
        {
            prev = next = nullptr;
        }
    };
    Node *left;
    Node *right;
    Node *parent;
    Inner *other;

    int balance;

    T data;

    Node(T&& data) : data(std::move(data))
    {
        left = right = parent = nullptr;
        other = nullptr;
        balance = 0;
    }
    Node()
    {
        left = right = parent = nullptr;
        other = nullptr;
        balance = 0;
    }

    ~Node()
    {
        while(other)
        {
            auto next = other->next;
            other->next = nullptr;
            delete other;
            other = next;
        }
    }

    void insert(T&& data)
    {
        auto n = new Inner(std::move(data));
        if(other)
        {
            other->prev = n;
            n->next = other;
            other = n;
        }
        else
        {
            other = n;
        }
    }

    bool remove(const T& data)
    {
        assert(other);
        assert(other->data.name());
        assert(data.name());
        assert(this->data.name());
        if(this->data == data)
        {
            this->data = std::move(other->data);
            if(other->next)
            {
                other = other->next;
                delete other->prev;
                other->prev = nullptr;
            }
            else
            {
                delete other;
                other = nullptr;
            }
        }
        else
        {
            auto n = other;
            while(n && !(n->data == data))
            {
                n = n->next;
            }
            if(n)
            {
                if(n->prev)
                    n->prev->next = n->next;
                if(n->next)
                    n->next->prev = n->prev;
                if(!n->prev)
                    other = n->next;
                delete n;
            }
            else // We did not find element
                return false;
        }
        return true;
    }

    T* find(const T& data)
    {
        if(this->data == data)
            return &this->data;

        auto n = other;
        while(n && !(n->data == data))
            n = n->next;

        if(n)
            return &n->data;
        return nullptr;
    }

    Node(const Node&) = delete;
    Node &operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node &operator=(Node &&) = delete;
};

template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::AvlTree()
{
    root = nullptr;
}

template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::~AvlTree()
{
    auto t = root;
    if(!t) return;
    while(t->left || t->right || t->parent)
    {
        if(t->left)
            t = t->left;
        else if(t->right)
            t = t->right;
        else
        {
            auto d = t;
            t = t->parent;
            if(t->left == d)
                t->left = nullptr;
            else
                t->right = nullptr;
            delete d;
        }
    }
    if(t)
        delete t;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateLeft(typename AvlTree<T, less>::Node *top)
{
    assert(top->right != nullptr);
    Node *newTop = top->right;
    top->right = newTop->left;
    if(newTop->left != nullptr)
        newTop->left->parent = top;

    newTop->parent = top->parent;
    if(top->parent == nullptr)
        root = newTop;
    else
    {
        if(top == top->parent->left)
            top->parent->left = newTop;
        else
            top->parent->right = newTop;
    }
    newTop->left = top;
    top->parent = newTop;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateRight(typename AvlTree<T, less>::Node *top)
{
    assert(top->left != nullptr);
    Node *newTop = top->left;
    top->left = newTop->right;
    if(newTop->right != nullptr)
        newTop->right->parent = top;

    newTop->parent = top->parent;
    if(top->parent == nullptr)
        root = newTop;
    else
    {
        if(top == top->parent->left)
            top->parent->left = newTop;
        else
            top->parent->right = newTop;
    }
    newTop->right = top;
    top->parent = newTop;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateLeftSmall(typename AvlTree<T, less>::Node *top)
{
    auto newTop = top->left;
    rotateLeft(top);

    if(newTop.balance == 0)
    {
        newTop.balance = -1;
        top.balance = 1;
    }
    else
    {
        newTop.balance = 0;
        newTop.balance = 0;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateRightSmall(typename AvlTree<T, less>::Node *top)
{
    auto newTop = top->right;
    rotateRight(top);

    if(newTop.balance == 0)
    {
        newTop.balance = 1;
        top.balance = -1;
    }
    else
    {
        newTop.balance = 0;
        newTop.balance = 0;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateLeftBig(typename AvlTree<T, less>::Node *top)
{
    auto x = top;
    auto z = top->right;
    auto y = z->left;

    rotateRight(z);
    rotateLeft(x);

    if(y->balance > 0)
    {
        x->balance = -1;
        z->balance = 0;
    }
    else if(y->balance == 0)
    {
        x->balance = 0;
        z->balance = 0;
    }
    else
    {
        x->balance = 0;
        z->balance = 1;
    }

    y->balance = 0;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateRightBig(typename AvlTree<T, less>::Node *top)
{
    auto x = top;
    auto z = top->left;
    auto y = z->right;

    rotateRight(z);
    rotateLeft(x);

    if(y->balance < 0)
    {
        x->balance = +1;
        z->balance = 0;
    }
    else if(y->balance == 0)
    {
        x->balance = 0;
        z->balance = 0;
    }
    else
    {
        x->balance = 0;
        z->balance = -1;
    }

    y->balance = 0;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::insertFixup(typename AvlTree<T, less>::Node *top)
{
    auto z = top;
    auto x = top->parent;
    while(x)
    {
        auto g = x->parent;

        if(z == x->right)
        {
            if(x.balance > 0)
            {
                if(z->balance < 0)
                {
                    rotateLeftBig(x);
                }
                else
                {
                    rotateLeftSmall(x);
                }
            }
            else if(x.balance < 0)
            {
                x.balance = 0;
                break;
            }
            x.balance = 1;
        }
        else
        {
            if(x.balance < 0)
            {
                if(z.balance > 0)
                {
                    rotateRightBig(x);
                }
                else
                {
                    rotateRightSmall(x);
                }
            }
            else
            {
                if(x.balance > 0)
                {
                    x.balance = 0;
                    break;
                }
                x.balance = -1;
            }
        }
        z = x;
        x = g;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::deleteLeaf(typename AvlTree<T, less>::Node *node)
{
    auto n = node;
    auto x = n->parent;
    while(x)
    {
        auto g = x->parent;
        if(x->left == n)
        {
            if(x->balance > 0)
            {
                auto z = x->right;
                if(z->balance < 0)
                {
                    rotateLeftBig(x);
                }
                else
                {
                    rotateLeft(x);
                }
            }
            else
            {
                if(x->balance == 0)
                {
                    x->balance = 1;
                    break;
                }
                x->balance = 0;
            }
        }
        else
        {
            if(x->balance < 0)
            {
                auto z = x->left;
                if(z->balance > 0)
                {
                    rotateRightBig(x);
                }
                else
                {
                    rotateRight(x);
                }
            }
            else
            {
                if(x->balance == 0)
                {
                    x->balance = -1;
                    break;
                }
                x->balance = 0;
            }
        }
        x = x->parent;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::deleteNode(typename AvlTree<T, less>::Node *node)
{
    auto next = node;
    auto left = node->left;
    auto right = node->right;
    if(right == nullptr)
    {
        next = left;
    }
    else
    {
        next = node->right;
        while(next->left != nullptr)
            next = next->left;
    }

    if(next)
    {
        node->data = std::move(next->data);
        node->other = next->other;
        next->other = nullptr;
    }
    else
        next = node;

    deleteLeaf(next);
    
    if(next->parent)
    {
        if(next->parent->left == next)
            next->parent->left = nullptr;
        else if(next->parent->right == next)
            next->parent->right = nullptr;
    }
    else if(root == next)
        root = nullptr;
    delete next;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::Node *AvlTree<T, less>::getNearestNodeByKey(const T& key)
{
    auto n = root;
    while(n != nullptr)
    {
        if(less(key, n->data))
        {
            if(n->left == nullptr)
                return n;
            n = n->left;
        }
        else if(less(n->data, key))
        {
            if(n->right == nullptr)
                return n;
            n = n->right;
        }
        else
            return n;
    }
    return n;
}
template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::Node *AvlTree<T, less>::getNodeByKey(const T& key)
{
    auto n = root;
    while(n != nullptr)
    {
        if(less(key, n->data))
            n = n->left;
        else if(less(n->data, key))
            n = n->right;
        else
            return n;
    }
    return n;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::insert(T&& key)
{
    if(root == nullptr)
    {
        root = new Node(std::move(key));
        return;
    }

    auto parent = getNearestNodeByKey(key);
    if(!(less(key, parent->data) || less(parent->data, key)))
        parent->insert(std::move(key));

    else if(less(key, parent->data))
    {
        auto node = new Node(std::move(key));
        parent->left = node;
        node->parent = parent;
        node->color = Node::Red;
        insertFixup(node);
    }

    else if(less(parent->data, key))
    {
        auto node = new Node(std::move(key));
        parent->right = node;
        node->parent = parent;
        node->color = Node::Red;
        insertFixup(node);
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
bool AvlTree<T, less>::remove(const T& key)
{
    auto node = getNodeByKey(key);
    if(node == nullptr)
        return false;
    else if(node->other)
        return node->remove(key);
    else
    {
        deleteNode(node);
        return true;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
T* AvlTree<T, less>::search(const T& key)
{
    auto node = getNodeByKey(key);

    if(!node)
        return nullptr;
    return node->find(key);
}


template <typename T, bool less(const T& t1, const T& t2)>
class AvlTree::RecordIterator
{
private:
    Node *pos = nullptr;
    bool end = false;
    Node::Inner *inn = nullptr;

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
            auto node = nextNode(pos);
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
        auto tmp = nextNode(pos);
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
        auto tmp = prev
    }

    RecordIterator &operator--(int)
    {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    reference operator*()
    {
        return node->data;
    }

    pointer operator->()
    {
        return &node->data;
    }

    bool operator==(const rhs&) const
    {
        return (node == rhs.node) && (end == rhs.end);
    }

    bool operator!=(const rhs&) const
    {
        return !operator==(rhs);
    }

    RecordIterator() {}

    RecordIterator(Node *pos, Node::Inner *inn = nullptr) : pos(pos), inn(inn) {}

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
        if(!ptr)
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
}
