#include "parse_utils.h"

#include <memory>

Utils::Utils() : symbol_table(), semantic_checks(symbol_table){}

Utils &Utils::instance()
{
    static Utils pu;
    return pu;
}
// rm- changed order, 
void Utils::pProgram(int ln)
{
    if (semantic_checks.IsMainDefined())
    {
       symbol_table.PopScope();
    }else{
     errorMissingMain();
        exit(0);
        }
    
}

// rm - changed order, defs of args, if logic, auto removed
Tptr Utils::parseFunctionDef(int lnno, Tptr return_type, Tptr id, Tptr formals)
{

    STypeFunctionSymbolPtr func_sym = make_shared<STypeFunctionSymbol>(std::dynamic_pointer_cast<STypeString>(id)->token, std::dynamic_pointer_cast<STypeCType>(return_type)->general_type,
                                               std::dynamic_pointer_cast<SimpleSymbolList>(formals)->symbols_list);
    if (symbol_table.IsSymbolDefined(std::dynamic_pointer_cast<STypeString>(id)->token))
    {
        errorInDefintion(lnno, std::dynamic_pointer_cast<STypeString>(id)->token);
        exit(0);
    }
    else
    {
        symbol_table.AddFunction(func_sym);
        symbol_table.PushFunctionScope(SCOPE_FUNCTION, std::dynamic_pointer_cast<STypeCType>(return_type)->general_type, func_sym);
        for (auto param : func_sym->parameters)
        {
            auto param_symbol = make_shared<SimpleSymbol>(param);
            if (!semantic_checks.checkSymbolDefined(param_symbol->name))
            {
               symbol_table.AddParam(param_symbol); 
            }
            else{
                errorInDefintion(lnno, param_symbol->name);
                exit(0);

            }  
        }
    }
    return func_sym;
}

// rm
TypePtr Utils::pReturnType(int ln)
{
    return make_shared<STypeCType>(VOID_TYPE);
}

Tptr Utils::pReturnType(int ln, Tptr type)
{
    return type;
}

SymListPtr Utils::pFormals(int ln, Tptr formals)
{

    auto fp = make_shared<SimpleSymbolList>();
    for (auto sym = std::dynamic_pointer_cast<SimpleSymbolList>(formals)->symbols_list.rbegin();
         sym != std::dynamic_pointer_cast<SimpleSymbolList>(formals)->symbols_list.rend(); sym++)
    {

        fp->symbols_list.push_back(*sym);
    }
    return fp;
}

SymListPtr Utils::pFormals(int ln)
{
    return make_shared<SimpleSymbolList>();
}

SymListPtr Utils::pFormList(int ln, Tptr formal)
{
    auto arg_list_pointer = make_shared<SimpleSymbolList>();
    arg_list_pointer->symbols_list.push_back(*std::dynamic_pointer_cast<SimpleSymbol>(formal));
    return arg_list_pointer;
}

SymListPtr Utils::pFormList(int ln, Tptr formal, Tptr formals_list)
{
    auto dyn_cast_formal = std::dynamic_pointer_cast<SimpleSymbol>(formal);
    auto dyn_cast_formal_list = std::dynamic_pointer_cast<SimpleSymbolList>(formals_list);
    dyn_cast_formal_list->symbols_list.push_back(*dyn_cast_formal);
    return dyn_cast_formal_list;
}

SymbolPtr Utils::ParseFormalDecl(int ln, Tptr type, Tptr id)
{

    auto symbol_pointer = make_shared<SimpleSymbol>(std::dynamic_pointer_cast<STypeString>(id)->token, symbol_table.scope_stack.top()->offset,
                                                    std::dynamic_pointer_cast<STypeCType>(type)->general_type);
    return symbol_pointer;
}

void Utils::parseStateType(int ln, Tptr type, Tptr id)
{
    auto dyn_cast_type = std::dynamic_pointer_cast<STypeCType>(type);   // initialize token's type
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);  // initalize token's name

    // validate that there is no other symbol with the same name
    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
    const auto symbol = make_shared<SimpleSymbol>(dyn_cast_id->token, 0, dyn_cast_type->general_type);
    // add the symbol which holds: (token's name, offset, type)
    symbol_table.AddVariable(symbol);
    }
    else{
        errorInDefintion(ln, dyn_cast_id->token);
        exit(0);
    }

    
}

