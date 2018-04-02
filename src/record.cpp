#include "record.h"

int Record::group() const
{
    return _group;
}

int Record::group(int new_group)
{
    return _group = new_group;
}

int Record::phone() const
{
    return _phone;
}

int Record::phone(int new_phone)
{
    return _phone = new_phone;
}

char *Record::name() const
{
    return _name;
}

char *Record::name(char *new_name)
{
    _name = new_name;
    return _name;
}

Record::Record(Record&& record) : _group(record.group()), _phone(record.phone())
{
    _name = record._name;
    record._name = nullptr;
}

Record &Record::operator=(Record&& record)
{
    _group = record._group;
    _phone = record._phone;
    _name = record._name;
    record._name = nullptr;
    return *this;
}

Record::Record() {}

Record::Record(char *name, int group, int phone)
{
    if(!name)
        _name = nullptr;
    char *str = new char[std::strlen(name)+1];
    std::strcpy(str, name);
    _name = str;
    _group = group;
    _phone = phone;
}

Record::~Record() {}

bool Record::operator==(const Record &rhs) const
{
    if(strcmp(name(), rhs.name()) == 0 && group() == rhs.group() && phone() == rhs.phone())
        return true;
    return false;
}
