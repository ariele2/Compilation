#ifndef HW3_SCANNER_H
#define HW3_SCANNER_H

#include "types.h"
#include "parser.tab.hpp"
#include "hw3_output.hpp"

using namespace output;

int LexToken(int bison_enum, std::string token) {
    Tptr result = nullptr;

    switch (bison_enum) {
        case NUM: {
            result.reset(new STypeNumber(token));
        }
            break;
        case MUL:
        case DIV:
        case ADD:
        case SUB:
        case ID:
        case STRING:
        case NOT_EQUALITY:
        case EQUAL_UNEQUAL: {
            result.reset(new STypeString(token));
        }
            break;
        case AUTO: {
            result.reset(new AutoType(token));
        }
            break;
        default:
            break;
    }

    yylval = result;

    return bison_enum;
}

void errorLexAndExit(int lineno){
    errorInLexical(lineno);
    exit(0);
}

#endif //HW3_SCANNER_H