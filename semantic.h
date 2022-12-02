#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hw3_output.hpp"
#include <vector>
#include <string>
#include <stack>

using std::stack;
using std::string;
using std::vector;

enum Var_Type {V_INT, V_VOID, V_BOOL, V_BYTE, V_STRING, UNDEFINED};
class Node;
#define YYSTYPE Node*

class Node
{
   
};

class TableEntry
{
    string name;
    vector<string> types;
    int offset;
};

class Table
{
    vector<TableEntry> symbols;
};

class Semantic
{
    stack<Table> symbolTables;
    stack<int> offset;

    public:
    Semantic()
    {
        symbolTables = stack<Table>();
        offset = stack<int>();
    }

    ~Semantic() = default;
};

/***********************************************************************************************/

class Program : public Node
{
};

class Exp : public Node
{
    string name;
    Var_Type type;

};

class Id : public Node
{
    string name;

    public:
    Id(string txt): name(txt){}
};

class Type : public Node
{
    private:
    string type;
    
    public:
    Type(string type): type(type){}
};

class Num : public Node
{
    int value;
};

#endif /*SEMANTIC_H*/