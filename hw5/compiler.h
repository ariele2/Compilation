#ifndef HW3_PARSE_UTILS_H
#define HW3_PARSE_UTILS_H

#include "types.h"
#include "sym_table.h"
#include "hw3_output.hpp"
#include "validations.h"
#include "generator.h"
#include "bp.hpp"

using namespace output;

void yyerror(const char *);


class Compiler {
public:
    SymbolTable sym_tab;
    Validations validations;
    Generator code_genreation;
    const std::string GEN_IF ="_if";
    const std::string GEN_ELSE ="_else";
    const std::string GEN_WHILE_BODY ="_while_body";
    const std::string GEN_WHILE_HEAD ="_while_head";
    const std::string GEN_AND ="_and";
    const std::string GEN_OR ="_or";
    const std::string GEN_NEXT ="_next";


    Compiler();
    void pProgram(int ln);
    void pFuncs(int ln);
    void pFuncDecl(const BaseTypePtr &statements, const BaseTypePtr &next_label, int ln);
    void pFuncHead(const BaseTypePtr &ret_type, const BaseTypePtr &id, const BaseTypePtr &formals, int ln);
    static BaseTypePtr pRetType(BaseTypePtr type, int ln);
    static CTypePtr pRetType(int ln);
    static ArgListTypePtr pFs(int ln);
    static ArgListTypePtr pFs(const BaseTypePtr &formals, int ln);
    static ArgListTypePtr pFsList(const BaseTypePtr &formal, int ln);
    static ArgListTypePtr pFsList(const BaseTypePtr &formal, const BaseTypePtr &formals_list, int ln);
    SymbolTypePtr pFDecl(const BaseTypePtr &type, const BaseTypePtr &id, int ln);
    static StatementTypePtr pStat(BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement, int ln);
    StatementTypePtr pStat(const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
      const BaseTypePtr &next_label,
      const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement, int ln);
    static StatementTypePtr pStatOfStats(BaseTypePtr &statements, int ln);
    StatementTypePtr pStatType(const BaseTypePtr &type, const BaseTypePtr &id, int ln);
    StatementTypePtr pStatTypeAssign(const BaseTypePtr &type, const BaseTypePtr &id, const BaseTypePtr &exp, int ln);
    StatementTypePtr pAutoTypeAssignment(BaseTypePtr &type, BaseTypePtr &id, BaseTypePtr &exp, int ln);
    StatementTypePtr pStatAssign(const BaseTypePtr &id, const BaseTypePtr &exp, int ln);
    static StatementTypePtr pStatCall(int ln);
    StatementTypePtr pStatRet(int ln);
    StatementTypePtr pStatRetExp(const BaseTypePtr &exp, int ln);
    StatementTypePtr pStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
       const BaseTypePtr &if_list_as_statement, int ln);
    StatementTypePtr pStatIfElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
           BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
           const BaseTypePtr &else_statement, int ln);
    StatementTypePtr
    pStatWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
          const BaseTypePtr &exp,
          const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
          const BaseTypePtr &end_list_as_statement, int ln);
    StatementTypePtr pStatBreak(int ln);
    StatementTypePtr pStatContinue(int ln);

    BaseTypePtr pCall(const BaseTypePtr &id, const BaseTypePtr &exp_list, int ln);
    BaseTypePtr pCall(const BaseTypePtr &id, int ln);
    static BaseTypePtr pExplist(const BaseTypePtr &exp, int ln);
    static BaseTypePtr pExplist(const BaseTypePtr &exp, const BaseTypePtr &exp_list, int ln);
    static CTypePtr pInt(int ln);
    static CTypePtr pByte(int ln);
    static CTypePtr pBool(int ln);
    static BaseTypePtr pParen(BaseTypePtr exp, int ln);
    RegisterTypePtr pBinop(const BaseTypePtr &exp1, BaseTypePtr &binop, const BaseTypePtr &exp2, int ln);
    BaseTypePtr pID(const BaseTypePtr &id, int ln);
    BaseTypePtr pCallExp(BaseTypePtr call_exp, int ln);
    static NumberTypePtr pNum(const BaseTypePtr &num, int ln);
    BaseTypePtr pNumB(BaseTypePtr num, int ln);
    RegisterTypePtr pString(const BaseTypePtr &stype_string, int ln);
    BoolExpTypePtr pTrue(int ln);
    BoolExpTypePtr pFalse(int ln);
    BoolExpTypePtr pNot(const BaseTypePtr &bool_exp, int ln);
    BoolExpTypePtr pAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2, int ln);
    BoolExpTypePtr pOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2, int ln);
    BoolExpTypePtr pRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2, int ln);
    BaseTypePtr pAuto(int ln);
    StringTypePtr pGenerateIfL(int ln);
    StringTypePtr pGenerateElseL(int ln);
    StringTypePtr pGenerateWhileHeadL(int ln);
    StringTypePtr pGenerateWhileBodyL(int ln);
    StringTypePtr pGenerateAndL(int ln);
    StringTypePtr pGenerateOrL(int ln);
    StringTypePtr pGenerateNextL(int ln);
   
    void pAddStatScope(int ln);
    void pAddWhileScope(const BaseTypePtr &while_head_label, int ln);
    
    void pRemoveScope(int ln);
    
    void pCheckBool(const BaseTypePtr &bool_exp, int ln);
    StatementTypePtr pBNext(int ln);
    StatementTypePtr pBWhileH(int ln);
    StatementTypePtr pBIfNext(int ln);
   
    BaseTypePtr pConvBool(BaseTypePtr exp, int ln);
    static Compiler &instance();

    void handleErrorLex(int lineno);
    void handleErrorSyn(int lineno);
    void handleErrorUndef(int lineno, const string& id);
    void handleErrorDef(int lineno, const string& id);
    void handleErrorUndefFunc(int lineno, const string& id);
    void handleErrorMismatch(int lineno);
    void handleErrorPrototypeMismatch(int lineno, const string& id, vector<string>& argTypes);
    void handleErrorUnexpectedBreak(int lineno);
    void handleErrorUnexpectedContinue(int lineno);
    void handleErrorMainMissing();
    void handleErrorByteTooLarge(int lineno, const string& value);

};


#endif //HW3_PARSE_UTILS_H