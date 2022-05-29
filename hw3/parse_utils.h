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
    checkSemantics semantic_checks;

    Utils();
    static Utils &instance();
    void pCheckBool( Tptr bool_exp, int ln);
    void pPopScope(int ln);
    void pAddWhileScope(int ln);
    void pAddStateScope(int ln);
    void parseStateRet(int ln);
    void parseStatemeWhile(int ln){};
    void pProgram(int ln);
    void parseStateOfState(int ln){};
    void parseState(int ln){};
    void parseStateType(  Tptr type, Tptr id, int ln);
    void parseStateContinue(int ln);
    void parseStateTypeAssignment(  Tptr type, Tptr id, Tptr exp, int ln);
    void parseAutoTypeAssignment(Tptr type, Tptr id, Tptr exp, int ln);
    void parseStateAssignment(  Tptr id, Tptr exp, int ln);
    void parseStateBreak(int ln);
    void pFunctions(int ln){};
    void parseStateRetExpression(  Tptr exp, int ln);
    void parseStateCallFunc(int ln){};
    void parseStateElseIF(int ln){};
    void parseStateIf(int ln){};
    void pFunctionDeclaration(int ln){};
    Tptr pReturnType(  Tptr type, int ln);
    SymListPtr pFormals(  Tptr formals, int ln);
    TypePtr pReturnType(int ln);
    Tptr pOr(  Tptr bool_exp1, Tptr bool_exp2, int ln);
    BoolTypePtr pRelOprator(  Tptr exp1, Tptr exp2, int ln);
    SymListPtr pFormList(  Tptr formal, Tptr formals_list, int ln);
    SymbolPtr ParseFormalDecl(  Tptr type, Tptr id, int ln);
    Tptr pCallExpression(  Tptr call_exp, int ln);
    Tptr pParen(  Tptr exp, int ln);
    Tptr pBinaryOp(  Tptr exp1, Tptr exp2, int ln);
    BoolTypePtr pFalse(int ln);
    TypePtr pByte(int ln);
    TypePtr pBool(int ln);
    TypePtr pAuto(int ln);
    Tptr pCast(  Tptr type, Tptr exp, int ln);
    Tptr pAnd(  Tptr bool_exp1, Tptr bool_exp2, int ln);
    StringTypePtr pString(  Tptr stype_string, int ln);
    NumberTypePtr pNum(  Tptr n, int ln);
    Tptr pNumB(  Tptr n, int ln);
    Tptr pNot(  Tptr bool_exp, int ln);
    SymListPtr pFormList(  Tptr formal, int ln);
    SymListPtr pFormals(int ln);
    Tptr parseFunctionDef( Tptr ret_type, Tptr id, Tptr formals, int ln);
    Tptr pCall(  Tptr id, Tptr exp_list, int ln);
    Tptr pCall(  Tptr id, int ln);
    Tptr pExpressionList(  Tptr exp, int ln);
    Tptr pExpressionList(  Tptr exp, Tptr exp_list, int ln);
    TypePtr pInt(int ln);
    BoolTypePtr pTrue(int ln);
    Tptr pId(  Tptr id, int ln);
    void handleErrorDoesNotMatch( int ln);
    
};

#endif // HW3_PARSE_UTILS_H