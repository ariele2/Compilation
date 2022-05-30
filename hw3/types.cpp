#include "types.h"


TermianlBase::TermianlBase() : general_type(OTHER_TYPE) {}

TermianlBase::TermianlBase(Type type) : general_type(type) {}

StringType::StringType(std::string &token) : TermianlBase(STRING_TYPE), token(token) {}

NumberType::NumberType(std::string &token_string) : TermianlBase(INT_TYPE) {
    token = std::stoi(token_string);
}

BoolType::BoolType(bool token) : TermianlBase(BOOL_TYPE), token(token) {}

AutoType::AutoType(std::string& token) : TermianlBase(AUTO_TYPE), token(token) {}

CType::CType(Type type) : TermianlBase(type) {}


std::string TypeToString(Type type) {
    if (type == STRING_TYPE) {
        return "STRING";
    }
    else if (type == BOOL_TYPE) {
        return "BOOL";
    }
    else if (type == BYTE_TYPE) {
        return "BYTE";
    }
    else if (type == INT_TYPE) {
        return "INT";
    }
    else if (type == VOID_TYPE) {
        return "VOID";
    }
    return "UNRECOGNIZED";
}

void SSListToStrings(SimpleSymsList &symbols_list, std::vector<std::string> &string_list) {
    string_list.clear();
    for (const auto &expression:symbols_list) {
        string_list.push_back(TypeToString(expression.general_type));
    }
}

SimpleSymbol::SimpleSymbol(std::string &n, int offs, Type t) : TermianlBase(t), n(n),
                                                                offs(offs) {}


SimpleSymbolList::SimpleSymbolList() : syms_list() {}

SimpleSymbolList::SimpleSymbolList(SimpleSymsList &syms_list) : syms_list(syms_list) {}

FuncSymType::FuncSymType(std::string &sym_name, Type sym_type, SimpleSymsList &sym_list)
        : SimpleSymbol(sym_name, 0, FUNCTION_TYPE), parameters(sym_list), ret_type(sym_type) {}

TExpList::TExpList() : expression_list() {}

TExpList::TExpList(ExpList &expression_list) : expression_list(expression_list) {}
