#ifndef RECORD_H
#define RECORD_H

#include <cstring>
#include <memory>

class Record
{
    int _group = 0;
    int _phone = 0;
    std::unique_ptr<const char[]> _name = nullptr;

public:
    int group() const
    {
        return _group;
    }
    int group(int new_group)
    {
        return _group = new_group;
    }

    int phone() const
    {
        return _phone;
    }
    int phone(int new_phone)
    {
        return _phone = new_phone;
    }

    const char *name() const
    {
        return _name.get();
    }
    const char *name(const char *&&new_name)
    {
        _name = std::unique_ptr<const char[]>(new_name);
        new_name = nullptr;
        return _name.get();
    }

    Record() = delete;
    Record(const Record&) = delete;
    Record(Record&& record) : _group(record.group()), _phone(record.phone()), _name(std::move(record._name)) {}

    const Record &operator=(const Record&) = delete;
    Record &operator=(Record&& record)
    {
        _group = record._group;
        _phone = record._phone;
        _name = std::move(record._name);
        return *this;
    }

    Record(char *name, int group, int phone)
    {
        if(!name)
            _name = nullptr;
        char *str = new char[std::strlen(name)+1];
        std::strcpy(str, name);
        _name = std::unique_ptr<const char[]>(str);
    }

    ~Record() {}
};

#endif
