#ifndef HW3_PARSE_UTILS_H
#define HW3_PARSE_UTILS_H

#include "typedefs.h"
#include "symbol_table.h"
#include "hw3_output.hpp"
#include "semantics.h"
#include "code_gen.h"
#include "bp.hpp"

using namespace output;

void yyerror(const char *);

// note: maybe it's wiser to create a `Parser` class which is stored here and move the functions there.
// it would require a lot of new functions though.

// note: should've removed all the ln args and used the global instead.
class Compiler {
public:
    SymbolTable symbol_table;
    SemanticChecks semantic_checks;
    CodeGen code_gen;

    Compiler();
    void pProgram(int ln);
    void pFuncs(int ln);
    void pFuncDecl(int ln, const BaseTypePtr &statements, const BaseTypePtr &next_label);
    void pFuncHead(int ln, const BaseTypePtr &ret_type, const BaseTypePtr &id, const BaseTypePtr &formals);
    static BaseTypePtr pRetType(int ln, BaseTypePtr type);
    static CTypePtr pRetType(int ln);
    static ArgListTypePtr pFs(int ln);
    static ArgListTypePtr pFs(int ln, const BaseTypePtr &formals);
    static ArgListTypePtr pFsList(int ln, const BaseTypePtr &formal);
    static ArgListTypePtr pFsList(int ln, const BaseTypePtr &formal, const BaseTypePtr &formals_list);
    SymbolTypePtr pFDecl(int ln, const BaseTypePtr &type, const BaseTypePtr &id);
    static StatementTypePtr pStat(int ln, BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement);
    StatementTypePtr pStat(int ln, const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
                    const BaseTypePtr &next_label,
                    const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement);
    static StatementTypePtr pStatOfStats(int ln, BaseTypePtr &statements);
    StatementTypePtr pStatType(int ln, const BaseTypePtr &type, const BaseTypePtr &id);
    StatementTypePtr pStatTypeAssign(int ln, const BaseTypePtr &type, const BaseTypePtr &id, const BaseTypePtr &exp);
    StatementTypePtr pStatAssign(int ln, const BaseTypePtr &id, const BaseTypePtr &exp);
    static StatementTypePtr pStatCall(int ln);
    StatementTypePtr pStatRet(int ln);
    StatementTypePtr pStatRetExp(int ln, const BaseTypePtr &exp);
    StatementTypePtr pStatIf(int ln, const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                     const BaseTypePtr &if_list_as_statement);
    StatementTypePtr pStatIfElse(int ln, const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                         BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
                         const BaseTypePtr &else_statement);
    StatementTypePtr
    pStatWhile(int ln, BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
                        const BaseTypePtr &exp,
                        const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                        const BaseTypePtr &end_list_as_statement);
    StatementTypePtr pStatBreak(int ln);
    StatementTypePtr pStatContinue(int ln);
    // remove
    StatementTypePtr
    ParseStatementSwitch(int ln, BaseTypePtr exp, BaseTypePtr switch_list_as_statement, BaseTypePtr case_list);
    BaseTypePtr pCall(int ln, const BaseTypePtr &id, const BaseTypePtr &exp_list);
    BaseTypePtr pCall(int ln, const BaseTypePtr &id);
    static BaseTypePtr pExplist(int ln, const BaseTypePtr &exp);
    static BaseTypePtr pExplist(int ln, const BaseTypePtr &exp, const BaseTypePtr &exp_list);
    static CTypePtr pInt(int ln);
    static CTypePtr pByte(int ln);
    static CTypePtr pBool(int ln);
    static BaseTypePtr pParen(int ln, BaseTypePtr exp);
    RegisterTypePtr pBinop(int ln, const BaseTypePtr &exp1, BaseTypePtr &binop, const BaseTypePtr &exp2);
    BaseTypePtr pID(int ln, const BaseTypePtr &id);
    BaseTypePtr pCallExp(int ln, BaseTypePtr call_exp);
    static NumberTypePtr pNum(int ln, const BaseTypePtr &num);
    BaseTypePtr pNumB(int ln, BaseTypePtr num);
    RegisterTypePtr pString(int ln, const BaseTypePtr &stype_string);
    BoolExpTypePtr pTrue(int ln);
    BoolExpTypePtr pFalse(int ln);
    BoolExpTypePtr pNot(int ln, const BaseTypePtr &bool_exp);
    BoolExpTypePtr pAnd(int ln, const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2);
    BoolExpTypePtr pOr(int ln, const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2);
    BoolExpTypePtr pRelOp(int ln, const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2);
    StringTypePtr pGenerateIfL(int ln);
    StringTypePtr pGenerateElseL(int ln);
    StringTypePtr pGenerateWhileHeadL(int ln);
    StringTypePtr pGenerateWhileBodyL(int ln);
    StringTypePtr pGenerateAndL(int ln);
    StringTypePtr pGenerateOrL(int ln);
    StringTypePtr pGenerateNextL(int ln);
    //remove from here
    StringTypePtr pGenerateDefaultL(int ln);
    StringTypePtr pGenerateCaseDeclL(int ln);
    CaseListTypePtr pCaseList(int ln, BaseTypePtr case_decl, BaseTypePtr next_list, BaseTypePtr case_list);
    CaseListTypePtr pCaseList(int ln, BaseTypePtr case_decl);
    CaseListTypePtr
    ParseCaseDefault(int ln, BaseTypePtr list_as_statement, BaseTypePtr default_label, BaseTypePtr statements);
    CaseDeclTypePtr ParseCaseDecl(int ln, BaseTypePtr num, BaseTypePtr list_as_statement, BaseTypePtr case_decl_label,
                                   BaseTypePtr statements);
     // until here
    void pAddStatScope(int ln);
    void pAddWhileScope(int ln, const BaseTypePtr &while_head_label);
    //remove
    void ParsePushSwitchScope(int ln, const BaseTypePtr &switch_head_label);
    void pRemoveScope(int ln);
    //remove
    void ParseCheckSwitchExp(int ln, const BaseTypePtr &num_exp);
    void pCheckBool(int ln, const BaseTypePtr &bool_exp);
    StatementTypePtr pBNext(int ln);
    StatementTypePtr pBWhileH(int ln);
    StatementTypePtr pBIfNext(int ln);
    //remove from here
    StatementTypePtr ParseBranchSwitchHead(int ln);
    StatementTypePtr ParseBranchCaseHead(int ln);
    StatementTypePtr ParseBranchDefaultHead(int ln);
    // until here
    BaseTypePtr pConvBool(int ln, BaseTypePtr exp);
    static Compiler &instance();

    void handleErrorDoesNotMatch(int ln);
    void handleErrorIsUndefined(int ln, string s);
    void handleErrorInDefintion(int ln, string s);
    void handleErrorUndefinedFunction(int ln, string s);
    void handleErrorInLexical(int ln);
    void handleErrorInSyntax(int ln);
    void handleErrorBreak(int ln);
    void handleErrorInContinue(int ln);
    void handleErrorInMissingMain(int ln);
    void handleErrorByteIsTooBig(int ln, string s);
    void handleErrorPrototypeMismatch(int ln, const string &id, std::vector<string> &argTypes);

};


#endif //HW3_PARSE_UTILS_H