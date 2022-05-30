#include "utils.h"

#include <memory>

Utils::Utils() : symbol_table(), semantic_checks(symbol_table) {}

void Utils::handleErrorIsUndefined(int ln, std::string s)
{
    errorIsUndefined(ln, s);
    exit(0);
}
void Utils::handleErrorPrototypeMismatch(int ln, const string &id, std::vector<string> &argTypes)
{
    errorPrototypeMismatch(ln, id, argTypes);
    exit(0);
}

void Utils::handleErrorDoesNotMatch(int ln)
{
    errorDoesNotMatch(ln);
    exit(0);
}

void Utils::handleErrorInDefintion(int ln, string s)
{
    errorInDefintion(ln, s);
    exit(0);
}

void Utils::handleErrorUndefinedFunction(int ln, string s)
{
    errorUndefinedFunction(ln, s);
    exit(0);
}

void Utils::handleErrorBreak(int ln)
{
    errorBreak(ln);
    exit(0);
}

void Utils::handleErrorInContinue(int ln)
{
    errorContinue(ln);
    exit(0);
}
void Utils::handleErrorInMissingMain(int lineno)
{
    errorMissingMain();
    exit(0);
}
void Utils::handleErrorByteIsTooBig(int ln, string s)
{
    errorByteIsTooBig(ln, s);
    exit(0);
}

Utils &Utils::instance()
{
    static Utils pu;
    return pu;
}

void Utils::pProgram(int ln)
{
    if (semantic_checks.checkMainIsDefined())
    {
        symbol_table.deleteScope();
    }
    else
    {
        handleErrorInMissingMain(ln);
    }
}

Tptr Utils::parseFunctionDef(Tptr return_type, Tptr id, Tptr formals, int lnno)
{

    FuncSymTypePtr func_sym = make_shared<FuncSymType>(std::dynamic_pointer_cast<StringType>(id)->token, std::dynamic_pointer_cast<CType>(return_type)->g_type,
                                                                       std::dynamic_pointer_cast<SimpleSymbolList>(formals)->syms_list);
    if (symbol_table.checkIfSymbolDefined(std::dynamic_pointer_cast<StringType>(id)->token))
    {
        handleErrorInDefintion(lnno, std::dynamic_pointer_cast<StringType>(id)->token);
    }
    else
    {
        symbol_table.addFunc(func_sym);
        symbol_table.addFuncScope(SCOPE_FUNCTION, std::dynamic_pointer_cast<CType>(return_type)->g_type, func_sym);
        for (SimpleSymbol param : func_sym->parameters)
        {
            std::shared_ptr<SimpleSymbol> param_symbol = make_shared<SimpleSymbol>(param);
            if (!semantic_checks.checkSymbolDefined(param_symbol->n))
            {
                symbol_table.AddParameter(param_symbol);
            }
            else
            {
                handleErrorInDefintion(lnno, param_symbol->n);
            }
        }
    }
    return func_sym;
}

Tptr Utils::pExpressionList(Tptr exp, Tptr exp_list, int ln)
{
    std::shared_ptr<TExpList> dynamic_cast_exp_list = std::dynamic_pointer_cast<TExpList>(exp_list);

    dynamic_cast_exp_list->expression_list.push_back(*exp);
    return dynamic_cast_exp_list;
}

SymPtr Utils::ParseFormalDecl(Tptr type, Tptr id, int ln)
{

    std::shared_ptr<SimpleSymbol> symbol_pointer = make_shared<SimpleSymbol>(std::dynamic_pointer_cast<StringType>(id)->token, symbol_table.s_stack.top()->offset,
                                                                             std::dynamic_pointer_cast<CType>(type)->g_type);
    return symbol_pointer;
}

void Utils::parseStateType(Tptr type, Tptr id, int ln)
{
    std::shared_ptr<CType> dyn_cast_type = std::dynamic_pointer_cast<CType>(type);
    std::shared_ptr<StringType> dyn_cast_id = std::dynamic_pointer_cast<StringType>(id);
    const auto symbol = make_shared<SimpleSymbol>(dyn_cast_id->token, 0, dyn_cast_type->g_type);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        symbol_table.AddVar(symbol);
    }
    else
    {
        handleErrorInDefintion(ln, dyn_cast_id->token);
    }
}

TypePtr Utils::pInt(int ln)
{
    return std::make_shared<CType>(INT_TYPE);
}

TypePtr Utils::pByte(int ln)
{
    return std::make_shared<CType>(BYTE_TYPE);
}

