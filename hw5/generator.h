#ifndef HW5_GENERATOR_H
#define HW5_GENERATOR_H

#include "types.h"
#include "bp.hpp"
#include "validations.h"

const int MAX_VARIABLES_PER_FUNCTION = 50;
const int VARIABLE_SIZE = 4;
const int STACK_SIZE = MAX_VARIABLES_PER_FUNCTION * VARIABLE_SIZE;

class Generator
{
public:
    string stack_register;

    Buff buff;
    int num_of_regs;
    Validations validator_ref;

    StatementTypePtr addStatIf(const BaseTypePtr &expression, const BaseTypePtr &if_l, const BaseTypePtr &if_stat,
                               const BaseTypePtr &if_list_state);
    BoolExpTypePtr addTrue();

    BoolExpTypePtr addFalse();

    void emitPrinti();
    StatementTypePtr addStatType(string id);
    void emitErrorDivByZero();

    static BoolExpTypePtr addNot(const BaseTypePtr &bool_exp);

    StatementTypePtr addStatIfAndElse(const BaseTypePtr &expression, const BaseTypePtr &if_l, const BaseTypePtr &if_stat,
                                      BaseTypePtr if_list_state,
                                      const BaseTypePtr &else_l, const BaseTypePtr &else_stat);

    static StatementTypePtr addStatCall();

    void emitPrint();

    void emitPrintf();

    StatementTypePtr addStatAssign(string id, const BaseTypePtr &expression);

    StatementTypePtr addStatWhile(BaseTypePtr start_list_stat, const BaseTypePtr &head_label, const BaseTypePtr &expression,
                                  const BaseTypePtr &body_l, const BaseTypePtr &while_stat,
                                  const BaseTypePtr &end_list_stat, const br_list_pointer &break_l);

    StatementTypePtr addStatBreak();

    explicit Generator(Validations &semantic_refernce);

    name_of_register GenerateReg();

    BaseTypePtr addIdentification(const SymbolTypePtr &sym);

    name_of_register GenerateGlobalReg();

    void addGlobalFuncs();

    StatementTypePtr addStatRet();

    StatementTypePtr addStatRetExpression(const BaseTypePtr &expression);

    void addFunctionHead(const FuncSymbolTypePtr &sym);

    RegisterTypePtr addString(const BaseTypePtr &stype_str);
    RegisterTypePtr addCall(const FuncSymbolTypePtr &func);

    BoolExpTypePtr addAnd(const BaseTypePtr &bool_exp_first, const BaseTypePtr &and_l, const BaseTypePtr &bool_exp_second);

    BoolExpTypePtr addOr(const BaseTypePtr &bool_exp_first, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp_second);
    void addFuncDeclaration(const BaseTypePtr &stats, const BaseTypePtr &next_l);

    BoolExpTypePtr addRelOp(const BaseTypePtr &expression_first, BaseTypePtr &relop, const BaseTypePtr &expression_second);

    void emitExit();

    StatementTypePtr addStatContinue();

    RegisterTypePtr addCall(const FuncSymbolTypePtr &func, const ExpListTypePtr &expression_list);
    StatementTypePtr addBWhileHead();

    void addProg();

    string findNoBoolExpStr(const BaseTypePtr &expression);

    BaseTypePtr addCallExpression(BaseTypePtr call_expression);

    RegisterTypePtr addBinop(const BaseTypePtr &expression_first, string binop, const BaseTypePtr &expression_second);
    void addCheckDivZero(const BaseTypePtr &expression);

    static string findTypeOfLLVM(const Ty &t);
    StatementTypePtr addBranchNext();

    void addStoreReg(int offs, const name_of_register &reg_to_store);

    RegisterTypePtr addLoadReg(int offs, Ty t);

    StatementTypePtr addPBIfNext();

    void addBoolExpToReg(const BaseTypePtr &expression, const name_of_register &register_result);

    BaseTypePtr regToBooleanExpression(string &register_source);
};

#endif // HW5_GENERATOR_H
