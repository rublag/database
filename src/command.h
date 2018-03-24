#ifndef COMMAND_H
#define COMMAND_H

struct Condition
{
    enum Type {
        Nil = 0, Le, Lt, Ge, Gt, Eq, Ne, Like
    } type;
    enum class Left {
        Nil = 0, Name, Phone, Value
    } left;
    char *str;
    int value;
};

struct Command
{
    enum Type {
        Insert, Select, Delete, Quit, ErrParser
    } type;

    char *name;
    int phone;
    int value;
    
    bool ast;
    
    enum Field {
        Nil = 0, Name, Phone, Value
    } query[3];

    Condition cond[3] = {};
    bool andSet[2];
};

#endif
