#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hw3_output.hpp"
#include "classes.h"
#include <vector>
#include <string>
#include <list>

using std::list;
using std::string;
using std::vector;

class TableEntry
{
    string name;
    vector<Var_Type> types;
    int offset;

public:
    TableEntry();
    TableEntry(string &name, vector<Var_Type> &types, int offset)
    {
        this->name = name;
        this->types = types;
        this->offset = offset;
    }

    string &getName() { return this->name; }
    vector<Var_Type> &getTypes() { return this->types; }
    int getOffset() { return this->offset; }
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
    list<Table> symbolTables;
    list<int> offset;

public:
    Semantic();
    ~Semantic() = default;

    void openScope();
    void addSymbol(Node *symbol);
    void addFunc();
    void closeScope();
};

#endif /*SEMANTIC_H*/