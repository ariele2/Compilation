#include "sym_table.h"


void SymbolTable::PushDefaultFunctions() {
    // push print
    std::string message_name = "message";
    // SimpleSymbol(name, offset, type)
    SimpleSymbol print_param_symbol(message_name, -1, STRING_TYPE);

    std::string print_name = "print";
    SSList print_args;
    print_args.emplace_back(print_param_symbol);
    // STypeFunctionSymbol(name, type, list of SimpleSymbol)
    auto print_func = make_shared<STypeFunctionSymbol>(print_name, VOID_TYPE, print_args);
    AddFunction(print_func);

    // push printi
    std::string number_name = "number";
    SimpleSymbol printi_param_symbol(number_name, -1, INT_TYPE);

    std::string printi_name = "printi";
    SSList printi_args;
    printi_args.emplace_back(printi_param_symbol);
    auto printi_func = make_shared<STypeFunctionSymbol>(printi_name, VOID_TYPE, printi_args);
    AddFunction(printi_func);
}

void SymbolTable::PushScope(ScopeType scope_type) {
    Type ret_type;
    bool inside_while;

    if (scope_type == SCOPE_GLOBAL) {
        ret_type = OTHER_TYPE;
        inside_while = false;
    } else {
        assert(!scope_stack.empty());
        ret_type = scope_stack.top()->ret_type;
        inside_while = scope_stack.top()->inside_while;
    }

    if (scope_type == SCOPE_WHILE) {
        inside_while = true;
    }

   

    scope_stack.push(make_shared<Scope>(scope_type, current_offset, ret_type, inside_while));
}

void SymbolTable::PushFunctionScope(ScopeType scope_type, Type ret_type, STypeFunctionSymbolPtr function_symbol) {
    assert(!scope_stack.empty());
    bool inside_while = scope_stack.top()->inside_while;
    scope_stack.push(make_shared<Scope>(scope_type, current_offset, ret_type, inside_while));


}

void SymbolTable::PopScope() {
    endOfScope();

    // in global scope - functions only; in non-global scope - variables only
    if (scope_stack.top()->scope_type == SCOPE_GLOBAL) {
        for (const auto &func_symbol:scope_stack.top()->symbols) {
            assert(func_symbol->general_type == FUNCTION_TYPE);
            auto dynamic_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(func_symbol);
            std::vector<std::string> string_types;
            SSListToStrings(dynamic_cast_func->parameters, string_types);
            std::string ret_type = TypeToString(dynamic_cast_func->ret_type);
            idPrint(dynamic_cast_func->name, 0, makeFunctionType(ret_type, string_types));
            symbols_map.erase(dynamic_cast_func->name);
        }

    } else {
        for (const auto &basic_symbol:scope_stack.top()->symbols) {
            assert(basic_symbol->general_type != FUNCTION_TYPE);
            std::string type = TypeToString(basic_symbol->general_type);
            idPrint(basic_symbol->name, basic_symbol->offset, type);
            symbols_map.erase(basic_symbol->name);
        }
    }


    current_offset = scope_stack.top()->offset;
    scope_stack.pop();
}

SymbolTable::SymbolTable() : current_offset(0), symbols_map(), scope_stack() {
    PushScope(SCOPE_GLOBAL);
    PushDefaultFunctions();

}

void SymbolTable::AddParam(const SymbolPtr &symbol) {
    assert(!scope_stack.empty());
    scope_stack.top()->symbols.push_back(symbol);
    symbols_map.emplace(symbol->name, symbol);
}

void SymbolTable::AddVariable(const SymbolPtr &symbol) {
    // add params only after adding the function
    assert(!scope_stack.empty());
    symbol->offset = current_offset++;
    scope_stack.top()->symbols.push_back(symbol);
    symbols_map.emplace(symbol->name, symbol);
}

void SymbolTable::AddFunction(const STypeFunctionSymbolPtr &symbol) {
    assert(!scope_stack.empty());

    // set offsets
    symbol->offset = 0;
    auto curr_param_offset = 0;
    for (auto &param:symbol->parameters) {
        param.offset = --curr_param_offset;
    }

    scope_stack.top()->symbols.push_back(symbol);
    symbols_map.emplace(symbol->name, symbol);
}

bool SymbolTable::IsSymbolDefined(std::string &symbol_name) {
    return (symbols_map.find(symbol_name) != symbols_map.end());
}

SymbolPtr SymbolTable::GetDefinedSymbol(std::string &symbol_name) {
    return symbols_map[symbol_name];
}


Scope::Scope(ScopeType scope_type, int offset, Type ret_type, bool inside_while)
        : scope_type(scope_type), offset(offset), ret_type(ret_type), inside_while(inside_while) {
}