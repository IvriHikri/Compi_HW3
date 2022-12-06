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
    Var_Type returnValue;
    string value;
    int offset;
    bool isFunc;

public:
    TableEntry();
    explicit TableEntry(string &name, Var_Type &type, string &value, int offset, bool isFunc) // for single symbol
    {
        this->name = name;
        this->types = vector<Var_Type>(1, type);
        this->returnValue = UNDEFINED;
        this->value = value;
        this->offset = offset;
        this->isFunc = isFunc;
    }

    explicit TableEntry(string &name, vector<Var_Type> &types, Var_Type returnValue, string &value, int offset, bool isFunc) // for function
    {
        this->name = name;
        this->types = types;
        this->returnValue = returnValue;
        this->value = value;
        this->offset = offset;
        this->isFunc = isFunc;
    }

    ~TableEntry() = default;

    string &getName() { return this->name; }
    vector<Var_Type> &getTypes() { return this->types; }
    int getOffset() { return this->offset; }
    string &getValue() { return this->value; }
    bool getIsFunc() { return this->isFunc; }
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

    ~Table() = default;

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
    void declareFunction(Type *type, Node *id, Formals *formals);
    bool isExist(string id);
    TableEntry *getTableEntry(string id);
    void openScope();
    void closeScope();
    bool checkReturnType(Exp *e = nullptr);
    static bool start_while();
    static bool finish_while();
};

Semantic *sem = new Semantic();

#endif /*SEMANTIC_H*/