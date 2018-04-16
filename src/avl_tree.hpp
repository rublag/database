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
typename AvlTree<T, less>::Node *AvlTree<T, less>::rotateLeftSmall(typename AvlTree<T, less>::Node *top)
{
    auto newTop = top->right;
    rotateLeft(top);

    if(newTop->balance == 0)
    {
        newTop->balance = -1;
        top->balance = 1;
    }
    else
    {
        newTop->balance = 0;
        top->balance = 0;
    }
    return newTop;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::Node *AvlTree<T, less>::rotateRightSmall(typename AvlTree<T, less>::Node *top)
{
    auto newTop = top->left;
    rotateRight(top);

    if(newTop->balance == 0)
    {
        newTop->balance = 1;
        top->balance = -1;
    }
    else
    {
        newTop->balance = 0;
        top->balance = 0;
    }
    return newTop;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::Node *AvlTree<T, less>::rotateLeftBig(typename AvlTree<T, less>::Node *top)
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
    return y;
}

template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::Node *AvlTree<T, less>::rotateRightBig(typename AvlTree<T, less>::Node *top)
{
    auto x = top;
    auto z = top->left;
    auto y = z->right;

    rotateLeft(z);
    rotateRight(x);

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
    return y;
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
                break;
            }
            else
            {
                if(x->balance < 0)
                {
                    x->balance = 0;
                    break;
                }
                x->balance = 1;
                z = x;
                x = g;
            }
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
                break;
            }
            else
            {
                if(x->balance > 0)
                {
                    x->balance = 0;
                    break;
                }
                x->balance = -1;
                z = x;
                x = g;
            }
        }
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
                auto b = z->balance;
                if(b < 0)
                {
                    n = rotateLeftBig(x);
                }
                else
                {
                    n = rotateLeftSmall(x);
                }
                if(b == 0)
                    break;
            }
            else
            {
                if(x->balance == 0)
                {
                    x->balance = 1;
                    break;
                }
                n = x;
                x->balance = 0;
                x = x->parent;
            }
        }
        else
        {
            if(x->balance < 0)
            {
                auto z = x->left;
                auto b = z->balance;
                if(b > 0)
                {
                    rotateRightBig(x);
                }
                else
                {
                    rotateRightSmall(x);
                }
                if(b == 0)
                    break;
            }
            else
            {
                if(x->balance == 0)
                {
                    x->balance = -1;
                    break;
                }
                n = x;
                x->balance = 0;
                x = x->parent;
            }
        }
        x = g;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::deleteNode(typename AvlTree<T, less>::Node *node)
{
/*
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
*/
    auto next = node;
    auto parent = node->parent;
    auto left = node->left;
    auto right = node->right;
    
    if(right == nullptr && left)
    {
        next = left;
        
        next->parent = parent;
        if(parent)
        {
            if(parent->left == node)
                parent->left = next;
            else if(parent->right == node)
                parent->right = next;
        }
        else if(root == node)
            root = next;
        deleteLeaf(next);
        delete node;
        if(!invariant()) printf("err");;
        return;
    }
    else if(right)
    {
        next = node->right;
        while(next->left != nullptr)
            next = next->left;
    }
    else
    {
        next = nullptr;
    }

    if(next)
    {
        if(node->left)
        {
            node->left->parent = next;
        }
        if(node->parent)
        {
            if(node->parent->left == node)
                node->parent->left = next;
            else if(node->parent->right == node)
                node->parent->right = next;
        }

        if(next->parent == node)
        {
            next->parent = node->parent;
            node->parent = next;
            if(node->left == next)
            {
                node->left = next->left;
                node->right = next->right;
                next->left = node;
                next->right = right;
            }
            else if(node->right == next)
            {
                node->left = next->left;
                node->right = next->right;
                next->right = node;
                next->left = left;
            }
        }
        else if(next->parent)
        {
            if(next->parent->left == next)
                next->parent->left = node;
            else if(next->parent->right == next)
                next->parent->right = node;
    
            node->parent = next->parent;
            node->left = next->left;
            node->right = next->right;

            next->parent = parent;
            next->left = left;
            next->right = right;
        }

        auto bal = node->balance;
        node->balance = next->balance;
        next->balance = bal;
    }

    deleteLeaf(node);

    if(node->parent)
    {
        if(node->parent->left == node)
            node->parent->left = nullptr;
        else if(node->parent->right == node)
            node->parent->right = nullptr;
    }
    else if(root == node)
        root = next;
    delete node;
    if(!invariant()) printf("err");;
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
    typename Node::Inner *ignore;
    auto node = getNodeByKey(key);
    if(node == nullptr)
        return false;
    else if(node->other)
        return node->remove(key, ignore);
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

template <typename T, bool less(const T& t1, const T& t2)>
template <typename Key>
typename AvlTree<T, less>::iterator AvlTree<T,less>::lower_bound(const Key &key) const
{
    if(!root)
        return end();

    auto pos = root;
    while(pos)
    {
        if(pos->data < key)
        {
            if(pos->right)
                pos = pos->right;
            else
                return ++iterator(pos);
        }
        else if(key < pos->data)
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
template <typename Key>
typename AvlTree<T, less>::iterator AvlTree<T, less>::upper_bound(const Key& key) const
{
    auto pos = lower_bound(key);
    if(pos != end())
        return pos.nextNode();
    return pos;
}

    template <typename T, bool less(const T& t1, const T& t2)>
typename AvlTree<T, less>::iterator AvlTree<T, less>::erase(typename AvlTree<T, less>::iterator it)
{
    auto node = getNodeByKey(*it);
    if(node == nullptr)
    {
        ++it;
        return it;
    }
    else if(node->other)
    {
        typename Node::Inner *inn = nullptr;
        if(node->remove(*it, inn))
            return iterator(node, inn);
        ++it;
        return it;
    }
    else
    {
        ++it;
        deleteNode(node);
        return it;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
template <typename Key>
typename AvlTree<T, less>::iterator AvlTree<T, less>::find(const Key& key) const
{
    auto pos = lower_bound(key);
    auto node = pos.pos;
    while(!pos.atEnd() && pos.pos == node)
    {
        if(*pos == key)
            return pos;
        ++pos;
    }
    return end();
}

    template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::erase(const T& key)
{
    remove(key);
}

    template <typename T, bool less(const T& t1, const T& t2)>
int AvlTree<T, less>::invariant(typename AvlTree<T, less>::Node *start)
{
    if(!start)
        return 0;

    auto lh = invariant(start->left);
    auto rh = invariant(start->right);
    if(lh < 0 || rh < 0)
        return -1;
    if(rh - lh != start->balance)
        return -1;

    if(lh > rh)
        return lh + 1;
    return rh + 1;
}

    template <typename T, bool less(const T& t1, const T& t2)>
bool AvlTree<T, less>::invariant()
{
    return !(invariant(root) < 0);
}
