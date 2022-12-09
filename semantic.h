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
    explicit TableEntry(string &name, Var_Type type, string &value, int offset) // for single symbol
    {
        this->name = name;
        this->types = vector<Var_Type>(1, type);
        this->returnValue = UNDEFINED;
        this->value = value;
        this->offset = offset;
        this->isFunc = false;
    }

    explicit TableEntry(string &name, vector<Var_Type> &types, Var_Type returnValue, string &value, int offset) // for function
    {
        this->name = name;
        this->types = types;
        this->returnValue = returnValue;
        this->value = value;
        this->offset = offset;
        this->isFunc = true;
    }

    ~TableEntry() = default;

    string &getName() { return this->name; }
    vector<Var_Type> getTypes() { return this->types; }
    Var_Type getReturnValue() { return this->returnValue; }
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
    bool in_while;
    string currentFunction;

    Semantic();
    ~Semantic() = default;

    void addSymbol(Node *symbol, string &value);
    void declareFunction(Type *type, Node *id, Formals *formals);
    bool isExist(string id);
    TableEntry *getTableEntry(string id);
    void openScope();
    void closeScope();
    void findMain();
    bool checkReturnType(Var_Type type);
    bool start_while();
    bool finish_while();

    string &getCurrentFunction() { return this->currentFunction; }
    void setCurrentFunction(string &func) { this->currentFunction = func; }
};

Semantic *sem = new Semantic();

string &convertToString(Var_Type t)
{
    string s;
    switch (t)
    {
    case V_INT:
        s = "int";
        break;
    case V_BYTE:
        s = "byte";
        break;
    case V_BOOL:
        s = "bool";
        break;
    case V_STRING:
        s = "string";
        break;
    }
    return s;
}

vector<string> convertToStringVector(vector<Var_Type> vec)
{
    vector<string> new_vec = vector<string>();
    for (Var_Type t : vec)
    {
        new_vec.push_back(convertToString(t));
    }

    return new_vec;
}

#endif /*SEMANTIC_H*/