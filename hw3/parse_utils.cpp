#include "parse_utils.h"

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
        symbol_table.removeScope();
    }
    else
    {
        handleErrorInMissingMain(ln);
    }
}

Tptr Utils::parseFunctionDef(Tptr return_type, Tptr id, Tptr formals, int lnno)
{

    STypeFunctionSymbolPtr func_sym = make_shared<STypeFunctionSymbol>(std::dynamic_pointer_cast<STypeString>(id)->token, std::dynamic_pointer_cast<STypeCType>(return_type)->general_type,
                                                                       std::dynamic_pointer_cast<SimpleSymbolList>(formals)->syms_list);
    if (symbol_table.checkIfSymbolDefined(std::dynamic_pointer_cast<STypeString>(id)->token))
    {
        handleErrorInDefintion(lnno, std::dynamic_pointer_cast<STypeString>(id)->token);
    }
    else
    {
        symbol_table.AddFunc(func_sym);
        symbol_table.addFunctionScope(SCOPE_FUNCTION, std::dynamic_pointer_cast<STypeCType>(return_type)->general_type, func_sym);
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
    std::shared_ptr<STypeExpList> dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);

    dynamic_cast_exp_list->expression_list.push_back(*exp);
    return dynamic_cast_exp_list;
}

SymbolPtr Utils::ParseFormalDecl(Tptr type, Tptr id, int ln)
{

    std::shared_ptr<SimpleSymbol> symbol_pointer = make_shared<SimpleSymbol>(std::dynamic_pointer_cast<STypeString>(id)->token, symbol_table.s_stack.top()->offset,
                                                                             std::dynamic_pointer_cast<STypeCType>(type)->general_type);
    return symbol_pointer;
}

void Utils::parseStateType(Tptr type, Tptr id, int ln)
{
    std::shared_ptr<STypeCType> dyn_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    std::shared_ptr<STypeString> dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);
    const auto symbol = make_shared<SimpleSymbol>(dyn_cast_id->token, 0, dyn_cast_type->general_type);

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
    return std::make_shared<STypeCType>(INT_TYPE);
}

TypePtr Utils::pByte(int ln)
{
    return std::make_shared<STypeCType>(BYTE_TYPE);
}

StringTypePtr Utils::pString(Tptr stype_string, int ln)
{
    return std::dynamic_pointer_cast<STypeString>(stype_string);
}

BoolTypePtr Utils::pTrue(int ln)
{

    return make_shared<STypeBool>(true);
}

SymListPtr Utils::pFormList(Tptr formal, Tptr formals_list, int ln)
{
    auto dyn_cast_formal_list = std::dynamic_pointer_cast<SimpleSymbolList>(formals_list);
    auto dyn_cast_formal = std::dynamic_pointer_cast<SimpleSymbol>(formal);
    dyn_cast_formal_list->syms_list.push_back(*dyn_cast_formal);
    return dyn_cast_formal_list;
}

void Utils::parseAutoTypeAssignment(Tptr type, Tptr id, Tptr exp, int ln)
{
    // std::cout << "[DEBUG] start parseAutoTypeAssignment" << std::endl;
    // std::cout << "[DEBUG] type: " << type->general_type << std::endl;
    // std::cout << "[DEBUG] exp: " << exp->general_type << std::endl;
    bool check_semantics;
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);
    if (exp->general_type != INT_TYPE && exp->general_type != BYTE_TYPE && exp->general_type != BOOL_TYPE)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    type->general_type = exp->general_type;
    TypePtr dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    // std::cout << dynamic_cast_type << std::endl;
    // std::cout << "[DEBUG] id token : " << dynamic_cast_id->token <<std::endl;
    // std::cout << "[DEBUG] found auto type!" <<std::endl;
    // std::cout << "[DEBUG] Casting to: " << exp->general_type <<std::endl;
    // std::cout << "[DEBUG] dynamic_cast_type: " << dynamic_cast_type->general_type <<std::endl;
    // std::cout << "[DEBUG] dynamic_cast_id->general_type: " << dynamic_cast_id->general_type <<std::endl;
    check_semantics = semantic_checks.checkFunction(exp->general_type);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside IsFunctionType " << std::endl;
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    // std::cout << "[DEBUG] 2 " << std::endl;
    check_semantics = semantic_checks.checkSymbolDefined(dynamic_cast_id->token);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside checkSymbolDefined " << std::endl;
        handleErrorInDefintion(ln, dynamic_cast_id->token);
    }
    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    // std::cout << "[DEBUG] Adding Variable... " << std::endl;
    symbol_table.AddVar(symbol);
}

