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

char HexaToChar(const char s1, const char s2)
{
    int s = 0;

    if (s1 == 'a')
        s = 10 + s;
    else if (s1 == 'b')
        s = 11 + s;
    else if (s1 == 'c')
        s = 12 + s;
    else if (s1 == 'd')
        s = 13 + s;
    else if (s1 == 'e')
        s = 14 + s;
    else if (s1 == 'f')
        s = 15 + s;
    else if (s1 == 'A')
        s = 10 + s;
    else if (s1 == 'B')
        s = 11 + s;
    else if (s1 == 'C')
        s = 12 + s;
    else if (s1 == 'D')
        s = 13 + s;
    else if (s1 == 'E')
        s = 14 + s;
    else if (s1 == 'F')
        s = 15 + s;
    else
        s = (s1 - '0') + s;

    s = 16 * s;

    if (s2 == 'a')
        s = 10 + s;
    else if (s2 == 'b')
        s = 11 + s;
    else if (s2 == 'c')
        s = 12 + s;
    else if (s2 == 'd')
        s = 13 + s;
    else if (s2 == 'e')
        s = 14 + s;
    else if (s2 == 'f')
        s = 15 + s;
    else if (s2 == 'A')
        s = 10 + s;
    else if (s2 == 'B')
        s = 11 + s;
    else if (s2 == 'C')
        s = 12 + s;
    else if (s2 == 'D')
        s = 13 + s;
    else if (s2 == 'E')
        s = 14 + s;
    else if (s2 == 'F')
        s = 15 + s;
    else
        s = (s2 - '0') + s;

    return char(s);
}

int LexToken(int b_enum, string token)
{
    BaseTypePtr result = nullptr;

    switch (b_enum){
    case NUM:
        result.reset(new NumberType(token));
        break;
    case MULT_DIV:
        result.reset(new StringType(token));
        break;
    case INEQUALITY:
        result.reset(new StringType(token));
        break;
    case ID:
        result.reset(new StringType(token));
        break;
    case STRING:
        result.reset(new StringType(token));
        break;
    case EQUAL_UNEQUAL:
        result.reset(new StringType(token));
        break;
    case PLUS_MINUS:
        result.reset(new StringType(token));
        break;
    default:
        break;
    }

    yylval = result;
    return b_enum;
}

#endif // HW3_SCANNER_H