void Utils::parseStateTypeAssignment(int ln, Tptr type, Tptr id, Tptr exp)
{
    bool check_semantics;
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);
    auto dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    
    check_semantics = semantic_checks.IsFunctionType(exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    // Auto type needs to be casted to the right hand expression's type
    if (dynamic_cast_type->general_type == AUTO_TYPE) {
        // if (exp->general_type == INT_TYPE) {
        //     // cast id to int?
        // }
        dynamic_cast_type->general_type = exp->general_type;
    }
    check_semantics = semantic_checks.IsLegalAssignTypes(dynamic_cast_type->general_type, exp->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    check_semantics = semantic_checks.checkSymbolDefined(dynamic_cast_id->token);
    if (check_semantics)
    {
        errorInDefintion(ln, dynamic_cast_id->token);
        exit(0);
    }
    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    symbol_table.AddVariable(symbol);
}

void Utils::parseStateAssignment(int ln, Tptr id, Tptr exp)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    bool check_semantics  = semantic_checks.checkSymbolDefined(dyn_cast_id->token);
    if (!check_semantics)
    {
        errorIsUndefined(ln, dyn_cast_id->token);
        exit(0);
    }

    auto sym_from_id = symbol_table.GetDefinedSymbol(dyn_cast_id->token);
    check_semantics = semantic_checks.IsFunctionType(exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsFunctionType(sym_from_id->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(sym_from_id);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsLegalAssignTypes(sym_from_id->general_type, exp->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
}

void Utils::parseStateRet(int ln)
{
    bool check_semantics = semantic_checks.IsLegalReturnType(VOID_TYPE);
    if (!check_semantics){
        errorDoesNotMatch(ln);
        exit(0);
    }
}

void Utils::parseStateRetExpression(int ln, Tptr exp)
{
     bool check_semantics = semantic_checks.IsFunctionType(exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = (!semantic_checks.IsLegalReturnType(exp->general_type)) || (semantic_checks.IsVoidType(exp->general_type));
    if (check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
}

void Utils::parseStateBreak(int ln)
{
    bool check_semantics=semantic_checks.IsLegalBreak();
    if (!check_semantics)
    {
        errorBreak(ln);
        exit(0);
    }
}

void Utils::parseStateContinue(int ln)
{
    bool check_semantics=semantic_checks.IsLegalContinue();
    if (!check_semantics)
    {
        errorContinue(ln);
        exit(0);
    }
}

Tptr Utils::pCall(int ln, Tptr id, Tptr exp_list)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dyn_cast_id->token);

    if (!semantic_checks.IsFunctionType(symbol_from_id->general_type))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }
    auto reversed_exp_list = make_shared<STypeExpList>();
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);
    auto dyn_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    
    for (auto exp = dynamic_cast_exp_list->exp_list.rbegin();
         exp != dynamic_cast_exp_list->exp_list.rend(); exp++)
    {

        reversed_exp_list->exp_list.push_back(*exp);
    }

    if (!semantic_checks.IsLegalCallTypes(dyn_cast_func, reversed_exp_list))
    {
        std::vector<std::string> expected_args;
        for (auto sym : dyn_cast_func->parameters)
        {
            expected_args.push_back(TypeToString(sym.general_type));
        }
        errorPrototypeMismatch(ln, dyn_cast_id->token, expected_args);
        exit(0);
    }

    auto ret_type_pointer = make_shared<TBase>(dyn_cast_func->ret_type);
    return ret_type_pointer;
}

Tptr Utils::pCall(int ln, Tptr id)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dyn_cast_id->token);

    if (!semantic_checks.IsFunctionType(symbol_from_id->general_type))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto empty_exp_list = make_shared<STypeExpList>();

    if (!semantic_checks.IsLegalCallTypes(dynamic_cast_func, empty_exp_list))
    {
        std::vector<std::string> expected_args;
        for (auto symbol : dynamic_cast_func->parameters)
        {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        errorPrototypeMismatch(ln, dyn_cast_id->token, expected_args);
        exit(0);
    }

    auto ret_type_pointer = make_shared<TBase>(dynamic_cast_func->ret_type);
    return ret_type_pointer;
}

Tptr Utils::pExpressionList(int ln, Tptr exp)
{
    auto exp_list_pointer = make_shared<STypeExpList>();
    exp_list_pointer->exp_list.push_back(*exp);
    return exp_list_pointer;
}

Tptr Utils::pExpressionList(int ln, Tptr exp, Tptr exp_list)
{
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.push_back(*exp);
    return dynamic_cast_exp_list;
}

