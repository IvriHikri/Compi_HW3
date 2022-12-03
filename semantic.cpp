#include "semantic.h"

Semantic::Semantic()
{
    symbolTables = list<Table>();
    offset = list<int>();
    offset.push_back(0);
}

void Semantic::openScope()
{
    symbolTables.emplace_back(Table());
    offset.push_back(offset.back());
}

void Semantic::addSymbol(Node *symbol)
{
    for (Table t : symbolTables)
    {
        for (TableEntry ent : t.getEntries())
        {
            if (ent.getName().compare(symbol->value) == 0)
            {
                errorDef(yylineno, symbol->value);
            }
        }
    }

    this->symbolTables.back().getEntries().emplace_back(TableEntry(symbol->value, vector<Var_Type>(1, symbol->type), this->offset.back()));
    this->offset.back() += 1;
}

void Semantic::addFunc()
{
    // TODO
}

void Semantic::closeScope()
{
    this->symbolTables.pop_back();
    this->offset.pop_back();
}