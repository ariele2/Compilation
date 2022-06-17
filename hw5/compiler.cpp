#include "compiler.h"

#include <memory>

Compiler::Compiler() : symbol_table(), semantic_checks(symbol_table), code_gen(semantic_checks) {

}


Compiler &Compiler::instance() {
    static Compiler compiler;
    return compiler;
}

void Compiler::ParseProgram(int lineno) {
    if (!semantic_checks.IsMainDefined()) {
        errorMainMissing();
        exit(0);
    }

    symbol_table.PopScope();
    code_gen.EmitProgram();
}

void Compiler::ParseFuncs(int lineno) {
    // nop
}

void Compiler::ParseFuncHead(int lineno, const STypePtr &ret_type, const STypePtr &id, const STypePtr &formals) {
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);
    auto dynamic_cast_ret_type = dynamic_pointer_cast<STypeCType>(ret_type);
    auto dynamic_cast_formals = dynamic_pointer_cast<STypeArgList>(formals);

    if (symbol_table.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto function_symbol = make_shared<STypeFunctionSymbol>(dynamic_cast_id->token, dynamic_cast_ret_type->general_type,
                                                            dynamic_cast_formals->arg_list);
    symbol_table.AddFunction(function_symbol);
    symbol_table.PushFunctionScope(dynamic_cast_ret_type->general_type);
    for (const auto &param:function_symbol->parameters) {
        auto param_symbol = make_shared<STypeSymbol>(param);
        if (semantic_checks.IsSymbolDefined(param_symbol->name)) {
            errorDef(lineno, param_symbol->name);
            exit(0);
        }

        symbol_table.AddParam(param_symbol);
    }

    code_gen.EmitFuncHead(function_symbol);


}

void Compiler::ParseFuncDecl(int lineno, const STypePtr &statements, const STypePtr &next_label) {
    code_gen.EmitFuncDecl(statements, next_label);
}

STypePtr Compiler::ParseRetType(int lineno, STypePtr type) {
    return type;
}

STypeCTypePtr Compiler::ParseRetType(int lineno) {
    auto type_pointer = make_shared<STypeCType>(VOID_TYPE);
    return type_pointer;
}

STypeArgListPtr Compiler::ParseFormals(int lineno) {
    auto formals_pointer = make_shared<STypeArgList>();
    return formals_pointer;
}

STypeArgListPtr Compiler::ParseFormals(int lineno, const STypePtr &formals) {
    auto dynamic_cast_formals = dynamic_pointer_cast<STypeArgList>(formals);

    auto formals_pointer = make_shared<STypeArgList>();
    for (auto symbol_iter = dynamic_cast_formals->arg_list.rbegin();
         symbol_iter != dynamic_cast_formals->arg_list.rend(); symbol_iter++) {

        formals_pointer->arg_list.push_back(*symbol_iter);
    }
    return formals_pointer;
}

STypeArgListPtr Compiler::ParseFormalsList(int lineno, const STypePtr &formal) {
    auto dynamic_cast_formal = dynamic_pointer_cast<STypeSymbol>(formal);

    auto arg_list_pointer = make_shared<STypeArgList>();
    arg_list_pointer->arg_list.push_back(*dynamic_cast_formal);
    return arg_list_pointer;
}

STypeArgListPtr Compiler::ParseFormalsList(int lineno, const STypePtr &formal, const STypePtr &formals_list) {
    auto dynamic_cast_formals_list = dynamic_pointer_cast<STypeArgList>(formals_list);
    auto dynamic_cast_formal = dynamic_pointer_cast<STypeSymbol>(formal);

    dynamic_cast_formals_list->arg_list.push_back(*dynamic_cast_formal);
    return dynamic_cast_formals_list;
}

STypeSymbolPtr Compiler::ParseFormalDecl(int lineno, const STypePtr &type, const STypePtr &id) {
    auto dynamic_cast_type = dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    auto symbol_pointer = make_shared<STypeSymbol>(dynamic_cast_id->token, symbol_table.scope_stack.top()->offset,
                                                   dynamic_cast_type->general_type);
    return symbol_pointer;
}

