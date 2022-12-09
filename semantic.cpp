#include "semantic.h"


void openScope()
{
    if (symbolTables.empty())
    {
        symbolTables.emplace_back(new Table());
        string print = "print";
        string printInt = "printi";
        vector<Var_Type> vec_print = vector<Var_Type>(1, V_STRING);
        vector<Var_Type> vec_printInt = vector<Var_Type>(1, V_INT);
        symbolTables.back()->getEntries().emplace_back(new TableEntry(print, vec_print, V_VOID, 0));
        symbolTables.back()->getEntries().emplace_back(new TableEntry(printInt, vec_printInt, V_VOID, 0));
        offset.push(0);
    }
    else
    {
        symbolTables.emplace_back(new Table());
        offset.push(offset.top());
    }
}

void findMain()
{
    TableEntry *ent = getTableEntry(string("main"));
    if (ent == nullptr || ent->getTypes().size() != 0 || ent->getReturnValue() != V_VOID)
    {
        errorMainMissing();
    }
}

void closeScope()
{
    endScope();
    Table *t = symbolTables.back();
    string s;
    for (TableEntry *ent : t->getEntries())
    {
        if (ent->getIsFunc())
        {
            s = ent->convertToString(ent->getReturnValue());
            vector<string> temp = ent->convertToStringVector(ent->getTypes());
            printID(ent->getName(), ent->getOffset(), makeFunctionType(s, temp));
        }
        else
        {
            s = ent->convertToString(ent->getTypes()[0]);
            printID(ent->getName(), ent->getOffset(), s);
        }
    }
    symbolTables.pop_back();
    offset.pop();
    currentFunction = "";
}

void addSymbol(Node *symbol, string &value)
{
    if (isExist(symbol->value))
    {
        errorDef(yylineno, symbol->value);
    }

    symbolTables.back()->getEntries().emplace_back(new TableEntry(symbol->value, symbol->type, offset.top()));
    offset.top()++;
}

void declareFunction(Type *type, Id *id, Formals *formals)
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
    symbolTables.back()->getEntries().emplace_back(new TableEntry(id->value, var_types, type->type, 0));

    openScope();
    int i = -1;
    for (FormalDecl *f : formals->declaration)
    {
        symbolTables.back()->getEntries().emplace_back(new TableEntry(f->value, f->type, i));
        i--;
    }
    currentFunction = id->value;
    cout << "tables: " << symbolTables.size() << endl;
    for (Table *t : symbolTables)
    {
        for (TableEntry *ent : t->getEntries())
        {
            cout << ent->getName() << endl;
        }
    }
}

bool isExist(string id)
{
    for (Table *t : symbolTables)
    {
        for (TableEntry *ent : t->getEntries())
        {
            if (ent->getName().compare(id) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

TableEntry* getTableEntry(string id)
{
    for (Table *t : symbolTables)
    {
        for (TableEntry *ent : t->getEntries())
        {
            cout << ent->getName() << endl;
            cout << ent->getName().compare(id) << endl;
            if (ent->getName().compare(id) == 0)
            {
                return ent;
            }
        }
    }
    return nullptr;
}

bool checkReturnType(Var_Type type)
{
    TableEntry *ent = getTableEntry(currentFunction);
    if (ent == nullptr || !ent->getIsFunc())
    {
        // will be if we closed the scope of a function and then "currentFunction" = ""...
        //  shouldn't happen
        exit(1);
    }

    return ((ent->getReturnValue() == type) || (ent->getReturnValue() == V_INT && type == V_BYTE));
}

bool start_while()
{
    in_while = true;
}

bool finish_while()
{
    in_while = false;
}
