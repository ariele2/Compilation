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
    return (table_ref.scope_stack.top()->inside_while);
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
    for (std::pair<const std::string, SymbolPtr> map_pair : table_ref.symbols_map)
    {
        if (map_pair.second->general_type == FUNCTION_TYPE)
        {
            std::shared_ptr<STypeFunctionSymbol> dynamic_cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(map_pair.second);
            if (dynamic_cast_function->name == "main")
            {
                if (!dynamic_cast_function->ret_type == VOID_TYPE || !dynamic_cast_function->parameters.empty())
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
    return table_ref.IsSymbolDefined(name);
}

bool checkSemantics::checkCall(STypeFunctionSymbolPtr &func, STypeExpListPtr &exp_list)
{
    if (exp_list->exp_list.size() != func->parameters.size())
    {
        return false;
    }
    for (size_t i = 0; i < func->parameters.size(); ++i)
    {
        if (!checkAssigned(func->parameters[i].general_type, exp_list->exp_list[i].general_type))
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
    return checkAssigned(table_ref.scope_stack.top()->ret_type, type);
}

bool checkSemantics::checkBool(Type type)
{
    bool is_ok = (BOOL_TYPE == type);
    return is_ok;
}

bool checkSemantics::checkCast(Type f, Type s)
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

checkSemantics::checkSemantics(SymbolTable &table) : table_ref(table) {}