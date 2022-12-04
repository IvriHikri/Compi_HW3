#include "classes.h"
#include "semantic.h"

/****************************************   TYPE   ****************************************/

Type::Type(Type *t)
{
    this->type = t->type;
    this->value = value;
}

Type::Type(Var_Type v_type)
{
    switch (v_type)
    {
    case (V_INT):
    {
        this->value = "int";
        break;
    }
    case (V_BYTE):
    {
        this->value = "byte";
        break;
    }

    case (V_BOOL):
    {
        this->value = "bool";
        break;
    }

    case (V_VOID):
    {
        this->value = "void";
        break;
    }
    default:
    {
        // error.
    }
    }
    this->type = v_type;
}

/****************************************   STATEMENT   ****************************************/
Statement::Statement(Type *t, Node *symbol)
{
    symbol->type = t->type;
    string emptyVal = "";
    sem->addSymbol(symbol, emptyVal);
}

// Type ID = EXP;
Statement::Statement(Type *t, Node *symbol, Exp *exp)
{
    if (t->type != exp->type && !(t->type == V_INT && exp->type == V_BYTE))
    {
        errorMismatch(yylineno);
    }

    symbol->type = t->type;
    sem->addSymbol(symbol, exp->value);
}

// ID = Exp;
Statement::Statement(Node *symbol, Exp *exp)
{
    TableEntry *ent = sem->getTableEntry(symbol->value);
    if (ent == nullptr)
    {
        errorUndef(yylineno, symbol->value);
    }

    if (ent->getTypes()[0] != exp->type && !(ent->getTypes()[0] == V_INT && exp->type == V_BYTE))
    {
        errorMismatch(yylineno);
    }

    ent->setValue(exp->value);
}

// Call
Statement::Statement(Call *call)
{
    this->value = call->value;
    this->type = call->type;
}

// IF/WHILE (EXP) Statement
Statement::Statement(Node *symbol, Exp *exp, Statement *s)
{
    if (exp->type != V_BOOL)
    {
        errorMismatch(yylineno);
    }
    if (symbol->value.compare("while") == 0)
    {
        sem->finish_while();
    }
    if (exp->bool_value)
    {
        this->type = s->type;
        this->value = s->value;
    }
    else
    {
        this->type = Var_Type::UNDEFINED;
        this->value = "";
    }
}

// IF (EXP) then s1 else s2
Statement::Statement(Exp *exp, Statement *s1, Statement *s2)
{
    if (exp->type != V_BOOL)
    {
        errorMismatch(yylineno);
    }

    Statement *temp = exp->bool_value ? s1 : s2;
    this->type = temp->type;
    this->value = temp->value;
}

// BREAK / CONTINUE
Statement::Statement(Node *symbol)
{
    if (symbol->value.compare("break") == 0)
    {
        if (!sem->in_while)
        {
            errorUnexpectedBreak(yylineno);
        }
    }

    if (symbol->value.compare("continue") == 0)
    {
        if (!sem->in_while)
        {
            errorUnexpectedContinue(yylineno);
        }
    }

    this->type = UNDEFINED;
    this->value = symbol->value;
}

/****************************************   EXP   ****************************************/
Exp::Exp(Exp *exp)
{
    this->value = exp->value;
    this->type = exp->type;
}

Exp::Exp(Exp *e1, Exp *e2, Exp *e3)
{
    if (e2->type != V_BOOL)
    {
        // TODO- add here a check if e1 and e3 are in the same type or we can cast them..
        errorMismatch(yylineno);
    }
    Exp *temp = (e2->value.compare("True") == 0) ? e1 : e3;
    this->value = temp->value;
    this->type = temp->type;
}

// Binop Expression
Exp::Exp(Exp *e1, Node *n, Exp *e2)
{
    if ((e1->type != V_INT && e1->type != V_BYTE) || (e2->type != V_INT && e2->type != V_BYTE))
    {
        errorMismatch(yylineno);
    }

    if (e1->type == V_INT || e2->type == V_INT)
        this->type = V_INT;
    else
        this->type = V_BYTE;

    int num1 = stoi(e1->value);
    int num2 = stoi(e2->value);
    int num3;
    string op = n->value;
    if (op.compare("+") == 0)
        num3 = num1 + num2;

    else if (op.compare("-") == 0)
        num3 = num1 - num2;

    else if (op.compare("*") == 0)
        num3 = num1 * num2;

    else if (op.compare("/") == 0)
        num3 = num1 / num2;

    else
        ;
    // shouldnt get here, but this means that the n1 is not binop..

    if (this->type == V_BYTE && num3 > 255)
        errorByteTooLarge(yylineno, to_string(num3));

    this->value = to_string(num3);
}

