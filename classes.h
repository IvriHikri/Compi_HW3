#ifndef CLASSES_H
#define CLASSES_H

#include "hw3_output.hpp"
#include <vector>
extern int yylineno;
using namespace output;
using namespace std;

enum Var_Type
{
    V_INT,
    V_VOID,
    V_BOOL,
    V_BYTE,
    V_STRING,
    UNDEFINED
};

class Node
{
    public:
    string value;
    Var_Type type;

    Node() {};
    Node(string token_value) : value(token_value), type(UNDEFINED) {}
    Node(string token_value, Var_Type type) : value(token_value), type(type) {}
};

#define YYSTYPE Node *

/*************************************         CLASSES        **********************************************************/
class Exp;
class Type;
class Call;
class FormalDecl;
class Formals;
class FormalsList;

class Program : public Node
{
};

class Statement : public Node
{
    public:
    // Type ID;
    explicit Statement(Type *t, Node *symbol);

    // Type ID = Exp;
    explicit Statement(Type *t, Node *symbol, Exp *exp);

    // ID = Exp; or Return Exp;
    explicit Statement(Node *symbol, Exp *exp);

    // Call;
    explicit Statement(Call *call);

    // IF/WHILE (EXP) then statement;
    explicit Statement(Node *symbol, Exp *exp, Statement *s);

    // IF (EXP) then statement else statement;
    explicit Statement(Exp *exp, Statement *s1, Statement *s2);

    // BREAK/ CONTINUE/ RETURN
    explicit Statement(Node *symbol);
};

class Exp;
class Explist;

class Call : public Node
{
public:
    Call(Node *n);
    Call(Node *n, Explist *exp_list);
};

class Explist : public Node
{
    vector<Exp *> exp_list;

public:
    Explist(Exp *exp);
    Explist(Exp *exp, Explist *exp_list);

    vector<Exp *> getExpressions() { return this->exp_list; }
};

class Exp : public Node
{
public:
    bool bool_value;

    // (Exp)
    Exp(Exp *exp);

    // Exp IF EXP else EXP
    Exp(Exp *e1, Exp *e2, Exp *e3);

    // EXP BINOP EXP
    Exp(Exp *e1, Node *n, Exp *e2);

    // EXP AND/OR/RELOP EXP
    Exp(Var_Type type, Exp *e1, Node *n, Exp *e2);

    // NOT EXP
    Exp(Node *n, Exp *e);

    // (TYPE) EXP
    Exp(Type *t, Exp *e);

    // Call
    Exp(Call *c);

    // TRUE/FALSE/NUM/STRING
    Exp(Node *n);

    // NUM B
    Exp(Node *n1, Node *n2);
};

class Type : public Node
{
public:
    explicit Type(Type *t);
    explicit Type(Var_Type v_type);
};

class FormalDecl : public Node
{
public:
    FormalDecl(Type *type, Node *node);
};

class Formals : public Node
{
public:
    vector<FormalDecl *> declaration;
    Formals() = default;
    Formals(FormalsList *f_list);
};

class FormalsList : public Formals
{
public:
    FormalsList(FormalDecl *f_dec);
    FormalsList(FormalDecl *f_dec, FormalsList *f_list);
};

#endif /*CLASSES_H*/