#include "typedefs.h"

#include <utility>

TerminalBase::TerminalBase() : g_type(ERROR_TYPE) {}

TerminalBase::TerminalBase(Type type) : g_type(type) {}

StringType::StringType(string &token) : TerminalBase(STRING_TYPE), token(token) {}

NumberType::NumberType(string &token_string) : TerminalBase(INT_TYPE) {
    token = stoi(token_string);
}

CType::CType(Type type) : TerminalBase(type) {

}

string TypeToString(Type type) {

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
        string_vector.push_back(TypeToString(exp.g_type));
    }
}

SymbolType::SymbolType(string &name, int offset, Type type) : TerminalBase(type), name(name),
                                                                offset(offset) {

}


ArgListType::ArgListType() : arg_list() {

}

ArgListType::ArgListType(ArgList &arg_list) : arg_list(arg_list) {

}

FuncSymType::FuncSymType(string &symbol_name, Type symbol_type, ArgList &arg_list)
        : SymbolType(symbol_name, 0, FUNCTION_TYPE), params(arg_list), ret_type(symbol_type) {

}

TExpList::TExpList() : exp_list() {

}

TExpList::TExpList(ExpList &exp_list) : exp_list(exp_list) {

}

RegisterType::RegisterType(register_name reg_name, Type type) : TerminalBase(type), reg_name(move(reg_name)) {

}

StatementType::StatementType(branch_list next_list) : TerminalBase(STATEMENT_TYPE), next_list(move(next_list)) {

}

BoolExpType::BoolExpType(branch_list true_list, branch_list false_list) : TerminalBase(BOOL_TYPE),
                                                                            true_list(move(true_list)),
                                                                            false_list(move(false_list)) {

}

CaseListType::CaseListType(case_label_list case_list, string default_label, branch_list next_list) :
        case_list(move(case_list)), default_label(move(default_label)), next_list(move(next_list)) {

}

CaseDeclType::CaseDeclType(int case_num, string case_label, branch_list next_list):
        case_num(case_num), case_label(move(case_label)), next_list(move(next_list)) {

}
