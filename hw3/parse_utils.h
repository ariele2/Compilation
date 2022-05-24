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

    void ParseProgram(int lineno);
    void ParseFuncs(int lineno){};
    void ParseFuncDecl(int lineno) {};

    Tptr ParseFuncHead(int lineno, Tptr ret_type, Tptr id, Tptr formals);
    Tptr ParseRetType(int lineno, Tptr type);

    STypeCTypePtr ParseRetType(int lineno);
    SimpleSymbolListPtr ParseFormals(int lineno);
    SimpleSymbolListPtr ParseFormals(int lineno, Tptr formals);
    SimpleSymbolListPtr ParseFormalsList(int lineno, Tptr formal);
    SimpleSymbolListPtr ParseFormalsList(int lineno, Tptr formal, Tptr formals_list);
    SimpleSymbolPtr ParseFormalDecl(int lineno, Tptr type, Tptr id);

    void ParseStatements(int lineno){};
    void ParseStatementOfStatements(int lineno) {};
    void ParseStatementType(int lineno, Tptr type, Tptr id);
    void ParseStatementTypeAssign(int lineno, Tptr type, Tptr id, Tptr exp);
    void ParseStatementAssign(int lineno, Tptr id, Tptr exp);
    void ParseStatementCall(int lineno) {};
    void ParseStatementReturn(int lineno);
    void ParseStatementReturnExp(int lineno, Tptr exp);
    void ParseStatementIf(int lineno) {};
    void ParseStatementIfElse(int lineno) {};
    void ParseStatementWhile(int lineno) {};
    void ParseStatementBreak(int lineno);
    void ParseStatementContinue(int lineno);

    Tptr ParseCall(int lineno, Tptr id, Tptr exp_list);
    Tptr ParseCall(int lineno, Tptr id);
    Tptr ParseExplist(int lineno, Tptr exp);
    Tptr ParseExplist(int lineno, Tptr exp, Tptr exp_list);

    STypeCTypePtr ParseInt(int lineno);
    STypeCTypePtr ParseByte(int lineno);
    STypeCTypePtr ParseBool(int lineno);

    Tptr ParseParentheses(int lineno, Tptr exp);
    Tptr ParseBinop(int lineno, Tptr exp1, Tptr exp2);
    Tptr ParseID(int lineno, Tptr id);
    Tptr ParseCallExp(int lineno, Tptr call_exp);

    STypeNumberPtr ParseNum(int lineno, Tptr num);

    Tptr ParseNumB(int lineno, Tptr num);

    STypeStringPtr ParseString(int lineno, Tptr stype_string);

    STypeBoolPtr ParseTrue(int lineno);
    STypeBoolPtr ParseFalse(int lineno);

    Tptr ParseNot(int lineno, Tptr bool_exp);
    Tptr ParseAnd(int lineno, Tptr bool_exp1, Tptr bool_exp2);
    Tptr ParseOr(int lineno, Tptr bool_exp1, Tptr bool_exp2);

    STypeBoolPtr ParseRelOp(int lineno, Tptr exp1, Tptr exp2);

    Tptr ParseCast(int lineno, Tptr type, Tptr exp);

    void ParsePushStatementScope(int lineno);
    void ParsePushWhileScope(int lineno);
    void ParsePopScope(int lineno);
    void ParseCheckBool(int lineno, Tptr bool_exp);

    static Utils &instance();

    // TODO - add auto
};

#endif // HW3_PARSE_UTILS_H