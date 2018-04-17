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
void AvlTree<T, less>::deleteFixup(typename AvlTree<T, less>::Node *node)
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
                x = g;
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
                    n = rotateRightBig(x);
                }
                else
                {
                    n = rotateRightSmall(x);
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
                x = g;
            }
        }
        x = g;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::swapParentChild(typename AvlTree<T, less>::Node *parent, typename AvlTree<T, less>::Node *child)
{
    if(!parent || !child || child->parent != parent || !(parent->left == child || parent->right == child))
    {
        assert(false);
        return;
    }

    auto parent_parent = parent->parent;
    auto parent_left = parent->left;
    auto parent_right = parent->right;
    
    auto child_left = child->left;
    auto child_right = child->right;

    if(child == parent_left)
    {
        parent->left = child_left;
        if(child_left)
            child_left->parent = parent;

        parent->right = child_right;
        if(child_right)
            child_right->parent = parent;

        child->parent = parent_parent;

        child->left = parent;
        parent->parent = child;
        
        child->right = parent_right;
        if(parent_right)
            parent_right->parent = child;
    }
    else
    {
        parent->left = child_left;
        if(child_left)
            child_left->parent = parent;

        parent->right = child_right;
        if(child_right)
            child_right->parent = parent;

        child->parent = parent_parent;
        
        child->right = parent;
        parent->parent = child;

        child->left = parent_left;
        if(parent_left)
            parent_left->parent = child;
    }
    if(parent_parent)
    {
        if(parent_parent->left == parent)
            parent_parent->left = child;
        else
            parent_parent->right = child;
    }

    auto balance = parent->balance;
    parent->balance = child->balance;
    child->balance = balance;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::swapNodes(typename AvlTree<T, less>::Node *first, typename AvlTree<T, less>::Node *second)
{
    if(!first || !second)
    {
        assert(false);
        return;
    }

    if(first->parent == second || second->parent == first)
    {
        if(first->parent == second)
            swapParentChild(second, first);
        else
            swapParentChild(first, second);
        return;
    }

    auto first_parent = first->parent;
    auto first_left = first->left;
    auto first_right = first->right;

    auto second_parent = second->parent;
    auto second_left = second->left;
    auto second_right = second->right;

    if(first_parent)
    {
        if(first_parent->left == first)
            first_parent->left = second;
        else if(first_parent->right == first)
            first_parent->right = second;
        else
            assert(0);
    }
    
    if(second_parent)
    {
        if(second_parent->left == second)
            second_parent->left = first;
        else if(second_parent->right == second)
            second_parent->right = first;
        else
            assert(0);
    }

    if(first_left)
    {
        first_left->parent = second;
    }

    if(first_right)
    {
        first_right->parent = second;
    }

    if(second_left)
    {
        second_left->parent = first;
    }

    if(second_right)
    {
        second_right->parent = first;
    }

    first->parent = second_parent;
    second->parent = first_parent;
    
    first->left = second_left;
    second->left = first_left;

    first->right = second_right;
    second->right = first_right;

    auto balance = first->balance;
    first->balance = second->balance;
    second->balance = balance;
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::deleteNode(typename AvlTree<T, less>::Node *node)
{
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
        deleteFixup(next);
        delete node;
        if(!invariant()) printf("errl");
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
        swapNodes(node, next);
    }

    if(!invariant())
    {
        printf("errinvbef\n");
    }

    deleteFixup(node);

    if(root && root->parent)
    {
        printf("errroot\n");
    }

    if(node->parent)
    {
        if(node->parent->left == node)
            node->parent->left = nullptr;
        else if(node->parent->right == node)
            node->parent->right = nullptr;
        else
            assert(false);
    }
    else if(root == node)
        root = next;
    else
        assert(false);
    if(!invariant())
    {
        assert(false);
        printf("INVERR\n");
    }
    delete node;
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

    if(start->parent == start || start->left == start || start->right == start)
        return -1;

    if(start->left && start->left->parent != start)
        return -1;

    if(start->right && start->right->parent != start)
        return -1;

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
    if(root->parent)
        return false;
    return !(invariant(root) < 0);
}
