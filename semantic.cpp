#include "semantic.h"
#include "hw3_output.hpp"

using namespace output;
Exp::Exp(Exp* exp): Node(exp->value)
{
    this->type = exp->type;
    this->name = exp->name;
}

Exp::Exp(Exp* e1, Exp* e2, Exp* e3)
{
    if(e2->type != V_BOOL)
    {
        errorMismatch(yylineno);
    }
    Exp* temp = (e2->value.compare("True") == 0)? e1 : e3;
    this->value = temp->value;
    this->type = temp->type;
    this->name = temp->name;
}