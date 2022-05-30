#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "types.h"
#include "hw3_output.hpp"

using namespace output;

enum TypesOfScopes
{
    SCOPE_STATEMENT,
    SCOPE_FUNCTION,
    SCOPE_WHILE,
    SCOPE_GLOBAL
};

class Scope
{
public:
    std::vector<SymPtr> syms;
    Type return_type;
    TypesOfScopes s_type;
    int offset;
    bool is_in_while;
    Scope(TypesOfScopes s_type, int offset, Type return_type, bool inside_while);
};

class SymTable
{
public:
    int curr_offset;
    std::stack<ScopePtr> s_stack;
    std::unordered_map<std::string, SymPtr> syms_map;
    void removeScope();
    void AddFunc(const FuncSymTypePtr &symbol);
    void addScope(TypesOfScopes s_type);
    void AddParameter(const SymPtr &symbol);
    void AddVar(const SymPtr &symbol);
    void addDefFunctions();
    void addFunctionScope(TypesOfScopes s_type, Type return_type, FuncSymTypePtr func_sym);
    bool checkIfSymbolDefined(std::string &symbol_name);
    SymPtr retDefinedSym(std::string &symbol_name);
    SymTable();
};

#endif // SYM_TABLE_H