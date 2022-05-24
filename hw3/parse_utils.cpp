#include "parse_utils.h"

#include <memory>

Utils::Utils() : symbol_table(), semantic_checks(symbol_table)
{
}

Utils &Utils::instance()
{
    static Utils parse_utils;
    return parse_utils;
}

void Utils::ParseProgram(int lineno)
{
    if (!semantic_checks.IsMainDefined()) {
        errorMainMissing();
        exit(0);
    }

    symbol_table.PopScope();
}



Tptr Utils::ParseFuncHead(int lineno, Tptr ret_type, Tptr id, Tptr formals)
{
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);
    auto dynamic_cast_ret_type = std::dynamic_pointer_cast<STypeCType>(ret_type);
    auto dy_cast = std::dynamic_pointer_cast<SimpleSymbolList>(formals);

    if (symbol_table.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto function_symbol = make_shared<STypeFunctionSymbol>(dynamic_cast_id->token, dynamic_cast_ret_type->general_type,
                                                            dy_cast->symbols_list);
    symbol_table.AddFunction(function_symbol);
    symbol_table.PushFunctionScope(FUNCTION_SCOPE, dynamic_cast_ret_type->general_type, function_symbol);
    for (auto param:function_symbol->parameters) {
        auto param_symbol = make_shared<SimpleSymbol>(param);
        if (semantic_checks.IsSymbolDefined(param_symbol->name)) {
            errorDef(lineno, param_symbol->name);
            exit(0);
        }

        symbol_table.AddParam(param_symbol);
    }
    return function_symbol;
}



STypeCTypePtr Utils::ParseRetType(int lineno)
{
    auto type_pointer = make_shared<STypeCType>(VOID_TYPE);
    return type_pointer;
}

Tptr Utils::ParseRetType(int lineno, Tptr type)
{
    return type;
}

SimpleSymbolListPtr Utils::ParseFormals(int lineno, Tptr formals)
{
    auto dy_cast = std::dynamic_pointer_cast<SimpleSymbolList>(formals);

    auto fp = make_shared<SimpleSymbolList>();
    for (auto symbol_iter = dy_cast->symbols_list.rbegin();
         symbol_iter != dy_cast->symbols_list.rend(); symbol_iter++) {

        fp->symbols_list.push_back(*symbol_iter);
    }
    return fp;
}

SimpleSymbolListPtr Utils::ParseFormals(int lineno)
{
    auto fp = make_shared<SimpleSymbolList>();
    return fp;
}

SimpleSymbolListPtr Utils::ParseFormalsList(int lineno, Tptr formal)
{
    auto arg_list_pointer = make_shared<SimpleSymbolList>();
    arg_list_pointer->symbols_list.push_back(*std::dynamic_pointer_cast<SimpleSymbol>(formal));
    return arg_list_pointer;
}

SimpleSymbolListPtr Utils::ParseFormalsList(int lineno, Tptr formal, Tptr formals_list)
{
    auto dynamic_cast_formals_list = std::dynamic_pointer_cast<SimpleSymbolList>(formals_list);
    auto dynamic_cast_formal = std::dynamic_pointer_cast<SimpleSymbol>(formal);

    dynamic_cast_formals_list->symbols_list.push_back(*dynamic_cast_formal);
    return dynamic_cast_formals_list;
}

SimpleSymbolPtr Utils::ParseFormalDecl(int lineno, Tptr type, Tptr id)
{

    auto symbol_pointer = make_shared<SimpleSymbol>(std::dynamic_pointer_cast<STypeString>(id)->token, symbol_table.scope_stack.top()->offset,
                                                   std::dynamic_pointer_cast<STypeCType>(type)->general_type);
    return symbol_pointer;
}



void Utils::ParseStatementType(int lineno, Tptr type, Tptr id)
{
    auto dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorDef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    symbol_table.AddVariable(symbol);
}

void Utils::ParseStatementTypeAssign(int lineno, Tptr type, Tptr id, Tptr exp)
{
    auto dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
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

    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    symbol_table.AddVariable(symbol);
}