// Boolean Expression
Exp::Exp(Var_Type type, Exp *e1, Node *n1, Exp *e2)
{
    this->value = "this is boolean expression";
    this->type = V_BOOL;
    if (e1->type == V_BOOL || e2->type == V_BOOL)
    {
        if (n1->value.compare("and") == 0)
            this->bool_value = e1->bool_value && e2->bool_value;

        else if (n1->value.compare("or") == 0)
            this->bool_value = e1->bool_value || e2->bool_value;

        else
        {
            // error, TODO
        }
    }

    else if ((e1->type == V_INT || e1->type == V_BYTE) && (e2->type == V_INT || e2->type == V_BYTE))
    {
        if (n1->value.compare("<") == 0)
            this->bool_value = (stoi(e1->value) < stoi(e2->value));
        else if (n1->value.compare(">") == 0)
            this->bool_value = (stoi(e1->value) > stoi(e2->value));
        else if (n1->value.compare("<=") == 0)
            this->bool_value = (stoi(e1->value) <= stoi(e2->value));
        else if (n1->value.compare(">=") == 0)
            this->bool_value = (stoi(e1->value) >= stoi(e2->value));
        else if (n1->value.compare("!=") == 0)
            this->bool_value = (stoi(e1->value) != stoi(e2->value));
        else if (n1->value.compare("==") == 0)
            this->bool_value = (stoi(e1->value) == stoi(e2->value));
        else
            return; // error, need to refill.
    }
    else
    {
        errorMismatch(yylineno);
    }
}

Exp::Exp(Node *n, Exp *e)
{
    if (e->type != V_BOOL || n->value.compare("not") != 0)
        return; // TODO, fill error

    this->type = V_BOOL;
    this->bool_value = !e->bool_value;
}

Exp::Exp(Type *t, Exp *e)
{
    if (t->type != e->type)
    {
        if (t->type == V_BYTE && e->type == V_INT)
        {
            if (stoi(e->value) > 255)
                return; // error because byte is bigger than 255
        }

        else if (!(t->type == V_INT && e->type == V_BYTE))
            return; // errorrrrrrrrrrrrrrrrr
    }
    this->type = t->type;
    this->value = e->value;
}

Exp::Exp(Call *c)
{
    // TODO- fill this after we figure out what call is...
}

Exp::Exp(Node *n)
{
    this->value = n->value;
    if (n->value.compare("true") == 0)
    {
        this->bool_value = true;
        this->type = V_BOOL;
    }
    else if (n->value.compare("false") == 0)
    {
        this->bool_value = false;
        this->type = V_BOOL;
    }
    else if (n->value.compare("num") == 0) // לא יקרה
        this->type = V_INT;
    else if (n->value.compare("string") == 0)
        this->type = V_STRING;
}

Exp::Exp(Node *n1, Node *n2)
{
    if (n1->value.compare("num") != 0 || n2->value.compare("b") != 0) // לא יקרה
        return;                                                       // errorrrrr shouldnt get here..
    this->type = V_BYTE;
    this->value = n1->value;
}

/****************************************   FORMAL_DECLERATION   ****************************************/
FormalDecl::FormalDecl(Type *type, Node *node)
{
    this->type = type->type;
    this->value = node->value;
}

FormalsList::FormalsList(FormalDecl *f_dec)
{
    this->type = UNDEFINED;
    this->value = "This is a formals list";
    this->declaration.emplace_back(f_dec);
}

FormalsList::FormalsList(FormalDecl *f_dec, FormalsList *f_list)
{
    this->type = f_list->type;
    this->value = f_list->value;
    this->declaration = f_list->declaration;
    this->declaration.insert(declaration.begin(), f_dec);
}

Formals::Formals(FormalsList *f_list)
{
    this->value = "this is Formals";
    this->declaration = f_list->declaration;
    this->type = UNDEFINED;
}