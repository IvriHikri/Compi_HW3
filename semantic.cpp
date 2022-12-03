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

void Semantic::addSymbol(Node *symbol)
{
    if(isExist(symbol->value))
    {
        errorDef(yylineno, symbol->value);
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(symbol->value, vector<Var_Type>(1, symbol->type), this->offset.top()));
    this->offset.top() ++;
}

void Semantic::addFunc(Node *symbol)
{
    if(isExist(symbol->value))
    {

    }
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
}

bool Semantic::start_while()
{
    in_while = true;
}

bool Semantic::finish_while()
{
    in_while = false;
}
