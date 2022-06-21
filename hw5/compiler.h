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

class Compiler
{
public:
  SymbolTable sym_tab;
  Validations validations;
  Generator code_genreation;
  const std::string GEN_NEXT = "_next";
  const std::string GEN_IF = "_if";
  const std::string GEN_ELSE = "_else";
  const std::string GEN_OR = "_or";
  const std::string GEN_WHILE_HEAD = "_while_head";
  const std::string GEN_WHILE_BODY = "_while_body";
  const std::string GEN_AND = "_and";

  Compiler();

  void pCheckBool(const BaseTypePtr &bool_exp, int ln);

  static BaseTypePtr pRetType(BaseTypePtr type, int ln);
  static CTypePtr pRetType(int ln);
  void handleErrorMainMissing();

  void pAddWhileScope(const BaseTypePtr &while_head_label, int ln);
  void pRemoveScope(int ln);
  static ArgListTypePtr pFs(int ln);
  static ArgListTypePtr pFs(const BaseTypePtr &formals, int ln);
  static CTypePtr pByte(int ln);

  StatementTypePtr pAutoTypeAssignment(BaseTypePtr &type, BaseTypePtr &id, BaseTypePtr &exp, int ln);
  StatementTypePtr pStatAssign(const BaseTypePtr &id, const BaseTypePtr &exp, int ln);
  static CTypePtr pBool(int ln);
  void handleErrorDef(int lineno, const string &id);
  void handleErrorUndefFunc(int lineno, const string &id);
  void handleErrorMismatch(int lineno);

  static BaseTypePtr pParen(BaseTypePtr exp, int ln);
  static NumberTypePtr pNum(const BaseTypePtr &num, int ln);
  void handleErrorPrototypeMismatch(int lineno, const string &id, vector<string> &argTypes);

  static Compiler &instance();
  static ArgListTypePtr pFsList(const BaseTypePtr &formal, int ln);
  static ArgListTypePtr pFsList(const BaseTypePtr &formal, const BaseTypePtr &formals_list, int ln);

  SymbolTypePtr pFDecl(const BaseTypePtr &type, const BaseTypePtr &id, int ln);

  StatementTypePtr pStatRet(int ln);
  StatementTypePtr pStatRetExp(const BaseTypePtr &exp, int ln);
  void handleErrorUnexpectedBreak(int lineno);
  void handleErrorUnexpectedContinue(int lineno);
  StatementTypePtr pStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                           const BaseTypePtr &if_list_as_statement, int ln);
  StatementTypePtr pStatIfElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                               BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
                               const BaseTypePtr &else_statement, int ln);
  RegisterTypePtr pBinop(const BaseTypePtr &exp1, BaseTypePtr &binop, const BaseTypePtr &exp2, int ln);
  RegisterTypePtr pString(const BaseTypePtr &stype_string, int ln);

  BoolExpTypePtr pTrue(int ln);

  void handleErrorUndef(int lineno, const string &id);
  BoolExpTypePtr pFalse(int ln);
  StatementTypePtr pStatType(const BaseTypePtr &type, const BaseTypePtr &id, int ln);
  void pFuncDecl(const BaseTypePtr &statements, const BaseTypePtr &next_label, int ln);
  void pFuncHead(const BaseTypePtr &ret_type, const BaseTypePtr &id, const BaseTypePtr &formals, int ln);
  void handleErrorLex(int lineno);
  StatementTypePtr pStatTypeAssign(const BaseTypePtr &type, const BaseTypePtr &id, const BaseTypePtr &exp, int ln);
  void handleErrorSyn(int lineno);
  StatementTypePtr pStatWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
                              const BaseTypePtr &exp,
                              const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                              const BaseTypePtr &end_list_as_statement, int ln);
  StatementTypePtr pStatBreak(int ln);
  void pProgram(int ln);
  void pFuncs(int ln);
  StatementTypePtr pStatContinue(int ln);

  StringTypePtr pGenerateIfL(int ln);

  StringTypePtr pGenerateAndL(int ln);

  StatementTypePtr pStat(const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
                         const BaseTypePtr &next_label,
                         const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement, int ln);
  StringTypePtr pGenerateWhileBodyL(int ln);

  BaseTypePtr pAuto(int ln);
  BaseTypePtr pCall(const BaseTypePtr &id, const BaseTypePtr &exp_list, int ln);
  BaseTypePtr pCall(const BaseTypePtr &id, int ln);
  BaseTypePtr pID(const BaseTypePtr &id, int ln);

  StringTypePtr pGenerateOrL(int ln);
  BoolExpTypePtr pNot(const BaseTypePtr &bool_exp, int ln);
  BoolExpTypePtr pAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2, int ln);
  BoolExpTypePtr pOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2, int ln);
  StringTypePtr pGenerateNextL(int ln);
  StringTypePtr pGenerateElseL(int ln);
  static StatementTypePtr pStat(BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement, int ln);
  static StatementTypePtr pStatOfStats(BaseTypePtr &statements, int ln);

  static BaseTypePtr pExplist(const BaseTypePtr &exp, const BaseTypePtr &exp_list, int ln);
  static CTypePtr pInt(int ln);

  StringTypePtr pGenerateWhileHeadL(int ln);
  StatementTypePtr pBNext(int ln);
  void handleErrorByteTooLarge(int lineno, const string &value);
  void pAddStatScope(int ln);

  BaseTypePtr pCallExp(BaseTypePtr call_exp, int ln);
  BaseTypePtr pNumB(BaseTypePtr num, int ln);
  StatementTypePtr pBWhileH(int ln);
  static BaseTypePtr pExplist(const BaseTypePtr &exp, int ln);
  StatementTypePtr pBIfNext(int ln);
  static StatementTypePtr pStatCall(int ln);

  BaseTypePtr pConvBool(BaseTypePtr exp, int ln);

  BoolExpTypePtr pRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2, int ln);
};

#endif // HW3_PARSE_UTILS_H