#ifndef AVL_TREE_H
#define AVL_TREE_H
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
    int invariant(Node *start);
    bool invariant();
    class Iterator;
    class RecordIterator;
    using iterator = RecordIterator;
    AvlTree();
    AvlTree(const AvlTree&) = delete;
    AvlTree &operator=(const AvlTree&) = delete;
    AvlTree(AvlTree&&) = delete;
    AvlTree &operator=(AvlTree&&) = delete;

    ~AvlTree();

    void insert(T&& data);
    bool remove(const T& data);
    void erase(const T& data);
    T *search(const T& data);

    iterator begin() const;
    iterator end() const;

    iterator lower_bound(const T& data) const;
    iterator upper_bound(const T& data) const;

    template <typename Key>
    iterator lower_bound(const Key &key) const;
    template <typename Key>
    iterator upper_bound(const Key &key) const;
    iterator erase(iterator it);
    template <typename Key>
    iterator find(const Key &key) const;
private:
    Node *rotateLeftSmall(Node *);
    Node *rotateLeftBig(Node *);
    Node *rotateRightSmall(Node *);
    Node *rotateRightBig(Node *);
    void rotateLeft(Node *);
    void rotateRight(Node *);
    void insertFixup(Node *);
    void deleteFixup(Node *);
    void deleteNode(Node *);
    Node *getNearestNodeByKey(const T& key);
    Node *getNodeByKey(const T& key);
    void swapNodes(Node *, Node *);
    void swapParentChild(Node *, Node *);
};

template <typename T, bool less(const T& t1, const T& t2)>
struct AvlTree<T, less>::Node
{
    class Iterator;
    struct Inner;
    Node *left;
    Node *right;
    Node *parent;
    Inner *other;

    int balance;

    T data;

    Node(T&& data);
    Node();
    ~Node();
    void insert(T&& data);
    bool remove(const T& data, Inner *&inn);
    T* find(const T& data);
    Node(const Node&) = delete;
    Node &operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node &operator=(Node &&) = delete;
};

template <typename T, bool less(const T& t1, const T& t2)>
struct AvlTree<T, less>::Node::Inner
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
    const RecordIterator &operator++();
    const RecordIterator &nextNode();
    reference operator*();
    pointer operator->();
    bool operator==(const RecordIterator &rhs) const;
    bool operator!=(const RecordIterator &rhs) const;
    RecordIterator();
    RecordIterator(Node *pos, typename Node::Inner *inn = nullptr, bool atEnd = false);
    bool atEnd() const;

private:
    Node *nextNodePtr();
};


#include "avl_tree.hpp"
#include "avl_tree_node.hpp"
#include "avl_tree_iterator.hpp"

#endif
