#include "types.h"


TBase::TBase() : general_type(OTHER_TYPE) {}

TBase::TBase(Type type) : general_type(type) {}

STypeString::STypeString(std::string &token) : TBase(STRING_TYPE), token(token) {}

STypeNumber::STypeNumber(std::string &token_string) : TBase(INT_TYPE) {
    token = stoi(token_string);
}

STypeBool::STypeBool(bool token) : TBase(BOOL_TYPE), token(token) {
}

STypeCType::STypeCType(Type type) : TBase(type) {

}

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
            return "UNRECOGNIZED"; // OH NOES
    }
}

void SSListToStrings(SSList &symbols_list, std::vector<std::string> &string_vector) {
    string_vector.clear();
    for (const auto &exp:symbols_list) {
        string_vector.push_back(TypeToString(exp.general_type));
    }
}

SimpleSymbol::SimpleSymbol(std::string &name, int offset, Type type) : TBase(type), name(name),
                                                                offset(offset) {

}


SimpleSymbolList::SimpleSymbolList() : symbols_list() {

}

SimpleSymbolList::SimpleSymbolList(SSList &symbols_list) : symbols_list(symbols_list) {

}

STypeFunctionSymbol::STypeFunctionSymbol(std::string &symbol_name, Type symbol_type, SSList &symbols_list)
        : SimpleSymbol(symbol_name, 0, FUNCTION_TYPE), parameters(symbols_list), ret_type(symbol_type) {

}

STypeExpList::STypeExpList() : exp_list() {

}

STypeExpList::STypeExpList(ExpList &exp_list) : exp_list(exp_list) {

}