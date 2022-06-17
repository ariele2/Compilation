#ifndef HW3_SCANNER_H
#define HW3_SCANNER_H

#include "typedefs.h"
#include "parser.tab.hpp"
#include "hw3_output.hpp"

using namespace output;

void errorLexAndExit(int lineno)
{
    errorLex(lineno);
    exit(0);
}

int LexToken(int bison_enum, string token) {
    BaseTypePtr result = nullptr;

    switch (bison_enum) {
        case NUM:
            result.reset(new NumberType(token));
            break;
        case MULT_DIV:
        case PLUS_MINUS:
        case ID:
        case STRING:
        case INEQUALITY:
        case EQUAL_UNEQUAL:
            result.reset(new StringType(token));
            break;
        default:
            break;
    }

    yylval = result;

    return bison_enum;
}


#endif // HW3_SCANNER_H
