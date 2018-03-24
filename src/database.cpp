#include "database.h"
#include "record.h"

DatabaseCursor Database::query(const DatabaseQuery &query)
{
    // TODO Impement //
}

bool Database::insert(Record &&record)
{
    // Sorted DList of groups. Each group has container and AVL by name. Global AVL by name

    auto inserted_record = groups.insert(record);
    nameIndex.index(inserted_record);
}

bool Database::remove(const DatabaseCursor &cursor)
{
    auto &record = cursor.get();
    nameIndex.remove(record);
    
}
