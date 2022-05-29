#include "types.h"


TermianlBase::TermianlBase() : general_type(OTHER_TYPE) {}

TermianlBase::TermianlBase(Type type) : general_type(type) {}

STypeString::STypeString(std::string &token) : TermianlBase(STRING_TYPE), token(token) {}

STypeNumber::STypeNumber(std::string &token_string) : TermianlBase(INT_TYPE) {
    token = std::stoi(token_string);
}

STypeBool::STypeBool(bool token) : TermianlBase(BOOL_TYPE), token(token) {}

AutoType::AutoType(std::string& token) : TermianlBase(AUTO_TYPE), token(token) {}

STypeCType::STypeCType(Type type) : TermianlBase(type) {}


std::string TypeToString(Type type) {

    switch (type) {
        case STRING_TYPE:
            return "STRING";
        case BOOL_TYPE:
            return "BOOL";
        case BYTE_TYPE:
            return "BYTE";
        case INT_TYPE:
            return "INT";
        case VOID_TYPE:
            return "VOID";
        default:
            return "UNRECOGNIZED";
    }
}

void SSListToStrings(SSList &symbols_list, std::vector<std::string> &string_list) {
    string_list.clear();
    for (const auto &expression:symbols_list) {
        string_list.push_back(TypeToString(expression.general_type));
    }
}

SimpleSymbol::SimpleSymbol(std::string &n, int offs, Type t) : TermianlBase(t), n(n),
                                                                offs(offs) {}


SimpleSymbolList::SimpleSymbolList() : syms_list() {}

SimpleSymbolList::SimpleSymbolList(SSList &syms_list) : syms_list(syms_list) {}

STypeFunctionSymbol::STypeFunctionSymbol(std::string &sym_name, Type sym_type, SSList &sym_list)
        : SimpleSymbol(sym_name, 0, FUNCTION_TYPE), parameters(sym_list), ret_type(sym_type) {}

STypeExpList::STypeExpList() : expression_list() {}

STypeExpList::STypeExpList(ExpList &expression_list) : expression_list(expression_list) {}