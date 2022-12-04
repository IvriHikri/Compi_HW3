#include "semantic.h"

Semantic::Semantic()
{
    symbolTables = list<Table>();
    offset = stack<int>();
    offset.push(0);
    in_while = false;
}

void Semantic::openScope()
{
    symbolTables.emplace_back(Table());
    offset.push(offset.top());
}

void Semantic::addSymbol(Node *symbol, string &value)
{
    if (isExist(symbol->value))
    {
        errorDef(yylineno, symbol->value);
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(symbol->value, vector<Var_Type>(1, symbol->type), value, this->offset.top(),false));
    this->offset.top()++;
}

void Semantic::addFunc(Func *func)
{
    if (isExist(func->value))
    {
        errorDef(yylineno, func->value);
    }

    string emptyString = "";
    // this->symbolTables.back().getEntries().emplace_back(TableEntry(func->value,func->vec,emptyString, 0, true));
}

void Semantic::closeScope()
{
    this->symbolTables.pop_back();
    this->offset.pop();
}

bool Semantic::isExist(string id)
{
    for (Table t : symbolTables)
    {
        for (TableEntry ent : t.getEntries())
        {
            if (ent.getName().compare(id) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

TableEntry *Semantic::getTableEntry(string id)
{
    for (Table t : symbolTables)
    {
        for (TableEntry ent : t.getEntries())
        {
            if (ent.getName().compare(id) == 0)
            {
                return &ent;
            }
        }
    }

    return nullptr;
} 

bool Semantic::start_while()
{
    in_while = true;
}

bool Semantic::finish_while()
{
    in_while = false;
}

void Semantic::declareFunction(Type* type, Node* id, Formals* formals)
{
    if(isExist(id->value))
        errorDef(yylineno, id->value);
    vector<Var_Type> var_types;
    for(FormalDecl* f : formals->declaration)
    {
        if(isExist(f->value))
            errorDef(yylineno, f->value);
        var_types.push_back(f->type);
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(id->value, var_types, id->value, 0, true));
    int i = -1;
    for(FormalDecl* f : formals->declaration)
    {
        this->symbolTables.back().getEntries().emplace_back(TableEntry(f->value, vector<Var_Type>(1, f->type), f->value, i, false));
        i--;
    }
}