STypeStatementPtr
Compiler::ParseStatements(int lineno, STypePtr &statement, const STypePtr &next_list_as_statement) {
    auto dynamic_cast_statement = dynamic_pointer_cast<STypeStatement>(statement);
    auto dynamic_cast_next_list = dynamic_pointer_cast<STypeStatement>(next_list_as_statement);

    dynamic_cast_statement->next_list = CodeBuffer::merge(dynamic_cast_statement->next_list,
                                                          dynamic_cast_next_list->next_list);

    return dynamic_cast_statement;
}

STypeStatementPtr
Compiler::ParseStatements(int lineno, const STypePtr &statements, const STypePtr &old_next_list_as_statement,
                          const STypePtr &next_label,
                          const STypePtr &next_statement, const STypePtr &my_next_list_as_statement) {
    auto dynamic_cast_statements = dynamic_pointer_cast<STypeStatement>(statements);
    auto dynamic_cast_old_next_list = dynamic_pointer_cast<STypeStatement>(old_next_list_as_statement);
    auto dynamic_cast_next_label = dynamic_pointer_cast<STypeString>(next_label);
    auto dynamic_cast_next_statement = dynamic_pointer_cast<STypeStatement>(next_statement);
    auto dynamic_cast_my_next_list = dynamic_pointer_cast<STypeStatement>(my_next_list_as_statement);

    // backpatch last statement
    code_gen.code_buffer.bpatch(dynamic_cast_statements->next_list, dynamic_cast_next_label->token);
    code_gen.code_buffer.bpatch(dynamic_cast_old_next_list->next_list, dynamic_cast_next_label->token);

    // get last statement
    dynamic_cast_next_statement->next_list = CodeBuffer::merge(dynamic_cast_next_statement->next_list,
                                                               dynamic_cast_my_next_list->next_list);

    return dynamic_cast_next_statement;
}


STypeStatementPtr
Compiler::ParseStatementOfStatements(int lineno, STypePtr &statements) {
    auto dynamic_cast_statements = dynamic_pointer_cast<STypeStatement>(statements);
    return dynamic_cast_statements;
}

