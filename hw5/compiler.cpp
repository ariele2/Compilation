#include "compiler.h"

#include <memory>

Compiler::Compiler() : symbol_table(), semantic_checks(symbol_table), code_gen(semantic_checks) {

}


Compiler &Compiler::instance() {
    static Compiler compiler;
    return compiler;
}

void Compiler::pProgram(int lineno) {
    if (!semantic_checks.CheckMainIsDefined()) {
        errorMainMissing();
        exit(0);
    }

    symbol_table.PopScope();
    code_gen.EmitProgram();
}

void Compiler::pFuncs(int lineno) {
    // nop
}

void Compiler::pFuncHead(int lineno, const BaseTypePtr &ret_type, const BaseTypePtr &id, const BaseTypePtr &formals) {
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);
    auto dynamic_cast_ret_type = dynamic_pointer_cast<CType>(ret_type);
    auto dynamic_cast_formals = dynamic_pointer_cast<ArgListType>(formals);

    if (symbol_table.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto function_symbol = make_shared<FuncSymType>(dynamic_cast_id->token, dynamic_cast_ret_type->g_type,
                                                            dynamic_cast_formals->arg_list);
    symbol_table.AddFunction(function_symbol);
    symbol_table.PushFunctionScope(dynamic_cast_ret_type->g_type);
    for (const auto &param:function_symbol->params) {
        auto param_symbol = make_shared<SymbolType>(param);
        if (semantic_checks.CheckSymDefined(param_symbol->name)) {
            errorDef(lineno, param_symbol->name);
            exit(0);
        }

        symbol_table.AddParam(param_symbol);
    }

    code_gen.EmitFuncHead(function_symbol);


}

void Compiler::pFuncDecl(int lineno, const BaseTypePtr &statements, const BaseTypePtr &next_label) {
    code_gen.EmitFuncDecl(statements, next_label);
}

BaseTypePtr Compiler::pRetType(int lineno, BaseTypePtr type) {
    return type;
}

CTypePtr Compiler::pRetType(int lineno) {
    auto type_pointer = make_shared<CType>(VOID_TYPE);
    return type_pointer;
}

ArgListTypePtr Compiler::pFs(int lineno) {
    auto formals_pointer = make_shared<ArgListType>();
    return formals_pointer;
}

ArgListTypePtr Compiler::pFs(int lineno, const BaseTypePtr &formals) {
    auto dynamic_cast_formals = dynamic_pointer_cast<ArgListType>(formals);

    auto formals_pointer = make_shared<ArgListType>();
    for (auto symbol_iter = dynamic_cast_formals->arg_list.rbegin();
         symbol_iter != dynamic_cast_formals->arg_list.rend(); symbol_iter++) {

        formals_pointer->arg_list.push_back(*symbol_iter);
    }
    return formals_pointer;
}

ArgListTypePtr Compiler::pFsList(int lineno, const BaseTypePtr &formal) {
    auto dynamic_cast_formal = dynamic_pointer_cast<SymbolType>(formal);

    auto arg_list_pointer = make_shared<ArgListType>();
    arg_list_pointer->arg_list.push_back(*dynamic_cast_formal);
    return arg_list_pointer;
}

ArgListTypePtr Compiler::pFsList(int lineno, const BaseTypePtr &formal, const BaseTypePtr &formals_list) {
    auto dynamic_cast_formals_list = dynamic_pointer_cast<ArgListType>(formals_list);
    auto dynamic_cast_formal = dynamic_pointer_cast<SymbolType>(formal);

    dynamic_cast_formals_list->arg_list.push_back(*dynamic_cast_formal);
    return dynamic_cast_formals_list;
}

SymbolTypePtr Compiler::pFDecl(int lineno, const BaseTypePtr &type, const BaseTypePtr &id) {
    auto dynamic_cast_type = dynamic_pointer_cast<CType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);

    auto symbol_pointer = make_shared<SymbolType>(dynamic_cast_id->token, symbol_table.scope_stack.top()->offset,
                                                   dynamic_cast_type->g_type);
    return symbol_pointer;
}

