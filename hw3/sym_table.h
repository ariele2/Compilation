#ifndef HW3_SYMBOL_TABLE_H
#define HW3_SYMBOL_TABLE_H

#include "typedefs.h"
#include "hw3_output.hpp"

using namespace output;

enum ScopeType {
    STATEMENT_SCOPE,
    FUNCTION_SCOPE,
    WHILE_SCOPE,
    GLOBAL_SCOPE
};

class Scope {
public:
    ScopeType scope_type;
    int offset;
    std::vector<STypeSymbolPtr> symbols;
    Type ret_type;
    bool inside_while;

    Scope(ScopeType scope_type, int offset, Type ret_type, bool inside_while);
};

class SymbolTable {
public:
    int current_offset;
    std::unordered_map<std::string, STypeSymbolPtr> symbols_map;
    std::stack<ScopePtr> scope_stack;

    void PushDefaultFunctions();

    void PushScope(ScopeType scope_type);

    void PushFunctionScope(ScopeType scope_type, Type ret_type, STypeFunctionSymbolPtr function_symbol);

    void PopScope();

    SymbolTable();

    void AddParam(const STypeSymbolPtr& symbol);

    void AddVariable(const STypeSymbolPtr& symbol);

    void AddFunction(const STypeFunctionSymbolPtr & symbol);

    bool IsSymbolDefined(std::string &symbol_name);

    STypeSymbolPtr GetDefinedSymbol(std::string& symbol_name);

};

#endif