STypeStatementPtr Compiler::ParseStatementType(int lineno, const STypePtr &type, const STypePtr &id) {
    auto dynamic_cast_type = dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    if (semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    const auto symbol = make_shared<STypeSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    symbol_table.AddVariable(symbol);

    return code_gen.EmitStatementType(dynamic_cast_id->token);
}

STypeStatementPtr
Compiler::ParseStatementTypeAssign(int lineno, const STypePtr &type, const STypePtr &id, const STypePtr &exp) {
    auto dynamic_cast_type = dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsLegalAssignTypes(dynamic_cast_type->general_type, exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }
    if (semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    const auto symbol = make_shared<STypeSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    symbol_table.AddVariable(symbol);


    return code_gen.EmitStatementAssign(dynamic_cast_id->token, exp);
}

STypeStatementPtr Compiler::ParseStatementAssign(int lineno, const STypePtr &id, const STypePtr &exp) {
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorUndef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(symbol_from_id->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsLegalAssignTypes(symbol_from_id->general_type, exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitStatementAssign(dynamic_cast_id->token, exp);
}

STypeStatementPtr Compiler::ParseStatementCall(int lineno) {
    return CodeGen::EmitStatementCall();
}

STypeStatementPtr Compiler::ParseStatementReturn(int lineno) {
    if (!semantic_checks.IsLegalReturnType(VOID_TYPE)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitStatementReturn();
}

STypeStatementPtr Compiler::ParseStatementReturnExp(int lineno, const STypePtr &exp) {
    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if ((!semantic_checks.IsLegalReturnType(exp->general_type)) || (semantic_checks.IsVoidType(exp->general_type))) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitStatementReturnExp(exp);
}

STypeStatementPtr
Compiler::ParseStatementIf(int lineno, const STypePtr &exp, const STypePtr &if_label, const STypePtr &if_statement,
                           const STypePtr &if_list_as_statement) {
    return code_gen.EmitStatementIf(exp, if_label, if_statement, if_list_as_statement);
}

STypeStatementPtr
Compiler::ParseStatementIfElse(int lineno, const STypePtr &exp, const STypePtr &if_label, const STypePtr &if_statement,
                               STypePtr if_list_as_statement, const STypePtr &else_label,
                               const STypePtr &else_statement) {
    return code_gen.EmitStatementIfElse(exp, if_label, if_statement, move(if_list_as_statement), else_label,
                                        else_statement);
}

STypeStatementPtr
Compiler::ParseStatementWhile(int lineno, STypePtr start_list_as_statement, const STypePtr &while_head_label,
                              const STypePtr &exp,
                              const STypePtr &while_body_label, const STypePtr &while_statement,
                              const STypePtr &end_list_as_statement) {
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

STypeStatementPtr
Compiler::ParseStatementSwitch(int lineno, STypePtr exp, STypePtr switch_list_as_statement, STypePtr case_list) {
    // pop scope
    assert(symbol_table.scope_stack.top()->scope_type == SWITCH_SCOPE);
    auto break_list = symbol_table.scope_stack.top()->break_list;
    symbol_table.PopScope();

    return code_gen.EmitStatementSwitch(move(exp), move(switch_list_as_statement),
                                        move(case_list), break_list);
}

STypeStatementPtr Compiler::ParseStatementBreak(int lineno) {
    if (!semantic_checks.IsLegalBreak()) {
        errorUnexpectedBreak(lineno);
        exit(0);
    }

    return code_gen.EmitStatementBreak();
}

STypeStatementPtr Compiler::ParseStatementContinue(int lineno) {
    if (!semantic_checks.IsLegalContinue()) {
        errorUnexpectedContinue(lineno);
        exit(0);
    }

    return code_gen.EmitStatementContinue();
}

STypePtr Compiler::ParseCall(int lineno, const STypePtr &id, const STypePtr &exp_list) {
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (!semantic_checks.IsFunctionType(symbol_from_id->general_type)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto dynamic_cast_exp_list = dynamic_pointer_cast<STypeExpList>(exp_list);

    if (!semantic_checks.IsLegalCallTypes(dynamic_cast_func, dynamic_cast_exp_list)) {
        vector<string> expected_args;
        for (const auto &symbol:dynamic_cast_func->parameters) {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    return code_gen.EmitCall(dynamic_cast_func, dynamic_cast_exp_list);
}

STypePtr Compiler::ParseCall(int lineno, const STypePtr &id) {
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (!semantic_checks.IsFunctionType(symbol_from_id->general_type)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto empty_exp_list = make_shared<STypeExpList>();

    if (!semantic_checks.IsLegalCallTypes(dynamic_cast_func, empty_exp_list)) {
        vector<string> expected_args;
        for (const auto &symbol:dynamic_cast_func->parameters) {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    return code_gen.EmitCall(dynamic_cast_func);
}

STypePtr Compiler::ParseExplist(int lineno, const STypePtr &exp) {
    auto exp_list_pointer = make_shared<STypeExpList>();

    exp_list_pointer->exp_list.insert(exp_list_pointer->exp_list.begin(), exp);
    return exp_list_pointer;
}

STypePtr Compiler::ParseExplist(int lineno, const STypePtr &exp, const STypePtr &exp_list) {
    auto dynamic_cast_exp_list = dynamic_pointer_cast<STypeExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.insert(dynamic_cast_exp_list->exp_list.begin(), exp);
    return dynamic_cast_exp_list;
}

STypeCTypePtr Compiler::ParseInt(int lineno) {
    return make_shared<STypeCType>(INT_TYPE);
}

STypeCTypePtr Compiler::ParseByte(int lineno) {
    return make_shared<STypeCType>(BYTE_TYPE);
}

STypeCTypePtr Compiler::ParseBool(int lineno) {
    return make_shared<STypeCType>(BOOL_TYPE);
}

STypePtr Compiler::ParseParentheses(int lineno, STypePtr exp) {
    return exp;
}

STypeRegisterPtr Compiler::ParseBinop(int lineno, const STypePtr &exp1, STypePtr &binop, const STypePtr &exp2) {
    if (semantic_checks.IsFunctionType(exp1->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(exp2->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckAndGetBinOpType(exp1->general_type, exp2->general_type) == ERROR_TYPE) {
        errorMismatch(lineno);
        exit(0);
    }

    // return register type
    auto dynamic_cast_binop = dynamic_pointer_cast<STypeString>(binop);
    return code_gen.EmitBinop(exp1, dynamic_cast_binop->token, exp2);
}

STypePtr Compiler::ParseID(int lineno, const STypePtr &id) {
    auto dynamic_cast_string = dynamic_pointer_cast<STypeString>(id);

    if (!symbol_table.IsSymbolDefined(dynamic_cast_string->token)) {
        errorUndef(lineno, dynamic_cast_string->token);
        exit(0);
    }

    auto id_pointer = symbol_table.GetDefinedSymbol(dynamic_cast_string->token);
    return code_gen.EmitID(id_pointer);
}

STypePtr Compiler::ParseCallExp(int lineno, STypePtr call_exp) {
    return code_gen.EmitCallExp(move(call_exp));
}

STypeNumberPtr Compiler::ParseNum(int lineno, const STypePtr &num) {
    auto dynamic_cast_num = dynamic_pointer_cast<STypeNumber>(num);

    return dynamic_cast_num;
}

STypePtr Compiler::ParseNumB(int lineno, STypePtr num) {
    auto dynamic_cast_num = dynamic_pointer_cast<STypeNumber>(num);

    if (!semantic_checks.IsByteOverflow(dynamic_cast_num->token)) {
        errorByteTooLarge(lineno, to_string(dynamic_cast_num->token));
        exit(0);
    }
    num->general_type = BYTE_TYPE;
    return num;
}

STypeRegisterPtr Compiler::ParseString(int lineno, const STypePtr &stype_string) {

    return code_gen.EmitString(stype_string);
}

STypeBoolExpPtr Compiler::ParseTrue(int lineno) {
    return code_gen.EmitTrue();
}

STypeBoolExpPtr Compiler::ParseFalse(int lineno) {
    return code_gen.EmitFalse();
}

STypeBoolExpPtr Compiler::ParseNot(int lineno, const STypePtr &bool_exp) {
    // exp can be a bool literal or an id
    if (semantic_checks.IsFunctionType(bool_exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return CodeGen::EmitNot(bool_exp);
}

STypeBoolExpPtr
Compiler::ParseAnd(int lineno, const STypePtr &bool_exp1, const STypePtr &and_label, const STypePtr &bool_exp2) {
    if (semantic_checks.IsFunctionType(bool_exp1->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(bool_exp2->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp1->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp2->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitAnd(bool_exp1, and_label, bool_exp2);
}

STypeBoolExpPtr
Compiler::ParseOr(int lineno, const STypePtr &bool_exp1, const STypePtr &or_label, const STypePtr &bool_exp2) {
    if (semantic_checks.IsFunctionType(bool_exp1->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(bool_exp2->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp1->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp2->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitOr(bool_exp1, or_label, bool_exp2);
}

STypeBoolExpPtr Compiler::ParseRelOp(int lineno, const STypePtr &exp1, STypePtr &relop, const STypePtr &exp2) {
    if (semantic_checks.IsFunctionType(exp1->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(exp2->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsLegalRelopTypes(exp1->general_type, exp2->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return code_gen.EmitRelOp(exp1, relop, exp2);
}

STypeCaseListPtr Compiler::ParseCaseList(int lineno, STypePtr case_decl, STypePtr next_list, STypePtr case_list) {
    return code_gen.EmitCaseList(move(case_decl), move(next_list), move(case_list));
}

STypeCaseListPtr Compiler::ParseCaseList(int lineno, STypePtr case_decl) {
    return code_gen.EmitCaseList(move(case_decl));
}

STypeCaseListPtr
Compiler::ParseCaseDefault(int lineno, STypePtr list_as_statement, STypePtr default_label, STypePtr statements) {
    return code_gen.EmitCaseDefault(move(list_as_statement), move(default_label), move(statements));
}

STypeCaseDeclPtr Compiler::ParseCaseDecl(int lineno, STypePtr num, STypePtr list_as_statement, STypePtr case_decl_label,
                                         STypePtr statements) {
    return code_gen.EmitCaseDecl(move(num), move(list_as_statement), move(case_decl_label), move(statements));
}

void Compiler::ParsePushStatementScope(int lineno) {
    symbol_table.PushScope(STATEMENT_SCOPE);
}

void Compiler::ParsePushWhileScope(int lineno, const STypePtr &while_head_label) {
    auto dynamic_cast_while_head_label = dynamic_pointer_cast<STypeString>(while_head_label);
    auto break_list = make_shared<branch_list>();

    symbol_table.PushScope(WHILE_SCOPE);
    symbol_table.scope_stack.top()->while_continue_label = dynamic_cast_while_head_label->token;
    symbol_table.scope_stack.top()->break_list = break_list;
    symbol_table.scope_stack.top()->inside_while = true;
}

void Compiler::ParsePushSwitchScope(int lineno, const STypePtr &switch_head_label) {
    auto dynamic_cast_switch_head_label = dynamic_pointer_cast<STypeString>(switch_head_label);
    auto break_list = make_shared<branch_list>();

    symbol_table.PushScope(SWITCH_SCOPE);
    symbol_table.scope_stack.top()->break_list = break_list;
    symbol_table.scope_stack.top()->inside_switch = true;
}

void Compiler::ParsePopScope(int lineno) {
    symbol_table.PopScope();
}

void Compiler::ParseCheckSwitchExp(int lineno, const STypePtr &num_exp) {
    if (semantic_checks.IsFunctionType(num_exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(num_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsLegalAssignTypes(INT_TYPE, num_exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }
}

void Compiler::ParseCheckBool(int lineno, const STypePtr &bool_exp) {
    if (semantic_checks.IsFunctionType(bool_exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }
}

STypeStringPtr Compiler::ParseGenIfLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_if");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenElseLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_else");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenWhileBodyLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_while_body");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenWhileHeadLabel(int lineno) {
    // note: `PushWhileScope` uses this return value, so we don't have to modify the scopes here
    auto label_name = code_gen.code_buffer.genLabel("_while_head");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenAndLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_and");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenOrLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_or");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenNextLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_next");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenDefaultLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_default");
    return make_shared<STypeString>(label_name);
}

STypeStringPtr Compiler::ParseGenCaseDeclLabel(int lineno) {
    auto label_name = code_gen.code_buffer.genLabel("_case");
    return make_shared<STypeString>(label_name);
}

STypeStatementPtr Compiler::ParseBranchNext(int lineno) {
    // in llvm we must branch before every label
    return code_gen.EmitBranchNext();
}

STypeStatementPtr Compiler::ParseBranchWhileHead(int lineno) {
    return code_gen.EmitBranchWhileHead();
}

STypeStatementPtr Compiler::ParseBranchIfNext(int lineno) {
    return code_gen.EmitParseBranchIfNext();
}

STypeStatementPtr Compiler::ParseBranchSwitchHead(int lineno) {
    return code_gen.EmitBranchSwitchHead();
}


STypeStatementPtr Compiler::ParseBranchCaseHead(int lineno) {
    return code_gen.EmitBranchCaseHead();
}

STypeStatementPtr Compiler::ParseBranchDefaultHead(int lineno) {
    return code_gen.EmitBranchDefaultHead();
}

STypePtr Compiler::ParseConvertBool(int lineno, STypePtr exp) {
    if (exp->general_type == BOOL_TYPE) {
        auto dynamic_cast_symbol = dynamic_pointer_cast<STypeSymbol>(exp);
        auto reg_for_bool = code_gen.GenRegister();
        auto result_reg_ptr = make_shared<STypeRegister>(reg_for_bool, BOOL_TYPE);

        if (dynamic_cast_symbol) {
            result_reg_ptr = code_gen.EmitLoadRegister(dynamic_cast_symbol->offset,
                                                       dynamic_cast_symbol->general_type);
        } else {
            code_gen.EmitBoolExpToRegister(exp, reg_for_bool);
        }

        return result_reg_ptr;

    } else {
        return exp;
    }
}
