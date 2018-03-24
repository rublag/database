#ifndef NODE_H
#define NODE_H

#include "student.h"

class Node : public Student
{
private:
    Node *next_;
    Node *prev_;

public:
    Node(const char *n = 0, int r = 0, Node *next = nullptr, Node *prev = nullptr) : Student(n, r), next_(next), prev_(prev) {}
    Node(const Node &node) : Student(node), next_(node.next_), prev_(node.prev_) {}
    ~Node()
    {
    }

    Node &operator=(const Node &node)
    {
        Student::operator=(node);
        next_ = node.next_;
        prev_ = node.prev_;
        return *this;
    }

    using Student::operator<;
    bool operator<(const Node &node) const
    {
        return Student::operator<(node);
    }

    Node *next()
    {
        return next_;
    }

    Node *prev()
    {
        return prev_;
    }

    Node *next(Node *node)
    {
        next_ = node;
        return this;
    }

    Node *prev(Node *node)
    {
        prev_ = node;
        return this;
    }

    Node &next(Node &node)
    {
        next_ = &node;
        return *this;
    }

    Node &prev(Node &node)
    {
        prev_ = &node;
        return *this;
    }
};

#endif