StatementTypePtr
Compiler::pStat(int lineno, BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement) {
    auto dynamic_cast_statement = dynamic_pointer_cast<StatementType>(statement);
    auto dynamic_cast_next_list = dynamic_pointer_cast<StatementType>(next_list_as_statement);

    dynamic_cast_statement->next_list = CodeBuffer::merge(dynamic_cast_statement->next_list,
                                                          dynamic_cast_next_list->next_list);

    return dynamic_cast_statement;
}

StatementTypePtr
Compiler::pStat(int lineno, const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
                          const BaseTypePtr &next_label,
                          const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement) {
    auto dynamic_cast_statements = dynamic_pointer_cast<StatementType>(statements);
    auto dynamic_cast_old_next_list = dynamic_pointer_cast<StatementType>(old_next_list_as_statement);
    auto dynamic_cast_next_label = dynamic_pointer_cast<StringType>(next_label);
    auto dynamic_cast_next_statement = dynamic_pointer_cast<StatementType>(next_statement);
    auto dynamic_cast_my_next_list = dynamic_pointer_cast<StatementType>(my_next_list_as_statement);

    // backpatch last statement
    code_gen.code_buffer.bpatch(dynamic_cast_statements->next_list, dynamic_cast_next_label->token);
    code_gen.code_buffer.bpatch(dynamic_cast_old_next_list->next_list, dynamic_cast_next_label->token);

    // get last statement
    dynamic_cast_next_statement->next_list = CodeBuffer::merge(dynamic_cast_next_statement->next_list,
                                                               dynamic_cast_my_next_list->next_list);

    return dynamic_cast_next_statement;
}


StatementTypePtr
Compiler::pStatOfStats(int lineno, BaseTypePtr &statements) {
    auto dynamic_cast_statements = dynamic_pointer_cast<StatementType>(statements);
    return dynamic_cast_statements;
}

StatementTypePtr Compiler::pStatType(int lineno, const BaseTypePtr &type, const BaseTypePtr &id) {
    auto dynamic_cast_type = dynamic_pointer_cast<CType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);

    if (semantic_checks.CheckSymDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    const auto symbol = make_shared<SymbolType>(dynamic_cast_id->token, 0, dynamic_cast_type->g_type);
    symbol_table.AddVariable(symbol);

    return code_gen.EmitStatementType(dynamic_cast_id->token);
}