StringTypePtr Utils::pString(Tptr stype_string, int ln)
{
    return std::dynamic_pointer_cast<StringType>(stype_string);
}

BoolTypePtr Utils::pTrue(int ln)
{

    return make_shared<BoolType>(true);
}

SymListPtr Utils::pFormList(Tptr f, Tptr f_list, int ln)
{
    auto dyn_cast_formal = std::dynamic_pointer_cast<SimpleSymbol>(f);
    auto dyn_cast_formal_list = std::dynamic_pointer_cast<SimpleSymbolList>(f_list);
    dyn_cast_formal_list->syms_list.push_back(*dyn_cast_formal);
    return dyn_cast_formal_list;
}

void Utils::parseAutoTypeAssignment(Tptr type, Tptr id, Tptr exp, int ln)
{
    // std::cout << "[DEBUG] start parseAutoTypeAssignment" << std::endl;
    // std::cout << "[DEBUG] type: " << type->general_type << std::endl;
    // std::cout << "[DEBUG] exp: " << exp->general_type << std::endl;
    bool check_semantics;
    auto dynamic_cast_id = std::dynamic_pointer_cast<StringType>(id);
    if (exp->g_type != INT_TYPE && exp->g_type != BYTE_TYPE && exp->g_type != BOOL_TYPE)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    type->g_type = exp->g_type;
    TypePtr dynamic_cast_type = std::dynamic_pointer_cast<CType>(type);
    // std::cout << dynamic_cast_type << std::endl;
    // std::cout << "[DEBUG] id token : " << dynamic_cast_id->token <<std::endl;
    // std::cout << "[DEBUG] found auto type!" <<std::endl;
    // std::cout << "[DEBUG] Casting to: " << exp->general_type <<std::endl;
    // std::cout << "[DEBUG] dynamic_cast_type: " << dynamic_cast_type->general_type <<std::endl;
    // std::cout << "[DEBUG] dynamic_cast_id->general_type: " << dynamic_cast_id->general_type <<std::endl;
    check_semantics = semantic_checks.checkFunction(exp->g_type);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside IsFunctionType " << std::endl;
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    // std::cout << "[DEBUG] 2 " << std::endl;
    check_semantics = semantic_checks.checkSymbolDefined(dynamic_cast_id->token);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside checkSymbolDefined " << std::endl;
        handleErrorInDefintion(ln, dynamic_cast_id->token);
    }
    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->g_type);
    // std::cout << "[DEBUG] Adding Variable... " << std::endl;
    symbol_table.AddVar(symbol);
}

void Utils::parseStateTypeAssignment(Tptr type, Tptr id, Tptr exp, int ln)
{
    // std::cout << "[DEBUG] exp: " << exp->general_type << std::endl;
    bool check_semantics;
    auto dynamic_cast_id = std::dynamic_pointer_cast<StringType>(id);
    TypePtr dynamic_cast_type = std::dynamic_pointer_cast<CType>(type);
    // std::cout << dynamic_cast_type << std::endl;
    check_semantics = semantic_checks.checkFunction(exp->g_type);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside IsFunctionType " << std::endl;
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    // std::cout << "[DEBUG] 1 " << std::endl;
    check_semantics = semantic_checks.checkAssigned(dynamic_cast_type->g_type, exp->g_type);
    if (!check_semantics)
    {
        // std::cout << "[DEBUG] inside IsLegalAssignTypes " << std::endl;
        handleErrorDoesNotMatch(ln);
        exit(0);
    }
    // std::cout << "[DEBUG] 2 " << std::endl;
    check_semantics = semantic_checks.checkSymbolDefined(dynamic_cast_id->token);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside checkSymbolDefined " << std::endl;
        handleErrorInDefintion(ln, dynamic_cast_id->token);
    }
    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->g_type);
    // std::cout << "[DEBUG] Adding Variable... " << std::endl;
    symbol_table.AddVar(symbol);
}