void Utils::ParseStatementAssign(int lineno, Tptr id, Tptr exp)
{
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorUndef(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(symbol_from_id->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsLegalAssignTypes(symbol_from_id->general_type, exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }
}



void Utils::ParseStatementReturn(int lineno)
{
    if (!semantic_checks.IsLegalReturnType(VOID_TYPE)) {
        errorMismatch(lineno);
        exit(0);
    }
}

void Utils::ParseStatementReturnExp(int lineno, Tptr exp)
{
    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if ((!semantic_checks.IsLegalReturnType(exp->general_type)) || (semantic_checks.IsVoidType(exp->general_type))) {
        errorMismatch(lineno);
        exit(0);
    }
}

void Utils::ParseStatementBreak(int lineno)
{
    if (!semantic_checks.IsLegalBreak()) {
        errorUnexpectedBreak(lineno);
        exit(0);
    }
}

void Utils::ParseStatementContinue(int lineno)
{
    if (!semantic_checks.IsLegalContinue()) {
        errorUnexpectedContinue(lineno);
        exit(0);
    }
}




Tptr Utils::ParseCall(int lineno, Tptr id, Tptr exp_list)
{
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (!semantic_checks.IsFunctionType(symbol_from_id->general_type)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);

    auto reversed_exp_list = make_shared<STypeExpList>();
    for (auto exp_iter = dynamic_cast_exp_list->exp_list.rbegin();
         exp_iter != dynamic_cast_exp_list->exp_list.rend(); exp_iter++) {

        reversed_exp_list->exp_list.push_back(*exp_iter);
    }

    if (!semantic_checks.IsLegalCallTypes(dynamic_cast_func, reversed_exp_list)) {
        std::vector<std::string> expected_args;
        for (auto symbol:dynamic_cast_func->parameters) {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    auto ret_type_pointer = make_shared<TBase>(dynamic_cast_func->ret_type);
    return ret_type_pointer;
}

Tptr Utils::ParseCall(int lineno, Tptr id)
{
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.IsSymbolDefined(dynamic_cast_id->token)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dynamic_cast_id->token);

    if (!semantic_checks.IsFunctionType(symbol_from_id->general_type)) {
        errorUndefFunc(lineno, dynamic_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto empty_exp_list = make_shared<STypeExpList>();

    if (!semantic_checks.IsLegalCallTypes(dynamic_cast_func, empty_exp_list)) {
        std::vector<std::string> expected_args;
        for (auto symbol:dynamic_cast_func->parameters) {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(lineno, dynamic_cast_id->token, expected_args);
        exit(0);
    }

    auto ret_type_pointer = make_shared<TBase>(dynamic_cast_func->ret_type);
    return ret_type_pointer;
}

Tptr Utils::ParseExplist(int lineno, Tptr exp)
{
    auto exp_list_pointer = make_shared<STypeExpList>();
    exp_list_pointer->exp_list.push_back(*exp);
    return exp_list_pointer;
}

Tptr Utils::ParseExplist(int lineno, Tptr exp, Tptr exp_list)
{
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.push_back(*exp);
    return dynamic_cast_exp_list;
}

STypeCTypePtr Utils::ParseInt(int lineno)
{
    return std::make_shared<STypeCType>(INT_TYPE);
}

STypeCTypePtr Utils::ParseByte(int lineno)
{
    return std::make_shared<STypeCType>(BYTE_TYPE);
}

STypeCTypePtr Utils::ParseBool(int lineno)
{
    return std::make_shared<STypeCType>(BOOL_TYPE);
}

Tptr Utils::ParseParentheses(int lineno, Tptr exp)
{
    return exp;
}

Tptr Utils::ParseBinop(int lineno, Tptr exp1, Tptr exp2)
{
    if (semantic_checks.IsFunctionType(exp1->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(exp2->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckAndGetBinOpType(exp1->general_type, exp2->general_type) == OTHER_TYPE) {
        errorMismatch(lineno);
        exit(0);
    }
    return exp1;
}

Tptr Utils::ParseID(int lineno, Tptr id)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<STypeString>(id);

    if (!symbol_table.IsSymbolDefined(dynamic_cast_string->token)) {
        errorUndef(lineno, dynamic_cast_string->token);
        exit(0);
    }

    auto id_pointer = symbol_table.GetDefinedSymbol(dynamic_cast_string->token);
    return id_pointer;
}

Tptr Utils::ParseCallExp(int lineno, Tptr call_exp)
{
    return call_exp;
}

STypeNumberPtr Utils::ParseNum(int lineno, Tptr num)
{
    auto dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    return dynamic_cast_num;
}

Tptr Utils::ParseNumB(int lineno, Tptr num)
{
    auto dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    if (!semantic_checks.IsByteOverflow(dynamic_cast_num->token)) {
        errorByteTooLarge(lineno, to_string(dynamic_cast_num->token));
        exit(0);
    }
    num->general_type = BYTE_TYPE;
    return num;
}

STypeStringPtr Utils::ParseString(int lineno, Tptr stype_string)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<STypeString>(stype_string);

    return dynamic_cast_string;
}

STypeBoolPtr Utils::ParseTrue(int lineno)
{
    auto bool_pointer = make_shared<STypeBool>(true);
    return bool_pointer;
}

STypeBoolPtr Utils::ParseFalse(int lineno)
{
    auto bool_pointer = make_shared<STypeBool>(false);
    return bool_pointer;
}

Tptr Utils::ParseNot(int lineno, Tptr bool_exp)
{
    // exp can be a bool literal or an id
    if (semantic_checks.IsFunctionType(bool_exp->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }

    return bool_exp;
}

Tptr Utils::ParseAnd(int lineno, Tptr bool_exp1, Tptr bool_exp2)
{
    if (semantic_checks.IsFunctionType(bool_exp1->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(bool_exp2->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
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

Tptr Utils::ParseOr(int lineno, Tptr bool_exp1, Tptr bool_exp2)
{
    if (semantic_checks.IsFunctionType(bool_exp1->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(bool_exp2->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
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

STypeBoolPtr Utils::ParseRelOp(int lineno, Tptr exp1, Tptr exp2)
{
    if (semantic_checks.IsFunctionType(exp1->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(exp2->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
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

Tptr Utils::ParseCast(int lineno, Tptr type, Tptr exp)
{
    auto dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);

    if (semantic_checks.IsFunctionType(exp->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
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

void Utils::ParsePushStatementScope(int lineno)
{
    symbol_table.PushScope(STATEMENT_SCOPE);
}

void Utils::ParsePushWhileScope(int lineno)
{
    symbol_table.PushScope(WHILE_SCOPE);
}

void Utils::ParsePopScope(int lineno)
{
    symbol_table.PopScope();
}

void Utils::ParseCheckBool(int lineno, Tptr bool_exp)
{
    if (semantic_checks.IsFunctionType(bool_exp->general_type)) {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorUndef(lineno, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp->general_type)) {
        errorMismatch(lineno);
        exit(0);
    }
}