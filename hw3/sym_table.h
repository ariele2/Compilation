#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "types.h"
#include "hw3_output.hpp"

using namespace output;

enum ScopeType {
    SCOPE_STATEMENT,
    SCOPE_FUNCTION,
    SCOPE_WHILE,
    SCOPE_GLOBAL
};

class Scope {
public:
    ScopeType scope_type;
    int offset;
    std::vector<SymbolPtr> symbols;
    Type ret_type;
    bool inside_while;

    Scope(ScopeType scope_type, int offset, Type ret_type, bool inside_while);
};

class SymbolTable {
public:
    int current_offset;
    std::unordered_map<std::string, SymbolPtr> symbols_map;
    std::stack<ScopePtr> scope_stack;

    void PushDefaultFunctions();

    void PushScope(ScopeType scope_type);

    void PushFunctionScope(ScopeType scope_type, Type ret_type, STypeFunctionSymbolPtr function_symbol);

    void PopScope();

    SymbolTable();

    void AddParam(const SymbolPtr& symbol);

    void AddVariable(const SymbolPtr& symbol);

    void AddFunction(const STypeFunctionSymbolPtr & symbol);

    bool IsSymbolDefined(std::string &symbol_name);

    SymbolPtr GetDefinedSymbol(std::string& symbol_name);

};

#endif // SYM_TABLE_H