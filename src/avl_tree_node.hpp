template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::Node::Node(T&& data) : data(std::move(data))
{
    left = right = parent = nullptr;
    other = nullptr;
    balance = 0;
}
template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::Node::Node()
{
    left = right = parent = nullptr;
    other = nullptr;
    balance = 0;
}

template <typename T, bool less(const T& t1, const T& t2)>
AvlTree<T, less>::Node::~Node()
{
    while(other)
    {
        auto next = other->next;
        other->next = nullptr;
        delete other;
        other = next;
    }
}

template <typename T, bool less(const T& t1, const T& t2)>
void AvlTree<T, less>::Node::insert(T&& data)
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

template <typename T, bool less(const T& t1, const T& t2)>
bool AvlTree<T, less>::Node::remove(const T& data)
{
    assert(other);
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

template <typename T, bool less(const T& t1, const T& t2)>
T* AvlTree<T, less>::Node::find(const T& data)
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
