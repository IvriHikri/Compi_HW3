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
    }
    this->type = v_type;
}

/****************************************   STATEMENT   ****************************************/

// Type ID;
Statement::Statement(Type *t, Node *symbol)
{
    if (sem->isExist(symbol->value))
    {
        errorDef(yylineno, symbol->value);
    }
    symbol->type = t->type;
    string emptyVal = "";
    sem->addSymbol(symbol, emptyVal);
}

// Type ID = EXP;
Statement::Statement(Type *t, Node *symbol, Exp *exp)
{
    if (sem->isExist(exp->value))
    {
        errorDef(yylineno, exp->value);
    }
    if (t->type != exp->type && !(t->type == V_INT && exp->type == V_BYTE))
    {
        errorMismatch(yylineno);
    }

    symbol->type = t->type;
    sem->addSymbol(symbol, exp->value);
}

// ID = Exp; OR return Exp;
Statement::Statement(Node *symbol, Exp *exp)
{
    if (symbol->value.compare("return") == 0)
    {
        if (!sem->checkReturnType(exp->type))
        {
            errorMismatch(yylineno);
        }
    }
    else
    {
        TableEntry *ent = sem->getTableEntry(symbol->value);
        if (ent == nullptr || ent->getIsFunc())
        {
            errorUndef(yylineno, symbol->value);
        }

        if (ent->getReturnValue() != exp->type && !(ent->getReturnValue() == V_INT && exp->type == V_BYTE))
        {
            errorMismatch(yylineno);
        }

        ent->setValue(exp->value);
    }
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
        sem->start_while();
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
    if (symbol->value.compare("return") == 0)
    {
        if (!sem->checkReturnType(V_VOID))
        {
            errorMismatch(yylineno);
        }
    }
    else if (symbol->value.compare("break") == 0)
    {
        if (!sem->in_while)
        {
            errorUnexpectedBreak(yylineno);
        }
    }
    else if (symbol->value.compare("continue") == 0)
    {
        if (!sem->in_while)
        {
            errorUnexpectedContinue(yylineno);
        }
    }

    this->type = UNDEFINED;
    this->value = symbol->value;
}

/****************************************   CALL   ****************************************/

Call::Call(Node *symbol)
{
    TableEntry *ent = sem->getTableEntry(symbol->value);

    if (ent == nullptr || !ent->getIsFunc())
    {
        errorUndefFunc(yylineno, symbol->value);
    }

    if (!ent->getTypes().empty())
    {
        vector temp = ent->convertToStringVector(ent->getTypes());
        errorPrototypeMismatch(yylineno, symbol->value, temp);
    }

    sem->setCurrentFunction(symbol->value);

    this->value = symbol->value;
    this->type = ent->getReturnValue();
}

Call::Call(Node *symbol, Explist *exp_list)
{
    TableEntry *ent = sem->getTableEntry(symbol->value);
    if (ent == nullptr || !ent->getIsFunc())
    {
        errorUndefFunc(yylineno, symbol->value);
    }

    if (ent->getTypes().size() != exp_list->getExpressions().size())
    {
        vector temp = ent->convertToStringVector(ent->getTypes());
        errorPrototypeMismatch(yylineno, symbol->value, temp);
    }

    int index = 0;
    vector temp = exp_list->getExpressions();
    for (Var_Type t : ent->getTypes())
    {
        if (t != temp[index]->type && !(t == V_INT && temp[index]->type == V_BYTE))
        {
            vector temp = ent->convertToStringVector(ent->getTypes());
            errorPrototypeMismatch(yylineno, symbol->value, temp);
        }
        index++;
    }

    sem->setCurrentFunction(symbol->value); // I dont think we need it, why would I care what is the current function? I just want the semanticl analysis to be good...

    this->value = symbol->value;
    this->type = ent->getReturnValue();
}

