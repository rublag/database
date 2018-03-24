#ifndef INDEXER_H
#define INDEXER_H

struct IndexerQuery
{
public:
    enum class Compare {
        Nil, Eq, Lt, Le, Gt, Ge, Ne
    };
};

class IndexerCursor
{
public:
    bool next();
    Record &get();
    bool end();
}

class Indexer
{
    virtual bool index(const Record &record) =0;
    virtual bool remove(const Record &record) =0;
    virtual Index &search() =0;
}

#endif