void Utils::parseStateTypeAssignment(Tptr type, Tptr id, Tptr exp, int ln)
{
    bool check_semantics;
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);
    TypePtr dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    // std::cout << dynamic_cast_type << std::endl;
    check_semantics = semantic_checks.checkFunction(exp->general_type);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside IsFunctionType " << std::endl;
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    // std::cout << "[DEBUG] 1 " << std::endl;
    check_semantics = semantic_checks.checkAssigned(dynamic_cast_type->general_type, exp->general_type);
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
    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    // std::cout << "[DEBUG] Adding Variable... " << std::endl;
    symbol_table.AddVar(symbol);
}

Tptr Utils::pNot(Tptr bool_exp, int ln)
{

    if (semantic_checks.checkFunction(bool_exp->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }

    if (!semantic_checks.checkBool(bool_exp->general_type))
    {
        handleErrorDoesNotMatch(ln);
    }

    return bool_exp;
}

Tptr Utils::pAnd(Tptr bool_exp1, Tptr bool_exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(bool_exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
    }
    bool check_semantics1 = semantic_checks.checkBool(bool_exp1->general_type), check_semantics2 = semantic_checks.checkBool(bool_exp2->general_type);
    if (!check_semantics1 || !check_semantics2)
    {
        handleErrorDoesNotMatch(ln);
    }

    return bool_exp1;
}

void Utils::parseStateAssignment(Tptr id, Tptr exp, int ln)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    bool check_semantics = semantic_checks.checkSymbolDefined(dyn_cast_id->token);
    if (!check_semantics)
    {
        handleErrorIsUndefined(ln, dyn_cast_id->token);
    }
    check_semantics = semantic_checks.checkFunction(exp->general_type);
    auto sym_from_id = symbol_table.retDefinedSym(dyn_cast_id->token);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(sym_from_id->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(sym_from_id);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkAssigned(sym_from_id->general_type, exp->general_type);
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
    bool check_semantics = semantic_checks.checkFunction(exp->general_type);
    std::shared_ptr<STypeCType> dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);

    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkCast(dynamic_cast_type->general_type, exp->general_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }

    exp->general_type = dynamic_cast_type->general_type;
    return exp;
}

void Utils::pAddStateScope(int ln)
{
    symbol_table.addScope(SCOPE_STATEMENT);
}

TypePtr Utils::pReturnType(int ln)
{
    return make_shared<STypeCType>(VOID_TYPE);
}

TypePtr Utils::pBool(int ln)
{
    return std::make_shared<STypeCType>(BOOL_TYPE);
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
    symbol_table.removeScope();
}

TypePtr Utils::pAuto(int ln)
{
    return std::make_shared<STypeCType>(AUTO_TYPE);
}