/****************************************   EXP_LIST   ****************************************/
Explist::Explist(Exp *exp)
{
    this->exp_list.insert(this->exp_list.begin(), exp);
}

Explist::Explist(Exp *exp, Explist *exp_list)
{
    this->exp_list = vector<Exp *>(exp_list->exp_list);
    this->exp_list.insert(this->exp_list.begin(), exp);
}

/****************************************   EXP   ****************************************/

// (Exp)
Exp::Exp(Exp *exp)
{
    this->value = exp->value;
    this->type = exp->type;
}

// Exp IF EXP else EXP
Exp::Exp(Exp *e1, Exp *e2, Exp *e3)
{
    if (e2->type != V_BOOL || (e1->type != e3->type && !((e1->type == V_BYTE && e3->type == V_INT) || (e1->type == V_INT && e3->type == V_BYTE))))
    {
        // TODO- add here a check if e1 and e3 are in the same type or we can cast them..
        errorMismatch(yylineno);
    }
    Exp *temp = (e2->value.compare("true") == 0) ? e1 : e3;
    this->value = temp->value;
    this->type = temp->type;
}

// EXP BINOP EXP
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
    std::cout << "first number " << e1->value << " second number " << e2->value << std::endl;
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

// EXP AND/OR/RELOP EXP
Exp::Exp(Var_Type type, Exp *e1, Node *n1, Exp *e2)
{
    this->type = V_BOOL;
    if (e1->type == V_BOOL || e2->type == V_BOOL)
    {
        if (n1->value.compare("and") == 0)
        {
            this->bool_value = e1->bool_value && e2->bool_value;
            this->value = e1->value + "&&" + e2->value;
        }
        else if (n1->value.compare("or") == 0)
        {
            this->bool_value = e1->bool_value || e2->bool_value;
            this->value = e1->value + "||" + e2->value;
        }
        else
        {
            errorMismatch(yylineno);
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

        this->value = e1->value + n1->value + e2->value;
    }
    else
    {
        errorMismatch(yylineno);
    }
}

// NOT EXP
Exp::Exp(Node *n, Exp *e)
{
    if (e->type != V_BOOL || n->value.compare("not") != 0)
        errorMismatch(yylineno);

    this->type = V_BOOL;
    this->bool_value = !e->bool_value;
}

// (TYPE) EXP
Exp::Exp(Type *t, Exp *e)
{
    if (t->type != e->type)
    {
        if (t->type == V_BYTE && e->type == V_INT)
        {
            if (stoi(e->value) > 255)
                errorByteTooLarge(yylineno, e->value); // error because byte is bigger than 255
        }

        else if (!(t->type == V_INT && e->type == V_BYTE))
            errorMismatch(yylineno);
    }
    this->type = t->type;
    this->value = e->value;
}

// Call
Exp::Exp(Call *c)
{
    this->type = c->type;
    this->value = c->value;
}

// TRUE/FALSE/NUM/STRING
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
    else if (n->type == V_INT)
    {
        this->type = V_INT;
        this->value = n->value;
    }
    else if (n->type == V_STRING)
    {
        this->value = n->value;
        this->type = V_STRING;
    }
}

// NUM B
Exp::Exp(Node *n1, Node *n2)
{
    if (n1->type != V_INT || n2->value.compare("b") != 0)
        errorMismatch(yylineno);

    this->type = V_BYTE;
    if (stoi(n1->value) > 255)
        errorByteTooLarge(yylineno, n1->value);

    this->value = n1->value;
}

/****************************************   FORMAL_DECLERATION   ****************************************/
FormalDecl::FormalDecl(Type *type, Node *node)
{
    this->type = type->type;
    this->value = node->value;
}

/****************************************   FORMALS_LIST   ****************************************/

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

/****************************************   FORMALS  ****************************************/

Formals::Formals(FormalsList *f_list)
{
    this->value = "this is Formals";
    this->declaration = f_list->declaration;
    this->type = UNDEFINED;
}