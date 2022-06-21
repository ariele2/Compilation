#include "validations.h"
#define LOWER 0
#define UPPER 255

Validations::Validations(SymbolTable &table) : table_ref(table)
{
}


bool Validations::CheckGeneralType(Ty t, GeneralType gt)
{
    return gt == t;
}

bool Validations::CheckTypeType(Ty t1, Ty t)
{
    return t1 == t;
}


bool Validations::CheckContinue()
{
    return CheckBreak();
}


bool Validations::CheckCall(FuncSymbolTypePtr &func, ExpListTypePtr &exp_list)
{
    if (exp_list->exp_list.size() != func->params.size())
    {
        return false;
    }

    for (size_t i = 0; i < func->params.size(); ++i)
    {
        if (!CheckAssigned(func->params[i].generation_type, exp_list->exp_list[i]->generation_type))
        {
            return false;
        }
    }
    return true;
}

bool Validations::CheckReturn(Ty t)
{
    bool check_type = CheckAssigned(table_ref.scope_stack.top()->return_type, t);
    return check_type;
}
bool Validations::CheckOFByte(int &num)
{
    return (num <= UPPER && num >= LOWER);
}

bool Validations::CheckRelop(Ty f, Ty s)
{

    return (CheckGeneralType(s, BYTE_TYPE) || CheckGeneralType(s, INT_TYPE)) && (CheckGeneralType(f, BYTE_TYPE) || CheckGeneralType(f, INT_TYPE));
}

Ty Validations::CheckAndGetBinOpType(Ty f, Ty s)
{
    if (!CheckRelop(f, s))
    {
        return ERROR_TYPE;
    }
    if (!(CheckGeneralType(f, INT_TYPE) || CheckGeneralType(s, INT_TYPE)))
    {
        return BYTE_TYPE;
    }
    return INT_TYPE;
}

bool Validations::CheckFunction(Ty t)
{
    bool check_type = CheckGeneralType(t, FUNCTION_TYPE);
    return check_type;
}

bool Validations::CheckSymDefined(string &n)
{
    return table_ref.IsSymbolDefined(n);
}

bool Validations::CheckVoid(Ty t)
{
    bool check_type = CheckGeneralType(t, VOID_TYPE);
    return check_type;
}

bool Validations::CheckBool(Ty t)
{
    bool check_type = CheckGeneralType(t, BOOL_TYPE);
    return check_type;
}

bool Validations::CheckMainIsDefined()
{
    for (auto map_pair:table_ref.symbols_map) {
        if (map_pair.second->generation_type == FUNCTION_TYPE) {
            auto dyn_cast_func = dynamic_pointer_cast<FuncSymType>(map_pair.second);
            if (dyn_cast_func->name == "main") {
                if (dyn_cast_func->params.empty() && dyn_cast_func->ret_type == VOID_TYPE) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    return false;
}


bool Validations::CheckAssigned(Ty f, Ty s)
{
    if (CheckTypeType(s, f) || (CheckGeneralType(s, BYTE_TYPE) && CheckGeneralType(f, INT_TYPE)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Validations::CheckBreak()
{
    bool is_in_while(table_ref.scope_stack.top()->inside_while);
    return is_in_while;
}




bool Validations::CheckCast(Ty f, Ty s)
{
    return CheckAssigned(f, s);
}
