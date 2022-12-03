#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hw3_output.hpp"
#include "classes.h"
#include <vector>
#include <string>
#include <list>
#include <stack>

using std::list;
using std::stack;
using std::string;
using std::vector;

class TableEntry
{
    string name;
    vector<Var_Type> types;
    string value;
    int offset;
    bool isFunc;

public:
    TableEntry();
    TableEntry(string &name, vector<Var_Type> &types, string &value, int offset, bool isFunc)
    {
        this->name = name;
        this->types = types;
        this->value = value;
        this->offset = offset;
        this->isFunc = isFunc;
    }

    string &getName() { return this->name; }
    vector<Var_Type> &getTypes() { return this->types; }
    int getOffset() { return this->offset; }
    string &getValue() { return this->value; }
    void setValue(string value) { this->value = value; }
};

class Table
{
    vector<TableEntry> symbols;

public:
    Table()
    {
        symbols = vector<TableEntry>();
    }

    vector<TableEntry> &getEntries()
    {
        return this->symbols;
    }
};

class Semantic
{
public:
    list<Table> symbolTables;
    stack<int> offset;
    static bool in_while;

    Semantic();
    ~Semantic() = default;

    void addSymbol(Node *symbol, string &value);
    void addFunc(Func *func);
    bool isExist(string id);
    TableEntry *getTableEntry(string id);
    void openScope();
    void closeScope();
    static bool start_while();
    static bool finish_while();
};

Semantic *sem = new Semantic();

#endif /*SEMANTIC_H*/