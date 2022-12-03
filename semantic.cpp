#include "semantic.h"
#include "hw3_output.hpp"

using namespace output;

Type::Type(Type* t1)
{
    this->type = t1->type;
    this->value = value;
}

Type::Type(Var_Type v_type)
{
    switch(v_type){
     case(V_INT):
     {
        this->value = "int";
        break;
     }
     case(V_BYTE):
     {
        this->value = "byte";
        break;
     }

     case(V_BOOL):
     {
        this->value = "bool";
        break;
     }

     case(V_VOID):
    {
        this->value = "void";
        break;
    }
     default:
     {
        //error.
     }
    }
    this->type = v_type;
}

Exp::Exp(Exp* exp): Node(exp->value)
{
    this->type = exp->type;
}

Exp::Exp(Exp* e1, Exp* e2, Exp* e3)
{
    if(e2->type != V_BOOL)
    {
        //TODO- add here a check if e1 and e3 are in the same type or we can cast them..
        errorMismatch(yylineno);
    }
    Exp* temp = (e2->value.compare("True") == 0)? e1 : e3;
    this->value = temp->value;
    this->type = temp->type;
}

//Binop
Exp::Exp(Exp* e1, Node* n1, Exp* e2)
{
    if((e1->type != V_INT && e1 ->type != V_BYTE) || (e2->type != V_INT && e2->type != V_BYTE))
    {
        //some error according to the errors, TODO
    }

    if(e1->type == V_INT || e2->type == V_INT)
        this->type = V_INT;

    else
        this->type = V_BYTE;
    
    int num1 = stoi(e1->value);
    int num2 = stoi(e2->value);
    int num3;
    string op = n1->value;
    if(op.compare("+") == 0)
        num3 = num1+num2;

    else if(op.compare("-") == 0)
        num3 = num1-num2;

    else if(op.compare("*") == 0)
        num3 = num1 * num2;

    else if(op.compare("/") == 0)
        num3 = num1 / num2;
    
    else;
        //shouldnt get here, but this means that the n1 is not binop..

    if(this->type == V_BYTE && num3 > 255)
        errorByteTooLarge(yylineno, to_string(num3));

    this->value = to_string(num3);
}

//Boolean
Exp::Exp(Var_Type type, Exp* e1, Node* n1, Exp* e2)
{
    this->value = "this is boolean expression";
    this->type = V_BOOL;
    if(e1->type == V_BOOL || e2 ->type == V_BOOL)
    {
        if(n1->value.compare("and") == 0)
            this->bool_value = e1->bool_value && e2->bool_value;
        
        else if (n1->value.compare("or") == 0)
            this->bool_value = e1->bool_value || e2->bool_value;
        
        else
        {
            //error, TODO
        }
    }

    else if((e1->type == V_INT || e1->type == V_BYTE) && (e2->type == V_INT  || e2->type == V_BYTE))
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
            return; //error, need to refill.
    }
    else
    {
        //error
    }
}

Exp::Exp(Node* n1, Exp* e1)
{
    if(e1->type != V_BOOL || n1->value.compare("not") != 0)
        return; //TODO, fill error
    
    this->type = V_BOOL;
    this->bool_value = !e1->bool_value;
}

Exp::Exp(Type* t1, Exp* e1)
{
    if(t1->type != e1->type)
    {
        if(t1->type == V_BYTE && e1->type == V_INT) 
        {
            if(stoi(e1->value)>255)
                return; //error because byte is bigger than 255
        }

        else if (!(t1->type == V_INT && e1->type == V_BYTE))
            return; //errorrrrrrrrrrrrrrrrr
    }
    this->type = t1->type;
    this->value = e1->value;
}

Exp::Exp(Call* c)
{
    //TODO- fill this after we figure out what call is...
}

Exp::Exp(Node* n)
{
    this->value = n->value;
    if(n->value.compare("true") == 0)
    {
        this->bool_value = true;
        this->type = V_BOOL;
    }
    else if (n->value.compare("false") == 0)
    {
        this->bool_value = false;
        this->type = V_BOOL;
    }
    else if(n->value.compare("num") == 0)
        this->type = V_INT;
    else if(n->value.compare("string") == 0)
        this->type = V_STRING;
}

Exp::Exp(Node* n1, Node* n2)
{
    if(n1->value.compare("num") != 0 || n2->value.compare("b") != 0)
        return; //errorrrrr shouldnt get here..
    this->type = V_BYTE;
    this->value = n1->value;

}