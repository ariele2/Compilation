#ifndef HW5_CODE_GEN_H
#define HW5_CODE_GEN_H

#include "typedefs.h"
#include "bp.hpp"
#include "semantics.h"

const int MAX_VARIABLES_PER_FUNCTION = 50;
const int VARIABLE_SIZE = 4;
const int STACK_SIZE = MAX_VARIABLES_PER_FUNCTION * VARIABLE_SIZE;

class CodeGen {
public:
    int register_count;
    CodeBuffer code_buffer;
    SemanticChecks semantic_ref;
    string stack_register;

    explicit CodeGen(SemanticChecks &semantic_ref);

    register_name GenRegister();

    register_name GenGlobalRegister();

    void EmitGlobalFunctions();

    RegisterTypePtr EmitBinop(const BaseTypePtr &exp1, string binop, const BaseTypePtr &exp2);

    void EmitCheckDivZero(const BaseTypePtr &exp);

    void EmitFuncHead(const FuncSymbolTypePtr &symbol);

    void EmitFuncDecl(const BaseTypePtr &statements, const BaseTypePtr &next_label);

    StatementTypePtr EmitStatementType(string id);

    StatementTypePtr EmitStatementAssign(string id, const BaseTypePtr &exp);

    static StatementTypePtr EmitStatementCall();

    StatementTypePtr EmitStatementReturn();

    StatementTypePtr EmitStatementReturnExp(const BaseTypePtr &exp);

    StatementTypePtr
    EmitStatementIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                    const BaseTypePtr &if_list_as_statement);

    StatementTypePtr
    EmitStatementIfElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                        BaseTypePtr if_list_as_statement,
                        const BaseTypePtr &else_label, const BaseTypePtr &else_statement);

    StatementTypePtr
    EmitStatementWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label, const BaseTypePtr &exp,
                       const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                       const BaseTypePtr &end_list_as_statement, const branch_list_ptr &break_list);

    StatementTypePtr EmitStatementBreak();

    StatementTypePtr EmitStatementContinue();

    StatementTypePtr
    EmitStatementSwitch(BaseTypePtr exp, BaseTypePtr switch_list_as_statement, BaseTypePtr case_list,
                        branch_list_ptr break_list);

    RegisterTypePtr EmitCall(const FuncSymbolTypePtr &func, const ExpListTypePtr &exp_list);

    RegisterTypePtr EmitCall(const FuncSymbolTypePtr &func);

    BoolExpTypePtr EmitTrue();

    BoolExpTypePtr EmitFalse();

    static BoolExpTypePtr EmitNot(const BaseTypePtr &bool_exp);

    BoolExpTypePtr EmitAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr EmitOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr EmitRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2);

    CaseDeclTypePtr
    EmitCaseDecl(BaseTypePtr num, BaseTypePtr list_as_statement, BaseTypePtr case_decl_label, BaseTypePtr statements);

    CaseListTypePtr EmitCaseDefault(BaseTypePtr list_as_statement, BaseTypePtr default_label, BaseTypePtr statements);

    CaseListTypePtr EmitCaseList(BaseTypePtr case_decl);

    CaseListTypePtr EmitCaseList(BaseTypePtr case_decl, BaseTypePtr next_label, BaseTypePtr case_list);

    void EmitProgram();

    string GetNonBoolExpString(const BaseTypePtr &exp);

    static string GetLLVMType(const Type &type);

    void EmitStoreRegister(int offset, const register_name &reg_to_store);

    RegisterTypePtr EmitLoadRegister(int offset, Type type);

    StatementTypePtr EmitBranchNext();

    RegisterTypePtr EmitString(const BaseTypePtr &stype_string);

    BaseTypePtr EmitID(const SymbolTypePtr &symbol);

    void EmitBoolExpToRegister(const BaseTypePtr &exp, const register_name &reg_result);

    BaseTypePtr RegisterToBoolExp(string &reg_source);

    StatementTypePtr EmitBranchWhileHead();

    StatementTypePtr EmitParseBranchIfNext();

    StatementTypePtr EmitBranchSwitchHead();

    StatementTypePtr EmitBranchCaseHead();

    StatementTypePtr EmitBranchDefaultHead();

    BaseTypePtr EmitCallExp(BaseTypePtr call_exp);
};


#endif //HW5_CODE_GEN_H
