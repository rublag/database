#ifndef RECORD_H
#define RECORD_H

#include <cstring>
#include <memory>

class Record
{
    int _group = 0;
    int _phone = 0;
    char *_name = nullptr;

public:
    int group() const;
    int group(int new_group);
    int phone() const;
    int phone(int new_phone);
    char *name() const;
    char *name(char *new_name);
    Record();
    Record(const Record&) = delete;
    Record(Record&& record);

    bool operator==(const Record &rhs) const;

    const Record &operator=(const Record&) = delete;
    Record &operator=(Record&& record);
    Record(char *name, int group, int phone);
    ~Record(); 
};

#endif
