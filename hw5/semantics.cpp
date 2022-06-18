#include "semantics.h"
#define LOWER 0
#define UPPER 255

SemanticChecks::SemanticChecks(SymbolTable &table) : table_ref(table)
{
}

bool SemanticChecks::CheckSymDefined(string &n)
{
    return table_ref.IsSymbolDefined(n);
}

bool SemanticChecks::CheckMainIsDefined()
{
    for (auto map_pair : table_ref.symbols_map)
    {
        if (FUNCTION_TYPE == map_pair.second->generation_type)
        {
            auto dyn_cast_func = dynamic_pointer_cast<FuncSymType>(map_pair.second);
            if ("main"==dyn_cast_func->name )
            {
                if ((!CheckGeneralType(dyn_cast_func->ret_type, VOID_TYPE)) || (!(dyn_cast_func->params.empty())))
                    return false;
                else
                    return true;
            }
        }
    }
    return false;
}

bool SemanticChecks::CheckAssigned(Ty f, Ty s)
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

bool SemanticChecks::CheckCall(FuncSymbolTypePtr &func, ExpListTypePtr &exp_list)
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

bool SemanticChecks::CheckReturn(Ty t)
{
    bool check_type = CheckAssigned(table_ref.scope_stack.top()->return_type, t);
    return check_type;
}

bool SemanticChecks::CheckFunction(Ty t)
{
    bool check_type = CheckGeneralType(t, FUNCTION_TYPE);
    return check_type;
}

bool SemanticChecks::CheckVoid(Ty t)
{
    bool check_type = CheckGeneralType(t, VOID_TYPE);
    return check_type;
}

bool SemanticChecks::CheckBool(Ty t)
{
    bool check_type = CheckGeneralType(t, BOOL_TYPE);
    return check_type;
}

bool SemanticChecks::CheckGeneralType(Ty t, GeneralType gt)
{
    return gt == t;
}

bool SemanticChecks::CheckTypeType(Ty t1, Ty t)
{
    return t1 == t;
}

bool SemanticChecks::CheckBreak()
{
    bool is_in_while(table_ref.scope_stack.top()->inside_while);
    return is_in_while;
}

bool SemanticChecks::CheckContinue()
{
    return CheckBreak();
}

bool SemanticChecks::CheckOFByte(int &num)
{
    return (num <= UPPER && num >= LOWER);
}

bool SemanticChecks::CheckRelop(Ty f, Ty s)
{

    return (CheckGeneralType(s, BYTE_TYPE) || CheckGeneralType(s, INT_TYPE)) && (CheckGeneralType(f, BYTE_TYPE) || CheckGeneralType(f, INT_TYPE));
}

Ty SemanticChecks::CheckAndGetBinOpType(Ty f, Ty s)
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

bool SemanticChecks::CheckCast(Ty f, Ty s)
{
    return CheckAssigned(f, s);
}
