#include <utility>
#include <stdexcept>
#include <assert.h>
#include <string.h>

template <typename T, bool less(const T& t1, const T& t2)>
class RbTree
{
friend class IndexName;
private:
    struct Node;
    Node *root;

public:
    RbTree();
    RbTree(const RbTree&) = delete;
    RbTree &operator=(const RbTree&) = delete;
    RbTree(RbTree&&) = delete;
    RbTree &operator=(RbTree&&) = delete;

    ~RbTree();

    void insert(T&& data);
    bool remove(const T& data);
    T *search(const T& data);

    template <typename Ti, bool lessi(const Ti& t1, const Ti& t2)>
    friend RbTree<Ti, lessi>::Node *first(typename RbTree<Ti, lessi>::Node *start);
    friend Node *next(Node *start);
    friend Node *prev(Node *prev);

private:
    void rotateLeft(Node *);
    void rotateRight(Node *);
    void insertFixup(Node *);
    void deleteAtMostOneChild(Node *);
    void deleteNode(Node *);
    Node *getNearestNodeByKey(const T& key);
    Node *getNodeByKey(const T& key);
};

template <typename T, bool less(const T& t1, const T& t2)>
struct RbTree<T, less>::Node
{
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

    enum Color : bool
    {
        Red,
        Black
    } color;

    T data;