TypePtr Utils::pInt(int ln)
{
    return std::make_shared<STypeCType>(INT_TYPE);
}

TypePtr Utils::pByte(int ln)
{
    return std::make_shared<STypeCType>(BYTE_TYPE);
}

TypePtr Utils::pBool(int ln)
{
    return std::make_shared<STypeCType>(BOOL_TYPE);
}

Tptr Utils::pParen(int ln, Tptr exp)
{
    return exp;
}

Tptr Utils::pBinaryOp(int ln, Tptr exp1, Tptr exp2)
{
    if (semantic_checks.IsFunctionType(exp1->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }

    if (semantic_checks.IsFunctionType(exp2->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }

    if (semantic_checks.CheckAndGetBinOpType(exp1->general_type, exp2->general_type) == OTHER_TYPE)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    return exp1;
}

Tptr Utils::pId(int ln, Tptr id)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<STypeString>(id);

    if (!symbol_table.IsSymbolDefined(dynamic_cast_string->token))
    {
        errorIsUndefined(ln, dynamic_cast_string->token);
        exit(0);
    }

    auto id_pointer = symbol_table.GetDefinedSymbol(dynamic_cast_string->token);
    return id_pointer;
}

Tptr Utils::pCallExpression(int ln, Tptr call_exp)
{
    return call_exp;
}

NumberTypePtr Utils::pNum(int ln, Tptr num)
{
    auto dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    return dynamic_cast_num;
}

Tptr Utils::pNumB(int ln, Tptr num)
{
    auto dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    if (!semantic_checks.IsByteOverflow(dynamic_cast_num->token))
    {
        errorByteIsTooBig(ln, to_string(dynamic_cast_num->token));
        exit(0);
    }
    num->general_type = BYTE_TYPE;
    return num;
}

StringTypePtr Utils::pString(int ln, Tptr stype_string)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<STypeString>(stype_string);

    return dynamic_cast_string;
}

BoolTypePtr Utils::pTrue(int ln)
{
    auto bool_pointer = make_shared<STypeBool>(true);
    return bool_pointer;
}

BoolTypePtr Utils::pFalse(int ln)
{
    auto bool_pointer = make_shared<STypeBool>(false);
    return bool_pointer;
}

Tptr Utils::pNot(int ln, Tptr bool_exp)
{
    // exp can be a bool literal or an id
    if (semantic_checks.IsFunctionType(bool_exp->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.IsBoolType(bool_exp->general_type))
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    return bool_exp;
}

Tptr Utils::pAnd(int ln, Tptr bool_exp1, Tptr bool_exp2)
{
    bool check_semantics = semantic_checks.IsFunctionType(bool_exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsFunctionType(bool_exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsBoolType(bool_exp1->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    check_semantics= semantic_checks.IsBoolType(bool_exp2->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    return bool_exp1;
}

Tptr Utils::pOr(int ln, Tptr bool_exp1, Tptr bool_exp2)
{
    bool check_semantics = semantic_checks.IsFunctionType(bool_exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsFunctionType(bool_exp2->general_type); 
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsBoolType(bool_exp1->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    check_semantics = semantic_checks.IsBoolType(bool_exp2->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    return bool_exp1;
}

BoolTypePtr Utils::pRelOprator(int ln, Tptr exp1, Tptr exp2)
{
    bool check_semantics = semantic_checks.IsFunctionType(exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsFunctionType(exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsLegalRelopTypes(exp1->general_type, exp2->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    return make_shared<STypeBool>(true);
}

Tptr Utils::pCast(int ln, Tptr type, Tptr exp)
{
    bool check_semantics = semantic_checks.IsFunctionType(exp->general_type);
    auto dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);

    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.IsLegalCast(dynamic_cast_type->general_type, exp->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    exp->general_type = dynamic_cast_type->general_type;
    return exp;
}

void Utils::pAddStateScope(int ln)
{
    symbol_table.PushScope(SCOPE_STATEMENT);
}

void Utils::pAddWhileScope(int ln)
{
    symbol_table.PushScope(SCOPE_WHILE);
}

void Utils::pPopScope(int ln)
{
    symbol_table.PopScope();
}

void Utils::pCheckBool(int ln, Tptr bool_exp)
{
    bool check_semantics = semantic_checks.IsFunctionType(bool_exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics =  semantic_checks.IsBoolType(bool_exp->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
}