#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "types.h"
#include "hw3_output.hpp"

using namespace output;

enum ScopeType
{
    STATEMENT_SCOPE,
    FUNCTION_SCOPE,
    WHILE_SCOPE,
    SWITCH_SCOPE,
    GLOBAL_SCOPE
};

class Scope
{
public:
    Ty return_type;
    string while_continue_label;
    br_list_pointer break_list;
    ScopeType scope_type;
    bool inside_while;

    int offset;
    vector<SymbolTypePtr> symbols;

    bool inside_switch;

    Scope(ScopeType scope_type, int offset, Ty ret_type, bool inside_while, bool inside_switch,
          string while_continue_label, br_list_pointer break_list);
};

class SymbolTable
{
public:
    int current_offset;
    unordered_map<string, SymbolTypePtr> symbols_map;
    stack<ScopePointer> scope_stack;
    void AddVariable(const SymbolTypePtr &symbol);
    void PushScope(ScopeType scope_type);
    void AddFunction(const FuncSymbolTypePtr &symbol);
    void AddParam(const SymbolTypePtr &symbol);
    SymbolTypePtr GetDefinedSymbol(string &symbol_name);
    void PushFunctionScope(Ty ret_type);
    void PopScope();
    bool IsSymbolDefined(string &symbol_name);
    SymbolTable();
    void PushDefaultFunctions();
};

#endif // SYM_TABLE_H
