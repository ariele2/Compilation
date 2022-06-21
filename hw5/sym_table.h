#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "types.h"
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
    Ty return_type;
    bool inside_while;
    bool inside_switch;
    string while_continue_label;
    br_list_pointer break_list;

    Scope(ScopeType scope_type, int offset, Ty ret_type, bool inside_while, bool inside_switch,
          string while_continue_label, br_list_pointer break_list);
};

class SymbolTable {
public:
    int current_offset;
    unordered_map<string, SymbolTypePtr> symbols_map;
    stack<ScopePointer> scope_stack;

    void PushDefaultFunctions();

    void PushScope(ScopeType scope_type);

    void PushFunctionScope(Ty ret_type);

    void PopScope();

    SymbolTable();

    void AddParam(const SymbolTypePtr& symbol);

    void AddVariable(const SymbolTypePtr& symbol);

    void AddFunction(const FuncSymbolTypePtr & symbol);

    bool IsSymbolDefined(string &symbol_name);

    SymbolTypePtr GetDefinedSymbol(string& symbol_name);

};


#endif // SYM_TABLE_H
