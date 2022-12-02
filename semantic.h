#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hw3_output.hpp"
#include <vector>
#include <string>
#include <stack>

using std::stack;
using std::string;
using std::vector;
extern int yylineno;

enum Var_Type {V_INT, V_VOID, V_BOOL, V_BYTE, V_STRING, UNDEFINED};
class Node;
#define YYSTYPE Node*

//if($1.type != V_BOOL || $3.type != V_BOOL) errorMismatch(yylineno);

class Node
{
   public:
   string value;
   Node();
   Node (string token_name) : value(token_name){}
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
    Var_Type type;
    bool bool_value;

    public:
    Exp(Exp* exp);
    Exp(Exp* e1, Exp* e2, Exp* e3);
    Exp(Exp* e1, Node* n1, Exp* e2);
    Exp(Var_Type type, Exp* e1, Node* n1, Exp* e2);
    Exp(Node* n1, Exp* e1);
    Exp(Type* t1, Exp* e1);

};

class Id : public Node
{
    string name;

    public:
    Id(string txt): Node("ID"), name(txt){}
};

class Type : public Node
{
    public:
    Var_Type type;
};

class Num : public Node
{
    int value;
};

#endif /*SEMANTIC_H*/