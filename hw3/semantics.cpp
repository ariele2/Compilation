#include "semantics.h"

#define LOWER 0
#define UPPER 255

bool checkSemantics::checkFunction(Type type)
{
    bool is_ok = (FUNCTION_TYPE == type);
    return is_ok;
}

bool checkSemantics::checkContinue()
{
    return checkBreak();
}

bool checkSemantics::checkRelop(Type f, Type s)
{
    return (s == BYTE_TYPE || s == INT_TYPE) && (f == BYTE_TYPE || f == INT_TYPE);
}

bool checkSemantics::checkBreak()
{
    return (table_ref.s_stack.top()->is_in_while);
}

Type checkSemantics::checkBinop(Type f, Type s)
{
    if (!checkRelop(f, s))
    {
        return OTHER_TYPE;
    }
    else if (INT_TYPE == s || INT_TYPE == f)
    {
        return INT_TYPE;
    }
    else
        return BYTE_TYPE;
}

bool checkSemantics::checkMainIsDefined()
{
    for (std::pair<const std::string, SymbolPtr> map_pair : table_ref.syms_map)
    {
        if (map_pair.second->general_type == FUNCTION_TYPE)
        {
            std::shared_ptr<STypeFunctionSymbol> dynamic_cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(map_pair.second);
            if (dynamic_cast_function->n == "main")
            {
                if ((!(dynamic_cast_function->ret_type == VOID_TYPE)) || (!(dynamic_cast_function->parameters.empty())))
                    return false;
                else
                    return true;
            }
        }
    }
    return false;
}

bool checkSemantics::checkOFByte(int &num)
{
    bool is_of = (num <= UPPER && num >= LOWER);
    return is_of;
}

bool checkSemantics::checkSymbolDefined(string &name)
{
    return table_ref.checkIfSymbolDefined(name);
}

bool checkSemantics::checkCall(STypeFunctionSymbolPtr &func, TExpListPtr &exp_list)
{
    if (exp_list->expression_list.size() != func->parameters.size())
    {
        return false;
    }
    for (size_t i = 0; i < func->parameters.size(); ++i)
    {
        if (!checkAssigned(func->parameters[i].general_type, exp_list->expression_list[i].general_type))
        {
            return false;
        }
    }
    return true;
}

bool checkSemantics::checkAssigned(Type f, Type s)
{
    bool is_correct_case = (BYTE_TYPE == s && INT_TYPE == f);
    if (s == f)
    {
        return true;
    }
    else if (is_correct_case)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool checkSemantics::checkVoid(Type type)
{
    bool is_ok = (VOID_TYPE == type);
    return is_ok;
}

bool checkSemantics::checkReturn(Type type)
{
    return checkAssigned(table_ref.s_stack.top()->return_type, type);
}

bool checkSemantics::checkBool(Type type)
{
    bool is_ok = (BOOL_TYPE == type);
    return is_ok;
}

bool checkSemantics::checkCast(Type f, Type s)
{

    bool is_correct_case = (BYTE_TYPE == s && INT_TYPE == f);
    if (s == f || (s == INT_TYPE && f == BYTE_TYPE) || (s == INT_TYPE && f == BYTE_TYPE))
    {
        return true;
    }
    else if (is_correct_case)
    {
        return true;
    }
    else
    {
        return false;
    }
}

checkSemantics::checkSemantics(SymTable &table) : table_ref(table) {}