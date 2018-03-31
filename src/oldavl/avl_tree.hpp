#include <cassert>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <utility>

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

    if(newTop->balance == 0)
    {
        newTop->balance = -1;
        top->balance = 1;
    }
    else
    {
        newTop->balance = 0;
        newTop->balance = 0;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::rotateRightSmall(typename AvlTree<T, less>::Node *top)
{
    auto newTop = top->right;
    rotateRight(top);

    if(newTop->balance == 0)
    {
        newTop->balance = 1;
        top->balance = -1;
    }
    else
    {
        newTop->balance = 0;
        newTop->balance = 0;
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
            if(x->balance > 0)
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
            else if(x->balance < 0)
            {
                x->balance = 0;
                break;
            }
            x->balance = 1;
        }
        else
        {
            if(x->balance < 0)
            {
                if(z->balance > 0)
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
                if(x->balance > 0)
                {
                    x->balance = 0;
                    break;
                }
                x->balance = -1;
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
        node->balance = 0;
        insertFixup(node);
    }

    else if(less(parent->data, key))
    {
        auto node = new Node(std::move(key));
        parent->right = node;
        node->parent = parent;
        node->balance = 0;
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

/*template <typename T, bool less(const T& t1, const T& t2)>
T* AvlTree<T, less>::search(const T& key)
{
    auto node = getNodeByKey(key);

    if(!node)
        return nullptr;
    return node->find(key);
}*/

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::iterator AvlTree<T, less>::begin() const
{
    if(!root)
        return end();
    auto pos = root;
    while(pos->left)
        pos = pos->left;
    return iterator(pos);
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::iterator AvlTree<T, less>::end() const
{
    return iterator(root, nullptr, true);
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::iterator AvlTree<T, less>::lower_bound(const T& key) const
{
    if(!root)
        return end();

    auto pos = root;
    while(pos)
    {
        if(less(pos->data, key))
        {
            if(pos->right)
                pos = pos->right;
            else
                return ++iterator(pos);
        }
        else if(less(key, pos->data))
        {
            if(pos->left)
                pos = pos->left;
            else
                return iterator(pos);
        }
        else
            return iterator(pos);
    }
    return end(); // Supress warning. 
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::iterator AvlTree<T, less>::upper_bound(const T& key) const
{
    auto pos = lower_bound(key);
    if(pos != end())
        return pos.nextNode();
    return pos;
}
