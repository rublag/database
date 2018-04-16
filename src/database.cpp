#include "external_index.h"
#include "record.h"
#include "database.h"
#include "groups.h"
#include "group.h"
#include "records.h"
#include "test_like.h"

bool Database::test(const Record &record, const Database::Query &query) const
{
    switch(query.nameOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
        if(std::strcmp(query.name, record.name()) != 0)
            return false;
        break;
    case Query::Operator::Ne:
        if(std::strcmp(query.name, record.name()) == 0)
            return false;
        break;
    case Query::Operator::Lt:
        if(std::strcmp(query.name, record.name()) >= 0)
            return false;
        break;
    case Query::Operator::Le:
        if(std::strcmp(query.name, record.name()) > 0)
            return false;
        break;
    case Query::Operator::Gt:
        if(std::strcmp(query.name, record.name()) <= 0)
            return false;
        break;
    case Query::Operator::Ge:
        if(std::strcmp(query.name, record.name()) < 0)
            return false;
        break;
    case Query::Operator::Like:
        if(!test_like(record.name(), query.name))
            return false;
        break;
    }

    switch(query.groupOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
        if(query.group != record.group())
            return false;
        break;
    case Query::Operator::Ne:
        if(query.group == record.group())
            return false;
        break;
    case Query::Operator::Lt:
        if(query.group >= record.group())
            return false;
        break;
    case Query::Operator::Le:
        if(query.group > record.group())
            return false;
        break;
    case Query::Operator::Gt:
        if(query.group <= record.group())
            return false;
        break;
    case Query::Operator::Ge:
        if(query.group < record.group())
            return false;
        break;
    default:
        break;
    }

    switch(query.phoneOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
        if(query.phone != record.phone())
            return false;
        break;
    case Query::Operator::Ne:
        if(query.phone == record.phone())
            return false;
        break;
    case Query::Operator::Lt:
        if(query.phone >= record.phone())
            return false;
        break;
    case Query::Operator::Le:
        if(query.phone > record.phone())
            return false;
        break;
    case Query::Operator::Gt:
        if(query.phone <= record.phone())
            return false;
        break;
    case Query::Operator::Ge:
        if(query.phone < record.phone())
            return false;
        break;
    default:
        break;
    }
    return true;
}

Group::Query Database::makeGroupQuery(Database::Query &query)
{
    Group::Query gq;
    gq.name = query.name;
    gq.phone = query.phone;

    switch(query.nameOp)
    {
    case Query::Operator::Eq:
        gq.nameOp = Group::Query::Operator::Eq;
        break;
    case Query::Operator::Ne:
        gq.nameOp = Group::Query::Operator::Ne;
        break;
    case Query::Operator::Lt:
        gq.nameOp = Group::Query::Operator::Lt;
        break;
    case Query::Operator::Le:
        gq.nameOp = Group::Query::Operator::Le;
        break;
    case Query::Operator::Gt:
        gq.nameOp = Group::Query::Operator::Gt;
        break;
    case Query::Operator::Ge:
        gq.nameOp = Group::Query::Operator::Ge;
        break;
    case Query::Operator::Nil:
        gq.nameOp = Group::Query::Operator::Nil;
        break;
    case Query::Operator::Like:
        gq.nameOp = Group::Query::Operator::Like;
        break;
    }

    switch(query.phoneOp)
    {
    case Query::Operator::Eq:
        gq.phoneOp = Group::Query::Operator::Eq;
        break;
    case Query::Operator::Ne:
        gq.phoneOp = Group::Query::Operator::Ne;
        break;
    case Query::Operator::Lt:
        gq.phoneOp = Group::Query::Operator::Lt;
        break;
    case Query::Operator::Le:
        gq.phoneOp = Group::Query::Operator::Le;
        break;
    case Query::Operator::Gt:
        gq.phoneOp = Group::Query::Operator::Gt;
        break;
    case Query::Operator::Ge:
        gq.phoneOp = Group::Query::Operator::Ge;
        break;
    case Query::Operator::Nil:
    default:
        gq.phoneOp = Group::Query::Operator::Nil;
        break;
    }
    return gq;
}

void Database::insert(Record &&record)
{
    auto it = groups.find(record.group());
    if(it == groups.end())
    {
        Group group(record.group());
        it = groups.insert(std::move(group));
    }
    auto &grp = it->insert(std::move(record));
    names.insert(&grp);
    assert( names.invariant() );
}

void Database::remove(const Query &query)
{
    auto it = select(query);
    while(!it.atEnd())
    {
        it = erase(it);
    }
}

typename Database::iterator Database::erase(typename Database::iterator it)
{
    if(it.uses == iterator::Uses::Name)
    {
        auto &record = *it;
        auto name_it = names.find(record);

        auto group_num = (*name_it)->group();
        auto &group = *(groups.find(group_num));
        name_it = names.erase(name_it);
        group.erase(record);
        it = iterator(name_it, it.query);
        return it;
    }
    
    auto tmp = it;
    ++it;
    erase(*tmp);
    return it;
}

typename Database::iterator Database::select(Database::Query query)
{
    if(query.nameOp != Query::Operator::Nil)
    {
        switch(query.nameOp)
        {
        case Query::Operator::Eq:
        case Query::Operator::Gt:
        case Query::Operator::Ge:
        case Query::Operator::Like:
        {
            auto lower = names.lower_bound(query.name);
            auto it = iterator(lower, query);
            return it;
        }
        case Query::Operator::Ne:
        case Query::Operator::Lt:
        case Query::Operator::Le:
        default:
        {
            auto lower = names.begin();
            auto it = iterator(lower, query);
            return it;
        }
        }
    }
    auto lower = groups.begin();
    auto it = iterator(lower, query);
    return it;
}

void Database::erase(const Record &record)
{
    auto it = names.find(record);
    if(it.atEnd())
        return;

    auto group_num = (*it)->group();
    auto &group = *(groups.find(group_num));
    names.erase(it);
    group.erase(record);
}

Database::Database(std::istream &istr)
{
    static const size_t buffer_size = 1024;
    char buffer[buffer_size];
    int phone, group;
    while(istr >> buffer >> phone >> group)
    {
        insert(Record(buffer, group, phone));
    }
}