void Utils::parseStateRetExpression(Tptr exp, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = (!semantic_checks.checkReturn(exp->general_type)) || (semantic_checks.checkVoid(exp->general_type));
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
    if (semantic_checks.checkFunction(exp1->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }

    if (semantic_checks.checkFunction(exp2->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        handleErrorIsUndefined(ln, cast_function->n);
    }

    if (semantic_checks.checkBinop(exp1->general_type, exp2->general_type) == OTHER_TYPE)
    {
        handleErrorDoesNotMatch(ln);
    }
    return exp1;
}

Tptr Utils::pId(Tptr id, int ln)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<STypeString>(id);

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

    return make_shared<STypeBool>(false);
}

Tptr Utils::pOr(Tptr bool_exp1, Tptr bool_exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(bool_exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkBool(bool_exp1->general_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
    check_semantics = semantic_checks.checkBool(bool_exp2->general_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }

    return bool_exp1;
}

BoolTypePtr Utils::pRelOprator(Tptr exp1, Tptr exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkFunction(exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkRelop(exp1->general_type, exp2->general_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
    return make_shared<STypeBool>(true);
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
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }
    SymbolPtr symbol_from_id = symbol_table.retDefinedSym(dyn_cast_id->token);

    if (!semantic_checks.checkFunction(symbol_from_id->general_type))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }
    std::shared_ptr<STypeFunctionSymbol> dyn_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);

    auto reversed_exp_list = make_shared<STypeExpList>();
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
            expected_args.push_back(TypeToString(sym.general_type));
        }
        handleErrorPrototypeMismatch(ln, dyn_cast_id->token, expected_args);
    }

    std::shared_ptr<TermianlBase> ret_type_pointer = make_shared<TermianlBase>(dyn_cast_func->ret_type);
    return ret_type_pointer;
}

Tptr Utils::pCall(Tptr id, int ln)
{
    std::shared_ptr<STypeString> dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }

    SymbolPtr symbol_from_id = symbol_table.retDefinedSym(dyn_cast_id->token);

    if (!semantic_checks.checkFunction(symbol_from_id->general_type))
    {
        handleErrorUndefinedFunction(ln, dyn_cast_id->token);
    }

    std::shared_ptr<STypeFunctionSymbol> dynamic_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto empty_exp_list = make_shared<STypeExpList>();

    if (!semantic_checks.checkCall(dynamic_cast_func, empty_exp_list))
    {
        std::vector<std::string> expected_args;
        for (SimpleSymbol symbol : dynamic_cast_func->parameters)
        {
            expected_args.push_back(TypeToString(symbol.general_type));
        }
        handleErrorPrototypeMismatch(ln, dyn_cast_id->token, expected_args);
    }

    return make_shared<TermianlBase>(dynamic_cast_func->ret_type);
}

NumberTypePtr Utils::pNum(Tptr num, int ln)
{

    return std::dynamic_pointer_cast<STypeNumber>(num);
}

Tptr Utils::pNumB(Tptr num, int ln)
{
    std::shared_ptr<STypeNumber> dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    if (!semantic_checks.checkOFByte(dynamic_cast_num->token))
    {
        handleErrorByteIsTooBig(ln, to_string(dynamic_cast_num->token));
    }
    num->general_type = BYTE_TYPE;
    return num;
}

SymListPtr Utils::pFormals(int ln)
{
    return make_shared<SimpleSymbolList>();
}

Tptr Utils::pExpressionList(Tptr exp, int ln)
{
    std::shared_ptr<STypeExpList> exp_list_pointer = make_shared<STypeExpList>();
    exp_list_pointer->expression_list.push_back(*exp);
    return exp_list_pointer;
}

Tptr Utils::pReturnType(Tptr type, int ln)
{

    return type;
}

SymListPtr Utils::pFormList(Tptr formal, int ln)
{
    std::shared_ptr<SimpleSymbolList> arg_list_pointer = make_shared<SimpleSymbolList>();
    arg_list_pointer->syms_list.push_back(*std::dynamic_pointer_cast<SimpleSymbol>(formal));
    return arg_list_pointer;
}

void Utils::pCheckBool(Tptr bool_exp, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp->general_type);
    if (check_semantics)
    {
        std::shared_ptr<STypeFunctionSymbol> cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        handleErrorIsUndefined(ln, cast_function->n);
    }
    check_semantics = semantic_checks.checkBool(bool_exp->general_type);
    if (!check_semantics)
    {
        handleErrorDoesNotMatch(ln);
    }
}