    Node(T&& data) : data(std::move(data))
    {
        left = right = parent = nullptr;
        other = nullptr;
        color = Black;
    }
    Node()
    {
        left = right = parent = nullptr;
        other = nullptr;
        color = Black;
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
RbTree<T, less>::RbTree()
{
    root = nullptr;
}

template <typename T, bool less(const T& t1, const T& t2)>
RbTree<T, less>::~RbTree()
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
void RbTree<T, less>::rotateLeft(typename RbTree<T, less>::Node *top)
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
void RbTree<T, less>::rotateRight(typename RbTree<T, less>::Node *top)
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
void RbTree<T, less>::insertFixup(typename RbTree<T, less>::Node *top)
{
     while(top != root
           && top->color != Node::Black
           && top->parent->color != Node::Black)
     {
         auto parent = top->parent;
         auto grand = parent->parent;
         auto uncle = (grand->left == parent) ? grand->right : grand->left;

         if(uncle && uncle->color == Node::Red)
         {
             parent->color = uncle->color = Node::Black;
             grand->color = Node::Red;
             top = grand;
         }

         else if(parent == grand->left)
         {
             if(top == parent->right)
             {
                 rotateLeft(parent);
                 top = parent;
                 parent = top->parent;
             }
             rotateRight(grand);
             auto temp = grand->color;
             grand->color = parent->color;
             parent->color = temp;
             top = parent;
         }

         else
         {
             if(top == parent->left)
             {
                 rotateRight(parent);
                 top = parent;
                 parent = top->parent;
             }
             rotateLeft(grand);
             auto temp = grand->color;
             grand->color = parent->color;
             parent->color = temp;
             top = parent;
         }
     }
     root->color = Node::Black;
}

template <typename T, bool less(const T& t1, const T& t2)>
void RbTree<T, less>::deleteAtMostOneChild(typename RbTree<T, less>::Node *node)
{
    auto child = (node->left != nullptr)? node->left : node->right;
    auto parent = node->parent;

    auto phantom = node;
    if(!child)
        child = phantom;

    if(node->color == Node::Red && child == phantom)
    {
        child = nullptr;
    }

    if(parent == nullptr)
        root = child;
    else if(parent->left == node)
        parent->left = child;
    else
        parent->right = child;

    if(child)
        child->parent = node->parent;

    if(node->color == Node::Red)
    {
        return;
    }

    if(child == phantom)
    {
        child->left = child->right = nullptr;
    }
    node = child;

    assert(node);

    if(node->color == Node::Red)
    {
        node->color = Node::Black;
        return;
    }

    while(true)
    {
        auto parent = node->parent;

        /*
         * M: element to delete
         * C: M's child
         * N: C in new position
         * S: N's sibling
         * Sl: left S' child
         * Sr: right S' child
         * P: N's parent
         */

        /* Case 1: N is root. */
        if(parent == nullptr)
        {
            if(node == phantom)
            {
                root = nullptr;
            }
            return;
        }

        /* Case 2: S is red */
        parent = node->parent;
        auto sibling = (parent->left == node)? parent->right : parent->left;
        assert(sibling != nullptr);

        if(sibling->color == Node::Red)
        {
            parent->color = Node::Red;
            sibling->color = Node::Black;

            if(sibling == parent->right)
                rotateLeft(parent);
            else
                rotateRight(parent);
        }

        /* Case 3: P, S, Sl, Sr are black */
        parent = node->parent;
        sibling = (parent->left == node) ? parent->right : parent->left;
        assert(sibling != nullptr);

        if(parent->color == Node::Black
           && sibling->color == Node::Black
           && (!sibling->left || sibling->left->color == Node::Black)
           && (!sibling->right || sibling->right->color == Node::Black))
        {
            sibling->color = Node::Red;
            node = parent;
        }
        else
            break;
    }

    /* Case 4: P is red; S, Sl, Sr are black */
    parent = node->parent;
    auto sibling = (parent->left == node) ? parent->right : parent->left;
    assert(sibling != nullptr);

    if(parent->color == Node::Red
       && sibling->color == Node::Black
       && (!sibling->left || sibling->left->color == Node::Black)
       && (!sibling->right || sibling->right->color == Node::Black))
    {
        sibling->color = Node::Red;
        parent->color = Node::Black;

        return;
    }

    /* Case 5: one of S' child is red and path to it from P is left-right or right-left, the other, S are black */
    parent = node->parent;
    sibling = (parent->left == node) ? parent->right : parent->left;
    assert(sibling != nullptr);

    if(sibling->color == Node::Black)
    {
        if(sibling == parent->right
           && (!sibling->right || sibling->right->color == Node::Black)
           && (sibling->left && sibling->left->color == Node::Red))
        {
            sibling->color = Node::Red;
            sibling->left->color = Node::Black;
            rotateRight(sibling);
        }
        else if(sibling == parent->left
                && (!sibling->left || sibling->left->color == Node::Black)
                && (sibling->right && sibling->right->color == Node::Red))
        {
            sibling->color = Node::Red;
            sibling->right->color = Node::Black;
            rotateLeft(sibling);
        }
    }

    /* Case 6: one of S' children is red and path to it from P is left-left or right-right.
     *         the other, S are black*/
    parent = node->parent;
    sibling = (parent->left == node) ? parent->right : parent->left;
    assert(sibling != nullptr);

    sibling->color = parent->color;
    parent->color = Node::Black;

    if(sibling == parent->right)
    {
        assert(sibling->right);
        sibling->right->color = Node::Black;
        rotateLeft(parent);
    }
    else
    {
        assert(sibling->left);
        sibling->left->color = Node::Black;
        rotateRight(parent);
    }

}

template <typename T, bool less(const T& t1, const T& t2)>
void RbTree<T, less>::deleteNode(typename RbTree<T, less>::Node *node)
{
    auto next = node;
    if(node->right == nullptr)
    {
        next = node;
    }
    else
    {
        next = node->right;
        while(next->left != nullptr)
            next = next->left;

        node->data = std::move(next->data);
        node->other = next->other;
        next->other = nullptr;
    }

    deleteAtMostOneChild(next);
    
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
typename RbTree<T, less>::Node *RbTree<T, less>::getNearestNodeByKey(const T& key)
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
typename RbTree<T, less>::Node *RbTree<T, less>::getNodeByKey(const T& key)
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
void RbTree<T, less>::insert(T&& key)
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
bool RbTree<T, less>::remove(const T& key)
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
T* RbTree<T, less>::search(const T& key)
{
    auto node = getNodeByKey(key);

    if(!node)
        return nullptr;
    return node->find(key);
}
