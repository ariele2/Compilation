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

int LexToken(int bison_enum, string token)
{
    BaseTypePtr result = nullptr;

    switch (bison_enum)
    {
    case NUM:
    {
        result.reset(new NumberType(token));
    }
    break;
    case AUTO:
    {
        result.reset(new AutoType(token));
    }
    break;
    case STRING:
    {
        result.reset(new StringType(token));
    }
    break;
    case INEQUALITY:
    {
        result.reset(new StringType(token));
    }
    break;
    case MULT_DIV:
        result.reset(new StringType(token));
        break;
    case PLUS_MINUS:
        result.reset(new StringType(token));
        break;

    case ID:
    {
        result.reset(new StringType(token));
    }
    break;
    case EQUAL_UNEQUAL:
    {
        result.reset(new StringType(token));
    }
    break;

    default:
        break;
    }

    yylval = result;

    return bison_enum;
}

#endif // HW3_SCANNER_H
