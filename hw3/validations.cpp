#include "validations.h"

#define LOWER 0
#define UPPER 255

bool Validator::checkGeneralType(Type t, GeneralType gt){
    return t==gt;
}

bool Validator::checkTypeType(Type t, Type t1){
    return t==t1;
}

bool Validator::checkFunction(Type t) {
    
    return checkGeneralType(t, FUNCTION_TYPE);
}

bool Validator::checkContinue() {
    return checkBreak();
}

bool Validator::checkRelop(Type f, Type s) {
    return (checkGeneralType(s , BYTE_TYPE) || checkGeneralType(s , INT_TYPE)) && (checkGeneralType(f , BYTE_TYPE) || checkGeneralType(f , INT_TYPE));
}

bool Validator::checkBreak() {
    return (table_ref.s_stack.top()->is_in_while);
}

Type Validator::checkBinop(Type f, Type s) {
    if (!checkRelop(f, s)) {
        return OTHER_TYPE;
    }
    else if (checkGeneralType(s,INT_TYPE ) || checkGeneralType(f,INT_TYPE )) {
        return INT_TYPE;
    }
    else
        return BYTE_TYPE;
}

bool Validator::checkMainIsDefined() {
    for (std::pair<const std::string, SymPtr> map_pair : table_ref.syms_map) {
        if (map_pair.second->g_type == FUNCTION_TYPE) {
            std::shared_ptr<FuncSymType> dynamic_cast_function = std::dynamic_pointer_cast<FuncSymType>(map_pair.second);
            if (dynamic_cast_function->n == "main") {
                if ((!checkGeneralType(dynamic_cast_function->ret_type , VOID_TYPE)) || (!(dynamic_cast_function->parameters.empty())))
                    return false;
                else
                    return true;
            }
        }
    }
    return false;
}

bool Validator::checkOFByte(int &num) {
    return (num <= UPPER && num >= LOWER);
}

bool Validator::checkSymbolDefined(string &name) {
    return table_ref.checkIfSymbolDefined(name);
}

bool Validator::checkCall(FuncSymTypePtr &func, TExpListPtr &exp_list) {
    if (exp_list->expression_list.size() != func->parameters.size())
    {
        return false;
    }
    for (size_t i = 0; i < func->parameters.size(); ++i)
    {
        if (!checkAssigned(func->parameters[i].g_type, exp_list->expression_list[i].g_type)) {
            return false;
        }
    }
    return true;
}

bool Validator::checkAssigned(Type f, Type s) {
    if (checkTypeType(s , f) || (checkGeneralType(s,BYTE_TYPE) && checkGeneralType(f,INT_TYPE))) {
        return true;
    }
    else {
        return false;
    }
}

bool Validator::checkVoid(Type t) {
    return checkGeneralType(t,VOID_TYPE);
}

bool Validator::checkReturn(Type t) {
    return checkAssigned(table_ref.s_stack.top()->return_type, t);
}

bool Validator::checkBool(Type t) {
    return checkGeneralType(t,BOOL_TYPE);
}

bool Validator::checkCast(Type f, Type s) {
    if (checkTypeType(s , f) || (checkGeneralType(s , INT_TYPE) && checkGeneralType(f , BYTE_TYPE)) || (checkGeneralType(s, BYTE_TYPE) && checkGeneralType(f , INT_TYPE))) {
        return true;
    }
    else {
        return false;
    }
}

Validator::Validator(SymTable &table) : table_ref(table) {}