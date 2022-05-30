#include "sym_table.h"
#define MSG "message"
#define NUMBER "number"
#define PRINT "print"
#define PRINTI "printi"
#define INIT -1

SymTable::SymTable() : curr_offset(0), syms_map(), s_stack()
{
    addScope(SCOPE_GLOBAL);
    addDefFunctions();
}

void SymTable::AddParameter(const SymPtr &sym)
{
    assert(!s_stack.empty());
    s_stack.top()->syms.push_back(sym);
    syms_map.emplace(sym->n, sym);
}

void SymTable::removeScope()
{
    endOfScope();
    if (s_stack.top()->s_type == SCOPE_GLOBAL)
    {
        for (const SymPtr &func_symbol : s_stack.top()->syms)
        {
            assert(FUNCTION_TYPE == func_symbol->general_type);
            std::shared_ptr<FuncSymType> dynamic_cast_func = std::dynamic_pointer_cast<FuncSymType>(func_symbol);
            std::vector<std::string> string_types;
            SSListToStrings(dynamic_cast_func->parameters, string_types);
            std::string ret_type = TypeToString(dynamic_cast_func->ret_type);
            idPrint(dynamic_cast_func->n, 0, makeFunctionType(ret_type, string_types));
            syms_map.erase(dynamic_cast_func->n);
        }
    }
    else
    {
        for (const SymPtr &basic_sym : s_stack.top()->syms)
        {
            assert(FUNCTION_TYPE != basic_sym->general_type);
            std::string type = TypeToString(basic_sym->general_type);
            idPrint(basic_sym->n, basic_sym->offs, type);
            syms_map.erase(basic_sym->n);
        }
    }

    curr_offset = s_stack.top()->offset;
    s_stack.pop();
}

Scope::Scope(TypesOfScopes scope_type, int offset, Type ret_type, bool inside_while)
    : s_type(scope_type), offset(offset), return_type(ret_type), is_in_while(inside_while)
{
}

SymPtr SymTable::retDefinedSym(std::string &sym_name)
{
    return syms_map[sym_name];
}

void SymTable::addDefFunctions()
{

    std::string message_name = MSG;
    std::string print_name = PRINT;
    std::string number_name = NUMBER;
    std::string printi_name = PRINTI;

    SimpleSymbol print_param_symbol(message_name, INIT, STRING_TYPE);

    SimpleSymsList print_args;
    print_args.emplace_back(print_param_symbol);

    auto print_func = make_shared<FuncSymType>(print_name, VOID_TYPE, print_args);
    AddFunc(print_func);

    SimpleSymbol printi_param_symbol(number_name, INIT, INT_TYPE);

    SimpleSymsList printi_args;
    printi_args.emplace_back(printi_param_symbol);
    auto printi_func = make_shared<FuncSymType>(printi_name, VOID_TYPE, printi_args);
    AddFunc(printi_func);
}

void SymTable::AddVar(const SymPtr &sym)
{

    assert(!s_stack.empty());
    sym->offs = curr_offset++;
    s_stack.top()->syms.push_back(sym);
    syms_map.emplace(sym->n, sym);
}

void SymTable::AddFunc(const FuncSymTypePtr &sym)
{
    assert(!s_stack.empty());
    sym->offs = 0;
    auto cur_offset = 0;
    for (SimpleSymbol &param : sym->parameters)
    {
        param.offs = --cur_offset;
    }

    s_stack.top()->syms.push_back(sym);
    syms_map.emplace(sym->n, sym);
}

void SymTable::addScope(TypesOfScopes scope_type)
{
    Type ret_type;
    bool inside_while;

    if (SCOPE_GLOBAL != scope_type)
    {
        assert(!s_stack.empty());
        ret_type = s_stack.top()->return_type;
        inside_while = s_stack.top()->is_in_while;
    }
    else
    {
        inside_while = false;
        ret_type = OTHER_TYPE;
    }

    if (SCOPE_WHILE == scope_type)
    {
        inside_while = true;
        s_stack.push(make_shared<Scope>(scope_type, curr_offset, ret_type, inside_while));
    }
    else
    {
        s_stack.push(make_shared<Scope>(scope_type, curr_offset, ret_type, inside_while));
    }
}

void SymTable::addFunctionScope(TypesOfScopes scope_type, Type ret_type, FuncSymTypePtr function_symbol)
{
    assert(!s_stack.empty());
    bool inside_while = s_stack.top()->is_in_while;
    s_stack.push(make_shared<Scope>(scope_type, curr_offset, ret_type, inside_while));
}

bool SymTable::checkIfSymbolDefined(std::string &sym_name)
{
    return (syms_map.end() != syms_map.find(sym_name));
}
