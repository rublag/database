#ifndef ENGINE_H
#define ENGINE_H

#include "command.h"
#include "database.h"

class Engine
{
public:
    void runStatement(const Command &cmd);

    Engine(Database &db);
    
    Engine() = delete;
    Engine(const Engine&) = delete;
    Engine &operator=(const Engine&) = delete;

private:
    Database &database;

    void remove(const Command &cmd);
    void select(const Command &cmd);
    void insert(const Command &cmd);
};

#endif
