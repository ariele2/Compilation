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

    StatementTypePtr addStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                               const BaseTypePtr &if_list_as_statement);
    BoolExpTypePtr addTrue();

    BoolExpTypePtr addFalse();

    void emitPrinti();
    StatementTypePtr addStatType(string id);
    void emitErrorDivByZero();

    static BoolExpTypePtr addNot(const BaseTypePtr &bool_exp);

    StatementTypePtr addStatIfAndElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                      BaseTypePtr if_list_as_statement,
                                      const BaseTypePtr &else_label, const BaseTypePtr &else_statement);

    static StatementTypePtr addStatCall();

    void emitPrint();

    void emitPrintf();

    StatementTypePtr addStatAssign(string id, const BaseTypePtr &exp);

    StatementTypePtr addStatWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label, const BaseTypePtr &exp,
                                  const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                                  const BaseTypePtr &end_list_as_statement, const br_list_pointer &break_list);

    StatementTypePtr addStatBreak();

    explicit Generator(Validations &semantic_ref);

    name_of_register GenerateReg();

    BaseTypePtr addIdentification(const SymbolTypePtr &symbol);

    name_of_register GenerateGlobalReg();

    void addGlobalFuncs();

    StatementTypePtr addStatRet();

    StatementTypePtr addStatRetExpression(const BaseTypePtr &exp);

    void addFunctionHead(const FuncSymbolTypePtr &symbol);

    RegisterTypePtr addString(const BaseTypePtr &stype_string);
    RegisterTypePtr addCall(const FuncSymbolTypePtr &func);

    BoolExpTypePtr addAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr addOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2);
    void addFuncDeclaration(const BaseTypePtr &statements, const BaseTypePtr &next_label);

    BoolExpTypePtr addRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2);

    void emitExit();

    StatementTypePtr addStatContinue();

    RegisterTypePtr addCall(const FuncSymbolTypePtr &func, const ExpListTypePtr &exp_list);
    StatementTypePtr addBWhileHead();

    void addProg();

    string findNoBoolExpStr(const BaseTypePtr &exp);

    BaseTypePtr addCallExpression(BaseTypePtr call_exp);

    RegisterTypePtr addBinop(const BaseTypePtr &exp1, string binop, const BaseTypePtr &exp2);
    void addCheckDivZero(const BaseTypePtr &exp);

    static string findTypeOfLLVM(const Ty &type);
    StatementTypePtr addBranchNext();

    void addStoreReg(int offset, const name_of_register &reg_to_store);

    RegisterTypePtr addLoadReg(int offset, Ty type);

    StatementTypePtr addPBIfNext();

    void addBoolExpToReg(const BaseTypePtr &exp, const name_of_register &reg_result);

    BaseTypePtr regToBooleanExpression(string &reg_source);
};

#endif // HW5_GENERATOR_H
