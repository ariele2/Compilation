#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "types.h"
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
    std::vector<SimpleSymbolPtr> symbols;
    Type ret_type;
    bool inside_while;

    Scope(ScopeType scope_type, int offset, Type ret_type, bool inside_while);
};

class SymbolTable {
public:
    int current_offset;
    std::unordered_map<std::string, SimpleSymbolPtr> symbols_map;
    std::stack<ScopePtr> scope_stack;

    void PushDefaultFunctions();

    void PushScope(ScopeType scope_type);

    void PushFunctionScope(ScopeType scope_type, Type ret_type, STypeFunctionSymbolPtr function_symbol);

    void PopScope();

    SymbolTable();

    void AddParam(const SimpleSymbolPtr& symbol);

    void AddVariable(const SimpleSymbolPtr& symbol);

    void AddFunction(const STypeFunctionSymbolPtr & symbol);

    bool IsSymbolDefined(std::string &symbol_name);

    SimpleSymbolPtr GetDefinedSymbol(std::string& symbol_name);

};

#endif // SYM_TABLE_H