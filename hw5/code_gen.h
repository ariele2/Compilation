#ifndef HW5_CODE_GEN_H
#define HW5_CODE_GEN_H

#include "typedefs.h"
#include "bp.hpp"
#include "semantics.h"


const int MAX_VARIABLES_PER_FUNCTION = 50;
const int VARIABLE_SIZE = 4;
const int STACK_SIZE = MAX_VARIABLES_PER_FUNCTION * VARIABLE_SIZE;

class Generator {
public:
    string stack_register;
    Validations validator_ref;
    int num_of_regs;
    Buff buff;

    void emitPrintf();
    void emitExit();
    void emitPrint();
    void emitPrinti();
    void emitErrorDivByZero();
    

    explicit Generator(Validations &semantic_ref);

    name_of_register GenerateReg();

    name_of_register GenerateGlobalReg();

    void addGlobalFuncs();

    RegisterTypePtr addBinop(const BaseTypePtr &exp1, string binop, const BaseTypePtr &exp2);

    void addCheckDivZero(const BaseTypePtr &exp);

    void addFunctionHead(const FuncSymbolTypePtr &symbol);

    void addFuncDeclaration(const BaseTypePtr &statements, const BaseTypePtr &next_label);

    StatementTypePtr addStatType(string id);

    StatementTypePtr addStatAssign(string id, const BaseTypePtr &exp);

    static StatementTypePtr addStatCall();

    StatementTypePtr addStatRet();

    StatementTypePtr addStatRetExpression(const BaseTypePtr &exp);

    StatementTypePtr
    addStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                    const BaseTypePtr &if_list_as_statement);

    StatementTypePtr
    addStatIfAndElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                        BaseTypePtr if_list_as_statement,
                        const BaseTypePtr &else_label, const BaseTypePtr &else_statement);

    StatementTypePtr
    addStatWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label, const BaseTypePtr &exp,
                       const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                       const BaseTypePtr &end_list_as_statement, const br_list_pointer &break_list);

    StatementTypePtr addStatBreak();

    StatementTypePtr addStatContinue();

    
    

    RegisterTypePtr addCall(const FuncSymbolTypePtr &func, const ExpListTypePtr &exp_list);

    RegisterTypePtr addCall(const FuncSymbolTypePtr &func);

    BoolExpTypePtr addTrue();

    BoolExpTypePtr addFalse();

    static BoolExpTypePtr addNot(const BaseTypePtr &bool_exp);

    BoolExpTypePtr addAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr addOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr addRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2);

    void addProg();

    string findNoBoolExpStr(const BaseTypePtr &exp);

    static string findTypeOfLLVM(const Ty &type);

    void addStoreReg(int offset, const name_of_register &reg_to_store);

    RegisterTypePtr addLoadReg(int offset, Ty type);

    StatementTypePtr addBranchNext();

    RegisterTypePtr addString(const BaseTypePtr &stype_string);

    BaseTypePtr addIdentification(const SymbolTypePtr &symbol);

    void addBoolExpToReg(const BaseTypePtr &exp, const name_of_register &reg_result);

    BaseTypePtr regToBooleanExpression(string &reg_source);

    StatementTypePtr addBWhileHead();

    StatementTypePtr addPBIfNext();

    BaseTypePtr addCallExpression(BaseTypePtr call_exp);
};


#endif //HW5_CODE_GEN_H