Tptr Utils::pNot(Tptr bool_exp, int ln)
{
    bool  check_type = semantic_checks.checkFunction(bool_exp->g_type);
    if (check_type)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(bool_exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_type = semantic_checks.checkBool(bool_exp->g_type);
    if (!check_type)
    {
        handleErrorDoesNotMatch(ln);
    }

    return bool_exp;
}

Tptr Utils::pAnd(Tptr bool_exp1, Tptr bool_exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp1->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(bool_exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(bool_exp2->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(bool_exp2);
    }
    bool check_semantics1 = semantic_checks.checkBool(bool_exp1->g_type), check_semantics2 = semantic_checks.checkBool(bool_exp2->g_type);
    if (!check_semantics1 || !check_semantics2)
    {
        handleErrorDoesNotMatch(ln);
    }

    return bool_exp1;
}

void Utils::parseStateAssignment(Tptr id, Tptr exp, int ln)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<StringType>(id);

    bool check_semantics = semantic_checks.checkSymbolDefined(dyn_cast_id->token);
    if (!check_semantics)
    {
        handleErrorIsUndefined(ln, dyn_cast_id->token);
    }
    check_semantics = semantic_checks.checkFunction(exp->g_type);
    auto sym_from_id = symbol_table.retDefinedSym(dyn_cast_id->token);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(sym_from_id->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(sym_from_id);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkAssigned(sym_from_id->g_type, exp->g_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
}

void Utils::parseStateRet(int ln)
{
    bool check_semantics = semantic_checks.checkReturn(VOID_TYPE);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
}

Tptr Utils::pCast(Tptr type, Tptr exp, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp->g_type);
    std::shared_ptr<CType> dynamic_cast_type = std::dynamic_pointer_cast<CType>(type);

    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkCast(dynamic_cast_type->g_type, exp->g_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }

    exp->g_type = dynamic_cast_type->g_type;
    return exp;
}

void Utils::pAddStateScope(int ln)
{
    symbol_table.addScope(SCOPE_STATEMENT);
}

TypePtr Utils::pReturnType(int ln)
{
    return make_shared<CType>(VOID_TYPE);
}

TypePtr Utils::pBool(int ln)
{
    return std::make_shared<CType>(BOOL_TYPE);
}

void Utils::pAddWhileScope(int ln)
{
    symbol_table.addScope(SCOPE_WHILE);
}

SymListPtr Utils::pFormals(Tptr formals, int ln)
{

    std::shared_ptr<SimpleSymbolList> fp = make_shared<SimpleSymbolList>();
    for (auto sym = std::dynamic_pointer_cast<SimpleSymbolList>(formals)->syms_list.rbegin();
         sym != std::dynamic_pointer_cast<SimpleSymbolList>(formals)->syms_list.rend(); sym++)
    {
        fp->syms_list.push_back(*sym);
    }
    return fp;
}

Tptr Utils::pCallExpression(Tptr call_exp, int ln)
{
    return call_exp;
}

void Utils::pPopScope(int ln)
{
    symbol_table.deleteScope();
}

TypePtr Utils::pAuto(int ln)
{
    return std::make_shared<CType>(AUTO_TYPE);
}

void Utils::parseStateRetExpression(Tptr exp, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = (!semantic_checks.checkReturn(exp->g_type)) || (semantic_checks.checkVoid(exp->g_type));
    if (check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
}

Tptr Utils::pParen(Tptr exp, int ln)
{
    return exp;
}

Tptr Utils::pBinaryOp(Tptr exp1, Tptr exp2, int ln)
{
    if (semantic_checks.checkFunction(exp1->g_type))
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }

    if (semantic_checks.checkFunction(exp2->g_type))
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp2);
        handleErrorIsUndefined(ln, cast_function->n);
    }

    Type ret_type = semantic_checks.checkBinop(exp1->g_type, exp2->g_type);
    if (ret_type == OTHER_TYPE)
    {
        handleErrorDoesNotMatch(ln);
    }
    if (ret_type == exp1->g_type) {
        return exp1;
    }
    return exp2;
}

Tptr Utils::pId(Tptr id, int ln)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<StringType>(id);

    if (!symbol_table.checkIfSymbolDefined(dynamic_cast_string->token))
    {
        handleErrorIsUndefined(ln, dynamic_cast_string->token);
    }
    return symbol_table.retDefinedSym(dynamic_cast_string->token);
}

void Utils::parseStateBreak(int ln)
{
    bool check_semantics = semantic_checks.checkBreak();
    if (!check_semantics)
    {
        handleErrorBreak(ln);
    }
}

BoolTypePtr Utils::pFalse(int ln)
{

    return make_shared<BoolType>(false);
}

