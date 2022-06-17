#include "symbol_table.h"

#include <utility>


void SymbolTable::PushDefaultFunctions() {
    // push print
    string message_name = "message";
    SymbolType print_param_symbol(message_name, -1, STRING_TYPE);

    string print_name = "print";
    ArgList print_args;
    print_args.emplace_back(print_param_symbol);
    auto print_func = make_shared<FuncSymType>(print_name, VOID_TYPE, print_args);
    AddFunction(print_func);

    // push printi
    string number_name = "number";
    SymbolType printi_param_symbol(number_name, -1, INT_TYPE);

    string printi_name = "printi";
    ArgList printi_args;
    printi_args.emplace_back(printi_param_symbol);
    auto printi_func = make_shared<FuncSymType>(printi_name, VOID_TYPE, printi_args);
    AddFunction(printi_func);
}

void SymbolTable::PushScope(ScopeType scope_type) {
    // copy everything from top scope, unless global. the calling functions handle other stuff
    Type ret_type;
    bool inside_while, inside_switch;
    string while_continue_label;
    branch_list_ptr break_list;

    if (scope_type == GLOBAL_SCOPE) {
        // labels and lists are empty, no need to init
        ret_type = ERROR_TYPE;
        inside_while = false;
        inside_switch = false;
    } else {
        assert(!scope_stack.empty());
        ret_type = scope_stack.top()->return_type;
        inside_while = scope_stack.top()->inside_while;
        inside_switch = scope_stack.top()->inside_switch;
        while_continue_label = scope_stack.top()->while_continue_label;
        break_list = scope_stack.top()->break_list;
    }

    scope_stack.push(make_shared<Scope>(scope_type, current_offset, ret_type, inside_while, inside_switch,
                                        while_continue_label, break_list));
}

void SymbolTable::PushFunctionScope(Type ret_type) {
    PushScope(FUNCTION_SCOPE);
    scope_stack.top()->return_type = ret_type;

}

void SymbolTable::PopScope() {
    if (PRINT_DEBUG) {
        endScope();
    }

    // in global scope - functions only; in non-global scope - variables only
    if (scope_stack.top()->scope_type == GLOBAL_SCOPE) {
        for (const auto &func_symbol:scope_stack.top()->symbols) {
            assert(func_symbol->g_type == FUNCTION_TYPE);
            auto dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(func_symbol);
            vector<string> string_types;
            ArgListToStrings(dynamic_cast_func->params, string_types);
            string ret_type = TypeToString(dynamic_cast_func->ret_type);
            if (PRINT_DEBUG) {
                printID(dynamic_cast_func->name, 0, makeFunctionType(ret_type, string_types));
            }
            symbols_map.erase(dynamic_cast_func->name);
        }

    } else {
        for (const auto &basic_symbol:scope_stack.top()->symbols) {
            assert(basic_symbol->g_type != FUNCTION_TYPE);
            string type = TypeToString(basic_symbol->g_type);
            if (PRINT_DEBUG) {
                printID(basic_symbol->name, basic_symbol->offset, type);
            }
            symbols_map.erase(basic_symbol->name);
        }
    }


    current_offset = scope_stack.top()->offset;
    scope_stack.pop();
}

SymbolTable::SymbolTable() : current_offset(0), symbols_map(), scope_stack() {
    PushScope(GLOBAL_SCOPE);
    PushDefaultFunctions();

}

void SymbolTable::AddParam(const SymbolTypePtr &symbol) {
    assert(!scope_stack.empty());
    scope_stack.top()->symbols.push_back(symbol);
    symbols_map.emplace(symbol->name, symbol);
}

void SymbolTable::AddVariable(const SymbolTypePtr &symbol) {
    // add params only after adding the function
    assert(!scope_stack.empty());
    current_offset+= 4;
    symbol->offset = current_offset;
    scope_stack.top()->symbols.push_back(symbol);
    symbols_map.emplace(symbol->name, symbol);
}

void SymbolTable::AddFunction(const FuncSymbolTypePtr &symbol) {
    assert(!scope_stack.empty());

    // set offsets
    symbol->offset = 0;
    auto curr_param_offset = 0;
    for (auto &param:symbol->params) {
        param.offset = --curr_param_offset;
    }

    scope_stack.top()->symbols.push_back(symbol);
    symbols_map.emplace(symbol->name, symbol);
}

bool SymbolTable::IsSymbolDefined(string &symbol_name) {
    return (symbols_map.find(symbol_name) != symbols_map.end());
}

SymbolTypePtr SymbolTable::GetDefinedSymbol(string &symbol_name) {
    return symbols_map[symbol_name];
}


// the labels are initialized empty
Scope::Scope(ScopeType scope_type, int offset, Type ret_type, bool inside_while, bool inside_switch,
             string while_continue_label, branch_list_ptr break_list) :
        scope_type(scope_type), offset(offset), return_type(ret_type),
        inside_while(inside_while), inside_switch(inside_switch),
        while_continue_label(move(while_continue_label)), break_list(move(break_list)) {

}
