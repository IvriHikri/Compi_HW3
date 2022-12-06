#include "semantic.h"

Semantic::Semantic()
{
    symbolTables = list<Table>();
    offset = stack<int>();
    in_while = false;
}

void Semantic::openScope()
{
    this->symbolTables.emplace_back(Table());
    if (this->symbolTables.size() == 1)
    {
        string print = "print";
        string printInt = "printi";
        vector<Var_Type> vec_print = vector<Var_Type>(1, V_STRING);
        vector<Var_Type> vec_printInt = vector<Var_Type>(1, V_INT);
        this->symbolTables.back().getEntries().emplace_back(TableEntry(print, vec_print, V_VOID, print, 0, true));
        this->symbolTables.back().getEntries().emplace_back(TableEntry(printInt, vec_printInt, V_VOID, printInt, 0, true));
        this->offset.push(0);
    }
    else
    {
        this->offset.push(offset.top());
    }
}

void Semantic::closeScope()
{
    if (this->symbolTables.size() == 1)
    {
        TableEntry *ent = this->getTableEntry(string("main"));
        if (ent == nullptr || ent->getTypes().size() != 1 || ent->getTypes()[0] != V_VOID)
        {
            errorMainMissing();
        }
    }
    this->symbolTables.pop_back();
    this->offset.pop();
    endScope();
}

void Semantic::addSymbol(Node *symbol, string &value)
{
    if (isExist(symbol->value))
    {
        errorDef(yylineno, symbol->value);
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(symbol->value, symbol->type, value, this->offset.top(), false));
    this->offset.top()++;
}

void Semantic::declareFunction(Type *type, Node *id, Formals *formals)
{
    if (isExist(id->value))
        errorDef(yylineno, id->value);
    vector<Var_Type> var_types;
    for (FormalDecl *f : formals->declaration)
    {
        if (isExist(f->value))
            errorDef(yylineno, f->value);
        var_types.push_back(f->type);
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(id->value, var_types, type->type, id->value, 0, true));
    int i = -1;
    for (FormalDecl *f : formals->declaration)
    {
        this->symbolTables.back().getEntries().emplace_back(TableEntry(f->value, f->type, f->value, i, false));
        i--;
    }
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
