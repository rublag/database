#ifndef LIST_H
#define LIST_H
#include "node.h"
#include <cstdio>
class List
{
    Node *root_;
    void init(const List &list)
    {
        root_ = new Node(*list.root_);
        root_->next(nullptr);
        root_->prev(nullptr);

        Node *o = list.root_->next();
        Node *l = root_;
        while(o)
        {
            Node *n = new Node(*o);
            n->prev(l);
            l->next(n);
            n->next(nullptr);
            l = n;
            o = o->next();
        }
    }

public:
    List() : root_(nullptr) {}
    List(const List& list)
    {
        init(list);
    }
    ~List()
    {
        if(!root_)
            return;
        
        while(root_)
        {
            Node *next = root_->next();
            delete root_;
            root_ = next;
        }
    }
    List &operator=(const List &list)
    {
        this->~List();
        init(list);
        return *this;
    }

    Node *root()
    {
        return root_;
    }

    Node *root(Node *n)
    {
        root_ = n;
        return root_;
    }

    Node &root(Node &n)
    {
        root_ = &n;
        return *root_;
    }

    void read(const char *fname)
    {
        this->~List();
        FILE *f = fopen(fname, "r");
        char name[256];
        int res;
        Node *prev = nullptr;
        Node *first = nullptr;
        while(fscanf(f, "%s%d", name, &res) == 2)
        {
            Node *cur = new Node(name, res, nullptr, prev);
            if(prev)
                prev->next(cur);
            else
                first = cur;
            prev = cur;
        }
        fclose(f);
        root_ = first;
    }

    void print()
    {
        Node *node = root_;
        while(node)
        {
            printf("%s %d\n", (*node).getName(), (*node).getResult());
            node = node->next();
        }
    }
};
#endif
