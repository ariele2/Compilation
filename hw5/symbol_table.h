#ifndef HW3_SYMBOL_TABLE_H
#define HW3_SYMBOL_TABLE_H

#include "typedefs.h"
#include "hw3_output.hpp"

using namespace output;

enum ScopeType {
    STATEMENT_SCOPE,
    FUNCTION_SCOPE,
    WHILE_SCOPE,
    SWITCH_SCOPE,
    GLOBAL_SCOPE
};

class Scope {
public:
    ScopeType scope_type;
    int offset;
    vector<SymbolTypePtr> symbols;
    Type return_type;
    bool inside_while;
    bool inside_switch;
    string while_continue_label;
    branch_list_ptr break_list;

    Scope(ScopeType scope_type, int offset, Type ret_type, bool inside_while, bool inside_switch,
          string while_continue_label, branch_list_ptr break_list);
};

class SymbolTable {
public:
    int current_offset;
    unordered_map<string, SymbolTypePtr> symbols_map;
    stack<ScopePtr> scope_stack;

    void PushDefaultFunctions();

    void PushScope(ScopeType scope_type);

    void PushFunctionScope(Type ret_type);

    void PopScope();

    SymbolTable();

    void AddParam(const SymbolTypePtr& symbol);

    void AddVariable(const SymbolTypePtr& symbol);

    void AddFunction(const FuncSymbolTypePtr & symbol);

    bool IsSymbolDefined(string &symbol_name);

    SymbolTypePtr GetDefinedSymbol(string& symbol_name);

};


#endif //HW3_SYMBOL_TABLE_H
