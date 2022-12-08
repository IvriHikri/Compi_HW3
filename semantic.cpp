#include "semantic.h"

Semantic::Semantic()
{
    symbolTables = list<Table>();
    offset = stack<int>();
    in_while = false;
    currentFunction = "";
}

void Semantic::openScope()
{
    if (this->symbolTables.empty())
    {
        this->symbolTables.emplace_back(Table());
        string print = "print";
        string printInt = "printi";
        vector<Var_Type> vec_print = vector<Var_Type>(1, V_STRING);
        vector<Var_Type> vec_printInt = vector<Var_Type>(1, V_INT);
        this->symbolTables.back().getEntries().emplace_back(TableEntry(print, vec_print, V_VOID, print, 0));
        this->symbolTables.back().getEntries().emplace_back(TableEntry(printInt, vec_printInt, V_VOID, printInt, 0));
        this->offset.push(0);
    }
    else
    {
        this->symbolTables.emplace_back(Table());
        this->offset.push(offset.top());
    }
}

void Semantic::findMain()
{
    TableEntry *ent = this->getTableEntry(string("main"));
    if (ent == nullptr || ent->getTypes().size() != 0 || ent->getReturnValue() != V_VOID)
    {
        errorMainMissing();
    }
}

void Semantic::closeScope()
{
    endScope();
    Table t = this->symbolTables.back();
    for (TableEntry ent : t.getEntries())
    {
        if (ent.getIsFunc())
        {
            printID(ent.getName(), ent.getOffset(), makeFunctionType(convertToString(ent.getReturnValue()), convertToStringVector(ent.getTypes())));
        }
        else
        {
            printID(ent.getName(), ent.getOffset(), convertToString(ent.getTypes()[0]));
        }
    }
    this->symbolTables.pop_back();
    this->offset.pop();
    this->currentFunction = "";
}

void Semantic::addSymbol(Node *symbol, string &value)
{
    if (isExist(symbol->value))
    {
        errorDef(yylineno, symbol->value);
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(symbol->value, symbol->type, value, this->offset.top()));
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
    this->symbolTables.back().getEntries().emplace_back(TableEntry(id->value, var_types, type->type, id->value, 0));

    openScope();
    this->symbolTables.emplace_back(Table());
    int i = -1;
    for (FormalDecl *f : formals->declaration)
    {
        this->symbolTables.back().getEntries().emplace_back(TableEntry(f->value, f->type, f->value, i));
        i--;
    }

    sem->currentFunction = id->value;
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


bool Semantic::checkReturnType(Var_Type type)
{
    TableEntry *ent = getTableEntry(currentFunction);
    if (ent == nullptr || !ent->getIsFunc())
    {
        //will be if we closed the scope of a function and then "currentFunction" = ""...
        // shouldn't happen
        exit(1);
    }

    return ((ent->getReturnValue() == type) || (ent->getReturnValue() == V_INT && type == V_BYTE));
}


bool Semantic::start_while()
{
    in_while = true;
}

bool Semantic::finish_while()
{
    in_while = false;
}
