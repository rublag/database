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
    //T *search(const T& data);

    iterator begin() const;
    iterator end() const;

    iterator lower_bound(const T& data) const;
    iterator upper_bound(const T& data) const;

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
    bool remove(const T& data);
    /*T* find(const T& data);*/
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
#include "avl_tree.hpp"
#include "avl_tree_node.hpp"
#include "avl_tree_iterator.hpp"
#endif