Tptr Utils::pOr(Tptr bool_exp1, Tptr bool_exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp1->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(bool_exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(bool_exp2->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(bool_exp2);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkBool(bool_exp1->g_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
    check_semantics = semantic_checks.checkBool(bool_exp2->g_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }

    return bool_exp1;
}

BoolTypePtr Utils::pRelOprator(Tptr exp1, Tptr exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp1->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(exp2->g_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<FuncSymType>(exp2);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkRelop(exp1->g_type, exp2->g_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
    return make_shared<BoolType>(true);
}

void Utils::parseStateContinue(int ln)
{
    bool check_semantics = semantic_checks.checkContinue();
    if (!check_semantics)
    {
        handleErrorInContinue(ln);
    }
}

Tptr Utils::pCall(Tptr id, Tptr exp_list, int ln)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<StringType>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }
    SymPtr symbol_from_id = symbol_table.retDefinedSym(dyn_cast_id->token);

    if (!semantic_checks.checkFunction(symbol_from_id->g_type))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }
    std::shared_ptr<FuncSymType> dyn_cast_func = std::dynamic_pointer_cast<FuncSymType>(symbol_from_id);
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<TExpList>(exp_list);

    auto reversed_exp_list = make_shared<TExpList>();
    for (auto exp = dynamic_cast_exp_list->expression_list.rbegin();
         exp != dynamic_cast_exp_list->expression_list.rend(); exp++)
    {

        reversed_exp_list->expression_list.push_back(*exp);
    }

    if (!semantic_checks.checkCall(dyn_cast_func, reversed_exp_list))
    {
        std::vector<std::string> expected_args;
        for (auto sym : dyn_cast_func->parameters)
        {
            expected_args.push_back(TypeToString(sym.g_type));
        }
        handleErrorPrototypeMismatch(ln, dyn_cast_id->token, expected_args);
    }

    std::shared_ptr<TermianlBase> ret_type_pointer = make_shared<TermianlBase>(dyn_cast_func->ret_type);
    return ret_type_pointer;
}

Tptr Utils::pCall(Tptr id, int ln)
{
    std::shared_ptr<StringType> dyn_cast_id = std::dynamic_pointer_cast<StringType>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }

    SymPtr symbol_from_id = symbol_table.retDefinedSym(dyn_cast_id->token);

    if (!semantic_checks.checkFunction(symbol_from_id->g_type))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }

    std::shared_ptr<FuncSymType> dynamic_cast_func = std::dynamic_pointer_cast<FuncSymType>(symbol_from_id);
    auto empty_exp_list = make_shared<TExpList>();

    if (!semantic_checks.checkCall(dynamic_cast_func, empty_exp_list))
    {
        std::vector<std::string> expected_args;
        for (SimpleSymbol symbol : dynamic_cast_func->parameters)
        {
            expected_args.push_back(TypeToString(symbol.g_type));
        }
        handleErrorPrototypeMismatch(ln, dyn_cast_id->token, expected_args);
    }

    return make_shared<TermianlBase>(dynamic_cast_func->ret_type);
}

NumberTypePtr Utils::pNum(Tptr num, int ln)
{

    return std::dynamic_pointer_cast<NumberType>(num);
}

Tptr Utils::pNumB(Tptr num, int ln)
{
    std::shared_ptr<NumberType> dynamic_cast_num = std::dynamic_pointer_cast<NumberType>(num);

    if (!semantic_checks.checkOFByte(dynamic_cast_num->token))
    {
        handleErrorByteIsTooBig(ln, to_string(dynamic_cast_num->token));
    }
    num->g_type = BYTE_TYPE;
    return num;
}

SymListPtr Utils::pFormals(int ln)
{
    return make_shared<SimpleSymbolList>();
}

Tptr Utils::pExpressionList(Tptr exp, int ln)
{
    std::shared_ptr<TExpList> exp_list_pointer = make_shared<TExpList>();
    exp_list_pointer->expression_list.push_back(*exp);
    return exp_list_pointer;
}

Tptr Utils::pReturnType(Tptr type, int ln)
{

    return type;
}

SymListPtr Utils::pFormList(Tptr f, int ln)
{
    std::shared_ptr<SimpleSymbolList> arg_list_pointer = make_shared<SimpleSymbolList>();
    arg_list_pointer->syms_list.push_back(*std::dynamic_pointer_cast<SimpleSymbol>(f));
    return arg_list_pointer;
}

void Utils::pCheckBool(Tptr bool_exp, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp->g_type);
    if (check_semantics)
    {
        std::shared_ptr<FuncSymType> cast_function = std::dynamic_pointer_cast<FuncSymType>(bool_exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkBool(bool_exp->g_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
}