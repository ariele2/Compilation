#ifndef HW3_PARSE_UTILS_H
#define HW3_PARSE_UTILS_H

#include "types.h"
#include "sym_table.h"
#include "hw3_output.hpp"
#include "semantics.h"

using namespace output;

void yyerror(const char *);

class Utils
{
public:
    SymbolTable symbol_table;
    SemanticChecks semantic_checks;

    Utils();
    // rm - changed names of funcs and types and order
    static Utils &instance();
    void pCheckBool(int ln, Tptr bool_exp);
    void pPopScope(int ln);
    void pAddWhileScope(int ln);
    void pAddStateScope(int ln);
    void parseStateRet(int ln);
    void parseStatemeWhile(int ln){};
    void pProgram(int ln);
    void parseStateOfState(int ln){};
    void parseState(int ln){};
    void parseStateType(int ln, Tptr type, Tptr id);
    void parseStateContinue(int ln);
    void parseStateTypeAssignment(int ln, Tptr type, Tptr id, Tptr exp);
    void parseStateAssignment(int ln, Tptr id, Tptr exp);
    void parseStateBreak(int ln);
    void pFunctions(int ln){};
    void parseStateRetExpression(int ln, Tptr exp);
    void parseStateCallFunc(int ln){};
    void parseStateElseIF(int ln){};
    void parseStateIf(int ln){};
    void pFunctionDeclaration(int ln){};
    Tptr pReturnType(int ln, Tptr type);
    SymListPtr pFormals(int ln, Tptr formals);
    TypePtr pReturnType(int ln);
    Tptr pOr(int ln, Tptr bool_exp1, Tptr bool_exp2);
    BoolTypePtr pRelOprator(int ln, Tptr exp1, Tptr exp2);
    SymListPtr pFormList(int ln, Tptr formal, Tptr formals_list);
    SymbolPtr ParseFormalDecl(int ln, Tptr type, Tptr id);
    Tptr pCallExpression(int ln, Tptr call_exp);
    Tptr pParen(int ln, Tptr exp);
    Tptr pBinaryOp(int ln, Tptr exp1, Tptr exp2);
    BoolTypePtr pFalse(int ln);
    TypePtr pByte(int ln);
    TypePtr pBool(int ln);
    TypePtr pAuto(int ln);
    Tptr pCast(int ln, Tptr type, Tptr exp);
    Tptr pAnd(int ln, Tptr bool_exp1, Tptr bool_exp2);
    StringTypePtr pString(int ln, Tptr stype_string);
    NumberTypePtr pNum(int ln, Tptr n);
    Tptr pNumB(int ln, Tptr n);
    Tptr pNot(int ln, Tptr bool_exp);
    SymListPtr pFormList(int ln, Tptr formal);
    SymListPtr pFormals(int ln);
    Tptr parseFunctionDef(int ln, Tptr ret_type, Tptr id, Tptr formals);
    Tptr pCall(int ln, Tptr id, Tptr exp_list);
    Tptr pCall(int ln, Tptr id);
    Tptr pExpressionList(int ln, Tptr exp);
    Tptr pExpressionList(int ln, Tptr exp, Tptr exp_list);
    TypePtr pInt(int ln);
    BoolTypePtr pTrue(int ln);
    Tptr pId(int ln, Tptr id);

    // TODO - add auto
};

#endif // HW3_PARSE_UTILS_H