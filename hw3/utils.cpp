#include "utils.h"

#include <memory>

ParseUtils::ParseUtils() : symbol_table(), semantic_checks(symbol_table) {

}


ParseUtils &ParseUtils::instance() {
    static ParseUtils parse_utils;
    return parse_utils;
}

void ParseUtils::ParseProgram(int lineno) {
    if (!semantic_checks.IsMainDefined()) {
        errorMainMissing();
        exit(0);
    }

    symbol_table.PopScope();
}

void ParseUtils::ParseFuncs(int lineno) {
    // nop
}

STypePtr ParseUtils::ParseFuncHead(int lineno, STypePtr ret_type, STypePtr id, STypePtr formals) {
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
    symbol_table.PushFunctionScope(FUNCTION_SCOPE, dynamic_cast_ret_type->general_type, function_symbol);
    for (auto param:function_symbol->parameters) {
        auto param_symbol = make_shared<STypeSymbol>(param);
        if (semantic_checks.IsSymbolDefined(param_symbol->name)) {
            errorDef(lineno, param_symbol->name);
            exit(0);
        }

        symbol_table.AddParam(param_symbol);
    }
    return function_symbol;
}

void ParseUtils::ParseFuncDecl(int lineno) {
}

STypePtr ParseUtils::ParseRetType(int lineno, STypePtr type) {
    return type;
}

STypeCTypePtr ParseUtils::ParseRetType(int lineno) {
    auto type_pointer = make_shared<STypeCType>(VOID_TYPE);
    return type_pointer;
}

STypeArgListPtr ParseUtils::ParseFormals(int lineno) {
    auto formals_pointer = make_shared<STypeArgList>();
    return formals_pointer;
}

STypeArgListPtr ParseUtils::ParseFormals(int lineno, STypePtr formals) {
    auto dynamic_cast_formals = dynamic_pointer_cast<STypeArgList>(formals);

    auto formals_pointer = make_shared<STypeArgList>();
    for (auto symbol_iter = dynamic_cast_formals->arg_list.rbegin();
         symbol_iter != dynamic_cast_formals->arg_list.rend(); symbol_iter++) {

        formals_pointer->arg_list.push_back(*symbol_iter);
    }
    return formals_pointer;
}

STypeArgListPtr ParseUtils::ParseFormalsList(int lineno, STypePtr formal) {
    auto dynamic_cast_formal = dynamic_pointer_cast<STypeSymbol>(formal);

    auto arg_list_pointer = make_shared<STypeArgList>();
    arg_list_pointer->arg_list.push_back(*dynamic_cast_formal);
    return arg_list_pointer;
}

STypeArgListPtr ParseUtils::ParseFormalsList(int lineno, STypePtr formal, STypePtr formals_list) {
    auto dynamic_cast_formals_list = dynamic_pointer_cast<STypeArgList>(formals_list);
    auto dynamic_cast_formal = dynamic_pointer_cast<STypeSymbol>(formal);

    dynamic_cast_formals_list->arg_list.push_back(*dynamic_cast_formal);
    return dynamic_cast_formals_list;
}

STypeSymbolPtr ParseUtils::ParseFormalDecl(int lineno, STypePtr type, STypePtr id) {
    auto dynamic_cast_type = dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    auto symbol_pointer = make_shared<STypeSymbol>(dynamic_cast_id->token, symbol_table.scope_stack.top()->offset,
                                                   dynamic_cast_type->general_type);
    return symbol_pointer;
}

void ParseUtils::ParseStatements(int lineno) {

}

void ParseUtils::ParseStatementOfStatements(int lineno) {

}

