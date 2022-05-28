#include "semantics.h"

checkSemantics::checkSemantics(SymbolTable &table) : table_ref(table) {} // c'tor

bool checkSemantics::checkSymbolDefined(string &name) {
    return table_ref.IsSymbolDefined(name);
}

bool checkSemantics::checkMainIsDefined() {
    for (auto map_pair:table_ref.symbols_map) {
        if (map_pair.second->general_type == FUNCTION_TYPE) {
            auto dynamic_cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(map_pair.second);
            if (dynamic_cast_function->name == "main") {
                if (dynamic_cast_function->parameters.empty() && dynamic_cast_function->ret_type == VOID_TYPE) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    return false;
}

bool checkSemantics::checkAssigned(Type first, Type second) {
    if (first == second) {
        return true;
    }
    if (first == INT_TYPE && second == BYTE_TYPE) {
        return true;
    }

    return false;
}

bool checkSemantics::checkCall(STypeFunctionSymbolPtr &func, STypeExpListPtr &exp_list) {
    if (func->parameters.size() != exp_list->exp_list.size()) {
        return false;
    }

    for (size_t i = 0; i < func->parameters.size(); ++i) {
        if (!checkAssigned(func->parameters[i].general_type, exp_list->exp_list[i].general_type)) {
            return false;
        }
    }

    return true;
}

bool checkSemantics::checkReturn(Type type) {
    auto required_return_type = table_ref.scope_stack.top()->ret_type;
    return checkAssigned(required_return_type, type);
}

bool checkSemantics::checkBool(Type type) {
    return (type == BOOL_TYPE);
}

bool checkSemantics::checkVoid(Type type) {
    return (type == VOID_TYPE);
}

bool checkSemantics::checkFunction(Type type) {
    return (type == FUNCTION_TYPE);
}

bool checkSemantics::checkBreak() {
    return (table_ref.scope_stack.top()->inside_while);
}

bool checkSemantics::checkContinue() {
    return (table_ref.scope_stack.top()->inside_while);
}

bool checkSemantics::checkOFByte(int &num) {
    return (num >= 0 && num <= 255);
}

bool checkSemantics::checkRelop(Type first, Type second) {
    // all numeric types are ok
    if (first == INT_TYPE || first == BYTE_TYPE) {
        if (second == INT_TYPE || second == BYTE_TYPE) {
            return true;
        }
    }
    return false;
}

Type checkSemantics::checkBinop(Type first, Type second) {
    if (!checkRelop(first, second)) {
        return OTHER_TYPE;
    }
    if (first == INT_TYPE || second == INT_TYPE) {
        return INT_TYPE;
    }
    return BYTE_TYPE;
}

bool checkSemantics::checkCast(Type first, Type second) {
    return checkAssigned(first, second);
}