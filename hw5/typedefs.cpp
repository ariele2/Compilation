#include "typedefs.h"

#include <utility>

TerminalBase::TerminalBase() : generation_type(ERROR_TYPE) {}

TerminalBase::TerminalBase(Ty type) : generation_type(type) {}

StringType::StringType(string &token) : TerminalBase(STRING_TYPE), token(token) {}

NumberType::NumberType(string &token_string) : TerminalBase(INT_TYPE) {
    token = stoi(token_string);
}

CType::CType(Ty type) : TerminalBase(type) {

}

string TypeToString(Ty type) {

    switch (type) {
        case VOID_TYPE:
            return "VOID";
        case INT_TYPE:
            return "INT";
        case BYTE_TYPE:
            return "BYTE";
        case BOOL_TYPE:
            return "BOOL";
        case STRING_TYPE:
            return "STRING";
        default:
            return "OH NOES";
    }

}

void ArgListToStrings(ArgList &arg_list, vector<string> &string_vector) {
    string_vector.clear();
    for (const auto &exp:arg_list) {
        string_vector.push_back(TypeToString(exp.generation_type));
    }
}

SymbolType::SymbolType(string &name, int offset, Ty type) : TerminalBase(type), name(name),
                                                                offset(offset) {

}


ArgListType::ArgListType() : arguments_list() {

}

ArgListType::ArgListType(ArgList &arg_list) : arguments_list(arg_list) {

}

FuncSymType::FuncSymType(string &symbol_name, Ty symbol_type, ArgList &arg_list)
        : SymbolType(symbol_name, 0, FUNCTION_TYPE), params(arg_list), ret_type(symbol_type) {

}

TExpList::TExpList() : exp_list() {

}

TExpList::TExpList(ExpList &exp_list) : exp_list(exp_list) {

}

RegisterType::RegisterType(name_of_register reg_name, Ty type) : TerminalBase(type), reg_name(move(reg_name)) {

}

StatementType::StatementType(br_list next_list) : TerminalBase(STATEMENT_TYPE), next_list(move(next_list)) {

}

BoolExpType::BoolExpType(br_list true_list, br_list false_list) : TerminalBase(BOOL_TYPE),
                                                                            true_list(move(true_list)),
                                                                            false_list(move(false_list)) {

}