void ParseUtils::ParseStatementType(int lineno, STypePtr type, STypePtr id) {
    auto dynamic_cast_type = dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = dynamic_pointer_cast<STypeString>(id);

    if (semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    const auto symbol = make_shared<STypeSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    symbol_table.AddVariable(symbol);
}

void ParseUtils::ParseStatementTypeAssign(int lineno, STypePtr type, STypePtr id, STypePtr exp) {
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
}

void ParseUtils::ParseStatementAssign(int lineno, STypePtr id, STypePtr exp) {
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
}

void ParseUtils::ParseStatementCall(int lineno) {

}

void ParseUtils::ParseStatementReturn(int lineno) {
    if (!semantic_checks.IsLegalReturnType(VOID_TYPE)) {
        errorMismatch(lineno);
        exit(0);
    }
}

void ParseUtils::ParseStatementReturnExp(int lineno, STypePtr exp) {
    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if ((!semantic_checks.IsLegalReturnType(exp->general_type)) || (semantic_checks.IsVoidType(exp->general_type))) {
        errorMismatch(lineno);
        exit(0);
    }
}

void ParseUtils::ParseStatementIf(int lineno) {

}

void ParseUtils::ParseStatementIfElse(int lineno) {

}

void ParseUtils::ParseStatementWhile(int lineno) {

}

void ParseUtils::ParseStatementBreak(int lineno) {
    if (!semantic_checks.IsLegalBreak()) {
        errorUnexpectedBreak(lineno);
        exit(0);
    }
}

void ParseUtils::ParseStatementContinue(int lineno) {
    if (!semantic_checks.IsLegalContinue()) {
        errorUnexpectedContinue(lineno);
        exit(0);
    }
}

STypePtr ParseUtils::ParseCall(int lineno, STypePtr id, STypePtr exp_list) {
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

    auto reversed_exp_list = make_shared<STypeExpList>();
    for (auto exp_iter = dynamic_cast_exp_list->exp_list.rbegin();
         exp_iter != dynamic_cast_exp_list->exp_list.rend(); exp_iter++) {

        reversed_exp_list->exp_list.push_back(*exp_iter);
    }

    if (!semantic_checks.IsLegalCallTypes(dynamic_cast_func, reversed_exp_list)) {
        vector<string> expected_args;
        for (auto symbol:dynamic_cast_func->parameters) {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    auto ret_type_pointer = make_shared<STypeBase>(dynamic_cast_func->ret_type);
    return ret_type_pointer;
}

STypePtr ParseUtils::ParseCall(int lineno, STypePtr id) {
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
        for (auto symbol:dynamic_cast_func->parameters) {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    auto ret_type_pointer = make_shared<STypeBase>(dynamic_cast_func->ret_type);
    return ret_type_pointer;
}

STypePtr ParseUtils::ParseExplist(int lineno, STypePtr exp) {
    auto exp_list_pointer = make_shared<STypeExpList>();
    exp_list_pointer->exp_list.push_back(*exp);
    return exp_list_pointer;
}

STypePtr ParseUtils::ParseExplist(int lineno, STypePtr exp, STypePtr exp_list) {
    auto dynamic_cast_exp_list = dynamic_pointer_cast<STypeExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.push_back(*exp);
    return dynamic_cast_exp_list;
}

STypeCTypePtr ParseUtils::ParseInt(int lineno) {
    return std::make_shared<STypeCType>(INT_TYPE);
}

STypeCTypePtr ParseUtils::ParseByte(int lineno) {
    return std::make_shared<STypeCType>(BYTE_TYPE);
}

STypeCTypePtr ParseUtils::ParseBool(int lineno) {
    return std::make_shared<STypeCType>(BOOL_TYPE);
}

STypePtr ParseUtils::ParseParentheses(int lineno, STypePtr exp) {
    return exp;
}

STypePtr ParseUtils::ParseBinop(int lineno, STypePtr exp1, STypePtr exp2) {
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

    if (semantic_checks.CheckAndGetBinOpType(exp1->general_type, exp2->general_type) == OTHER_TYPE) {
        errorMismatch(lineno);
        exit(0);
    }
    return exp1;
}

STypePtr ParseUtils::ParseID(int lineno, STypePtr id) {
    auto dynamic_cast_string = dynamic_pointer_cast<STypeString>(id);

    if (!symbol_table.IsSymbolDefined(dynamic_cast_string->token)) {
        errorUndef(lineno, dynamic_cast_string->token);
        exit(0);
    }

    auto id_pointer = symbol_table.GetDefinedSymbol(dynamic_cast_string->token);
    return id_pointer;
}

STypePtr ParseUtils::ParseCallExp(int lineno, STypePtr call_exp) {
    return call_exp;
}

STypeNumberPtr ParseUtils::ParseNum(int lineno, STypePtr num) {
    auto dynamic_cast_num = dynamic_pointer_cast<STypeNumber>(num);

    return dynamic_cast_num;
}

STypePtr ParseUtils::ParseNumB(int lineno, STypePtr num) {
    auto dynamic_cast_num = dynamic_pointer_cast<STypeNumber>(num);

    if (!semantic_checks.IsByteOverflow(dynamic_cast_num->token)) {
        errorByteTooLarge(lineno, to_string(dynamic_cast_num->token));
        exit(0);
    }
    num->general_type = BYTE_TYPE;
    return num;
}

STypeStringPtr ParseUtils::ParseString(int lineno, STypePtr stype_string) {
    auto dynamic_cast_string = dynamic_pointer_cast<STypeString>(stype_string);

    return dynamic_cast_string;
}

STypeBoolPtr ParseUtils::ParseTrue(int lineno) {
    auto bool_pointer = make_shared<STypeBool>(true);
    return bool_pointer;
}

STypeBoolPtr ParseUtils::ParseFalse(int lineno) {
    auto bool_pointer = make_shared<STypeBool>(false);
    return bool_pointer;
}

STypePtr ParseUtils::ParseNot(int lineno, STypePtr bool_exp) {
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

    return bool_exp;
}

STypePtr ParseUtils::ParseAnd(int lineno, STypePtr bool_exp1, STypePtr bool_exp2) {
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

    return bool_exp1;
}

STypePtr ParseUtils::ParseOr(int lineno, STypePtr bool_exp1, STypePtr bool_exp2) {
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

    return bool_exp1;
}

STypeBoolPtr ParseUtils::ParseRelOp(int lineno, STypePtr exp1, STypePtr exp2) {
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

    auto bool_pointer = make_shared<STypeBool>(true);
    return bool_pointer;
}

STypePtr ParseUtils::ParseCast(int lineno, STypePtr type, STypePtr exp) {
    auto dynamic_cast_type = dynamic_pointer_cast<STypeCType>(type);

    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }


    if (!semantic_checks.IsLegalCast(dynamic_cast_type->general_type, exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    exp->general_type = dynamic_cast_type->general_type;
    return exp;
}

void ParseUtils::ParsePushStatementScope(int lineno) {
    symbol_table.PushScope(STATEMENT_SCOPE);
}

void ParseUtils::ParsePushWhileScope(int lineno) {
    symbol_table.PushScope(WHILE_SCOPE);
}

void ParseUtils::ParsePopScope(int lineno) {
    symbol_table.PopScope();
}

void ParseUtils::ParseCheckBool(int lineno, STypePtr bool_exp) {
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