StatementTypePtr
Compiler::pStatTypeAssign(int lineno, const BaseTypePtr &type, const BaseTypePtr &id, const BaseTypePtr &exp) {
    auto dynamic_cast_type = dynamic_pointer_cast<CType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);

    if (semantic_checks.CheckFunction(exp->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckAssigned(dynamic_cast_type->g_type, exp->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }
    if (semantic_checks.CheckSymDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    const auto symbol = make_shared<SymbolType>(dynamic_cast_id->token, 0, dynamic_cast_type->g_type);
    symbol_table.AddVariable(symbol);


    return code_gen.EmitStatementAssign(dynamic_cast_id->token, exp);
}

StatementTypePtr Compiler::pStatAssign(int lineno, const BaseTypePtr &id, const BaseTypePtr &exp) {
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);

    if (!semantic_checks.CheckSymDefined(dynamic_cast_id->token)) {
        errorUndef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (semantic_checks.CheckFunction(exp->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckFunction(symbol_from_id->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(symbol_from_id);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckAssigned(symbol_from_id->g_type, exp->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitStatementAssign(dynamic_cast_id->token, exp);
}

StatementTypePtr Compiler::pStatCall(int lineno) {
    return CodeGen::EmitStatementCall();
}

StatementTypePtr Compiler::pStatRet(int lineno) {
    if (!semantic_checks.CheckReturn(VOID_TYPE)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitStatementReturn();
}

StatementTypePtr Compiler::pStatRetExp(int lineno, const BaseTypePtr &exp) {
    if (semantic_checks.CheckFunction(exp->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if ((!semantic_checks.CheckReturn(exp->g_type)) || (semantic_checks.CheckVoid(exp->g_type))) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitStatementReturnExp(exp);
}

StatementTypePtr
Compiler::pStatIf(int lineno, const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                           const BaseTypePtr &if_list_as_statement) {
    return code_gen.EmitStatementIf(exp, if_label, if_statement, if_list_as_statement);
}

StatementTypePtr
Compiler::pStatIfElse(int lineno, const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                               BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
                               const BaseTypePtr &else_statement) {
    return code_gen.EmitStatementIfElse(exp, if_label, if_statement, move(if_list_as_statement), else_label,
                                        else_statement);
}

StatementTypePtr
Compiler::pStatWhile(int lineno, BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
                              const BaseTypePtr &exp,
                              const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                              const BaseTypePtr &end_list_as_statement) {
    // pop scope
    assert(symbol_table.scope_stack.top()->scope_type == WHILE_SCOPE);
    auto break_list = symbol_table.scope_stack.top()->break_list;
    symbol_table.PopScope();

    // emit
    return code_gen.EmitStatementWhile(move(start_list_as_statement), while_head_label, exp, while_body_label,
                                       while_statement,
                                       end_list_as_statement,
                                       break_list);
}

StatementTypePtr
Compiler::ParseStatementSwitch(int lineno, BaseTypePtr exp, BaseTypePtr switch_list_as_statement, BaseTypePtr case_list) {
    // pop scope
    assert(symbol_table.scope_stack.top()->scope_type == SWITCH_SCOPE);
    auto break_list = symbol_table.scope_stack.top()->break_list;
    symbol_table.PopScope();

    return code_gen.EmitStatementSwitch(move(exp), move(switch_list_as_statement),
                                        move(case_list), break_list);
}

StatementTypePtr Compiler::pStatBreak(int lineno) {
    if (!semantic_checks.CheckBreak()) {
        errorUnexpectedBreak(lineno);
        exit(0);
    }

    return code_gen.EmitStatementBreak();
}

StatementTypePtr Compiler::pStatContinue(int lineno) {
    if (!semantic_checks.CheckContinue()) {
        errorUnexpectedContinue(lineno);
        exit(0);
    }

    return code_gen.EmitStatementContinue();
}

BaseTypePtr Compiler::pCall(int lineno, const BaseTypePtr &id, const BaseTypePtr &exp_list) {
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);

    if (!semantic_checks.CheckSymDefined(dynamic_cast_id->token)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (!semantic_checks.CheckFunction(symbol_from_id->g_type)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(symbol_from_id);
    auto dynamic_cast_exp_list = dynamic_pointer_cast<TExpList>(exp_list);

    if (!semantic_checks.CheckCall(dynamic_cast_func, dynamic_cast_exp_list)) {
        vector<string> expected_args;
        for (const auto &symbol:dynamic_cast_func->params) {
            expected_args.push_back(TypeToString(symbol.g_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    return code_gen.EmitCall(dynamic_cast_func, dynamic_cast_exp_list);
}

BaseTypePtr Compiler::pCall(int lineno, const BaseTypePtr &id) {
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(id);

    if (!semantic_checks.CheckSymDefined(dynamic_cast_id->token)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (!semantic_checks.CheckFunction(symbol_from_id->g_type)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(symbol_from_id);
    auto empty_exp_list = make_shared<TExpList>();

    if (!semantic_checks.CheckCall(dynamic_cast_func, empty_exp_list)) {
        vector<string> expected_args;
        for (const auto &symbol:dynamic_cast_func->params) {
            expected_args.push_back(TypeToString(symbol.g_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    return code_gen.EmitCall(dynamic_cast_func);
}

BaseTypePtr Compiler::pExplist(int lineno, const BaseTypePtr &exp) {
    auto exp_list_pointer = make_shared<TExpList>();

    exp_list_pointer->exp_list.insert(exp_list_pointer->exp_list.begin(), exp);
    return exp_list_pointer;
}

BaseTypePtr Compiler::pExplist(int lineno, const BaseTypePtr &exp, const BaseTypePtr &exp_list) {
    auto dynamic_cast_exp_list = dynamic_pointer_cast<TExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.insert(dynamic_cast_exp_list->exp_list.begin(), exp);
    return dynamic_cast_exp_list;
}

CTypePtr Compiler::pInt(int lineno) {
    return make_shared<CType>(INT_TYPE);
}

CTypePtr Compiler::pByte(int lineno) {
    return make_shared<CType>(BYTE_TYPE);
}

CTypePtr Compiler::pBool(int lineno) {
    return make_shared<CType>(BOOL_TYPE);
}

BaseTypePtr Compiler::pParen(int lineno, BaseTypePtr exp) {
    return exp;
}

RegisterTypePtr Compiler::pBinop(int lineno, const BaseTypePtr &exp1, BaseTypePtr &binop, const BaseTypePtr &exp2) {
    if (semantic_checks.CheckFunction(exp1->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckFunction(exp2->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckAndGetBinOpType(exp1->g_type, exp2->g_type) == ERROR_TYPE) {
        errorMismatch(lineno);
        exit(0);
    }

    // return register type
    auto dynamic_cast_binop = dynamic_pointer_cast<StringType>(binop);
    return code_gen.EmitBinop(exp1, dynamic_cast_binop->token, exp2);
}

BaseTypePtr Compiler::pID(int lineno, const BaseTypePtr &id) {
    auto dynamic_cast_string = dynamic_pointer_cast<StringType>(id);

    if (!symbol_table.IsSymbolDefined(dynamic_cast_string->token)) {
        errorUndef(lineno, dynamic_cast_string->token);
        exit(0);
    }

    auto id_pointer = symbol_table.GetDefinedSymbol(dynamic_cast_string->token);
    return code_gen.EmitID(id_pointer);
}

BaseTypePtr Compiler::pCallExp(int lineno, BaseTypePtr call_exp) {
    return code_gen.EmitCallExp(move(call_exp));
}

NumberTypePtr Compiler::pNum(int lineno, const BaseTypePtr &num) {
    auto dynamic_cast_num = dynamic_pointer_cast<NumberType>(num);

    return dynamic_cast_num;
}

BaseTypePtr Compiler::pNumB(int lineno, BaseTypePtr num) {
    auto dynamic_cast_num = dynamic_pointer_cast<NumberType>(num);

    if (!semantic_checks.CheckOFByte(dynamic_cast_num->token)) {
        errorByteTooLarge(lineno, to_string(dynamic_cast_num->token));
        exit(0);
    }
    num->g_type = BYTE_TYPE;
    return num;
}

RegisterTypePtr Compiler::pString(int lineno, const BaseTypePtr &stype_string) {

    return code_gen.EmitString(stype_string);
}

BoolExpTypePtr Compiler::pTrue(int lineno) {
    return code_gen.EmitTrue();
}

BoolExpTypePtr Compiler::pFalse(int lineno) {
    return code_gen.EmitFalse();
}

BoolExpTypePtr Compiler::pNot(int lineno, const BaseTypePtr &bool_exp) {
    // exp can be a bool literal or an id
    if (semantic_checks.CheckFunction(bool_exp->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(bool_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckBool(bool_exp->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return CodeGen::EmitNot(bool_exp);
}

BoolExpTypePtr
Compiler::pAnd(int lineno, const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2) {
    if (semantic_checks.CheckFunction(bool_exp1->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(bool_exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckFunction(bool_exp2->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(bool_exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckBool(bool_exp1->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    if (!semantic_checks.CheckBool(bool_exp2->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitAnd(bool_exp1, and_label, bool_exp2);
}

BoolExpTypePtr
Compiler::pOr(int lineno, const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2) {
    if (semantic_checks.CheckFunction(bool_exp1->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(bool_exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckFunction(bool_exp2->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(bool_exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckBool(bool_exp1->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    if (!semantic_checks.CheckBool(bool_exp2->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitOr(bool_exp1, or_label, bool_exp2);
}

BoolExpTypePtr Compiler::pRelOp(int lineno, const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2) {
    if (semantic_checks.CheckFunction(exp1->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckFunction(exp2->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckRelop(exp1->g_type, exp2->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitRelOp(exp1, relop, exp2);
}

CaseListTypePtr Compiler::pCaseList(int lineno, BaseTypePtr case_decl, BaseTypePtr next_list, BaseTypePtr case_list) {
    return code_gen.EmitCaseList(move(case_decl), move(next_list), move(case_list));
}

CaseListTypePtr Compiler::pCaseList(int lineno, BaseTypePtr case_decl) {
    return code_gen.EmitCaseList(move(case_decl));
}

CaseListTypePtr
Compiler::ParseCaseDefault(int lineno, BaseTypePtr list_as_statement, BaseTypePtr default_label, BaseTypePtr statements) {
    return code_gen.EmitCaseDefault(move(list_as_statement), move(default_label), move(statements));
}

CaseDeclTypePtr Compiler::ParseCaseDecl(int lineno, BaseTypePtr num, BaseTypePtr list_as_statement, BaseTypePtr case_decl_label,
                                         BaseTypePtr statements) {
    return code_gen.EmitCaseDecl(move(num), move(list_as_statement), move(case_decl_label), move(statements));
}

void Compiler::pAddStatScope(int lineno) {
    symbol_table.PushScope(STATEMENT_SCOPE);
}

void Compiler::pAddWhileScope(int lineno, const BaseTypePtr &while_head_label) {
    auto dynamic_cast_while_head_label = dynamic_pointer_cast<StringType>(while_head_label);
    auto break_list = make_shared<branch_list>();

    symbol_table.PushScope(WHILE_SCOPE);
    symbol_table.scope_stack.top()->while_continue_label = dynamic_cast_while_head_label->token;
    symbol_table.scope_stack.top()->break_list = break_list;
    symbol_table.scope_stack.top()->inside_while = true;
}

void Compiler::ParsePushSwitchScope(int lineno, const BaseTypePtr &switch_head_label) {
    auto dynamic_cast_switch_head_label = dynamic_pointer_cast<StringType>(switch_head_label);
    auto break_list = make_shared<branch_list>();

    symbol_table.PushScope(SWITCH_SCOPE);
    symbol_table.scope_stack.top()->break_list = break_list;
    symbol_table.scope_stack.top()->inside_switch = true;
}

void Compiler::pRemoveScope(int lineno) {
    symbol_table.PopScope();
}

void Compiler::ParseCheckSwitchExp(int lineno, const BaseTypePtr &num_exp) {
    if (semantic_checks.CheckFunction(num_exp->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(num_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckAssigned(INT_TYPE, num_exp->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }
}

void Compiler::pCheckBool(int lineno, const BaseTypePtr &bool_exp) {
    if (semantic_checks.CheckFunction(bool_exp->g_type)) {
        auto cast_function = dynamic_pointer_cast<FuncSymType>(bool_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.CheckBool(bool_exp->g_type)) {
        errorMismatch(lineno);
        exit(0);
    }
}

StringTypePtr Compiler::pGenerateIfL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_if");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateElseL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_else");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateWhileBodyL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_while_body");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateWhileHeadL(int lineno) {
    // note: `PushWhileScope` uses this return value, so we don't have to modify the scopes here
    auto label_name = code_gen.code_buffer.genLabel("_while_head");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateAndL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_and");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateOrL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_or");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateNextL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_next");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateDefaultL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_default");
    return make_shared<StringType>(label_name);
}

StringTypePtr Compiler::pGenerateCaseDeclL(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_case");
    return make_shared<StringType>(label_name);
}

StatementTypePtr Compiler::pBNext(int lineno) {
    // in llvm we must branch before every label
    return code_gen.EmitBranchNext();
}

StatementTypePtr Compiler::pBWhileH(int lineno) {
    return code_gen.EmitBranchWhileHead();
}

StatementTypePtr Compiler::pBIfNext(int lineno) {
    return code_gen.EmitParseBranchIfNext();
}

StatementTypePtr Compiler::ParseBranchSwitchHead(int lineno) {
    return code_gen.EmitBranchSwitchHead();
}


StatementTypePtr Compiler::ParseBranchCaseHead(int lineno) {
    return code_gen.EmitBranchCaseHead();
}

StatementTypePtr Compiler::ParseBranchDefaultHead(int lineno) {
    return code_gen.EmitBranchDefaultHead();
}

BaseTypePtr Compiler::pConvBool(int lineno, BaseTypePtr exp) {
    if (exp->g_type == BOOL_TYPE) {
        auto dynamic_cast_symbol = dynamic_pointer_cast<SymbolType>(exp);
        auto reg_for_bool = code_gen.GenRegister();
        auto result_reg_ptr = make_shared<RegisterType>(reg_for_bool, BOOL_TYPE);

        if (dynamic_cast_symbol) {
            result_reg_ptr = code_gen.EmitLoadRegister(dynamic_cast_symbol->offset,
                                                       dynamic_cast_symbol->g_type);
        } else {
            code_gen.EmitBoolExpToRegister(exp, reg_for_bool);
        }

        return result_reg_ptr;

    } else {
        return exp;
    }
}
