#include "parse_utils.h"

#include <memory>

Utils::Utils() : symbol_table(), semantic_checks(symbol_table){}

Utils &Utils::instance()
{
    static Utils pu;
    return pu;
}
 
void Utils::pProgram( int ln)
{
    if (semantic_checks.checkMainIsDefined())
    {
       symbol_table.PopScope();
    }else{
     errorMissingMain();
        exit(0);
        }
    
}

Tptr Utils::parseFunctionDef(Tptr return_type, Tptr id, Tptr formals, int lnno)
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

Tptr Utils::pExpressionList(Tptr exp, Tptr exp_list, int ln)
{
    auto dynamic_cast_exp_list = std::dynamic_pointer_cast<STypeExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.push_back(*exp);
    return dynamic_cast_exp_list;
}

SymbolPtr Utils::ParseFormalDecl( Tptr type, Tptr id, int ln)
{

    auto symbol_pointer = make_shared<SimpleSymbol>(std::dynamic_pointer_cast<STypeString>(id)->token, symbol_table.scope_stack.top()->offset,
                                                    std::dynamic_pointer_cast<STypeCType>(type)->general_type);
    return symbol_pointer;
}

void Utils::parseStateType( Tptr type, Tptr id, int ln)
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

TypePtr Utils::pInt(int ln)
{
    return std::make_shared<STypeCType>(INT_TYPE);
}

TypePtr Utils::pByte(int ln)
{
    return std::make_shared<STypeCType>(BYTE_TYPE);
}

StringTypePtr Utils::pString( Tptr stype_string, int ln)
{
    auto dynamic_cast_string = std::dynamic_pointer_cast<STypeString>(stype_string);

    return dynamic_cast_string;
}

BoolTypePtr Utils::pTrue(int ln)
{
    auto bool_pointer = make_shared<STypeBool>(true);
    return bool_pointer;
}

SymListPtr Utils::pFormList( Tptr formal, Tptr formals_list, int ln)
{
    auto dyn_cast_formal = std::dynamic_pointer_cast<SimpleSymbol>(formal);
    auto dyn_cast_formal_list = std::dynamic_pointer_cast<SimpleSymbolList>(formals_list);
    dyn_cast_formal_list->symbols_list.push_back(*dyn_cast_formal);
    return dyn_cast_formal_list;
}

void Utils::parseStateTypeAssignment( Tptr type, Tptr id, Tptr exp, int ln)
{
    // std::cout << "[DEBUG] start parseStateTypeAssignment" << std::endl;
    // std::cout << "[DEBUG] type: " << type->general_type << std::endl;
    // std::cout << "[DEBUG] exp: " << exp->general_type << std::endl;
    bool check_semantics;
    auto dynamic_cast_id = std::dynamic_pointer_cast<STypeString>(id);
    if (type->general_type == AUTO_TYPE) {
        type->general_type = exp->general_type;
        // std::cout << "[DEBUG] changed type: " << type->general_type << std::endl;
    }
    TypePtr dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);
    // std::cout << dynamic_cast_type << std::endl;
    if (type->general_type == AUTO_TYPE) {
        // std::cout << "[DEBUG] id token : " << dynamic_cast_id->token <<std::endl;
        // std::cout << "[DEBUG] found auto type!" <<std::endl;
        // std::cout << "[DEBUG] Casting to: " << exp->general_type <<std::endl;
        // std::cout << "[DEBUG] dynamic_cast_type: " << dynamic_cast_type->general_type <<std::endl;
    }
    // std::cout << "[DEBUG] dynamic_cast_id->general_type: " << dynamic_cast_id->general_type <<std::endl;
    check_semantics = semantic_checks.checkFunction(exp->general_type);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside IsFunctionType " << std::endl;
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    // std::cout << "[DEBUG] 1 " << std::endl;
    if (type->general_type != AUTO_TYPE) {
        check_semantics = semantic_checks.checkAssigned(dynamic_cast_type->general_type, exp->general_type);
        if (!check_semantics)
        {
            // std::cout << "[DEBUG] inside IsLegalAssignTypes " << std::endl;
            errorDoesNotMatch(ln);
            exit(0);
        }
    }
    // std::cout << "[DEBUG] 2 " << std::endl;
    check_semantics = semantic_checks.checkSymbolDefined(dynamic_cast_id->token);
    if (check_semantics)
    {
        // std::cout << "[DEBUG] inside checkSymbolDefined " << std::endl;
        errorInDefintion(ln, dynamic_cast_id->token);
        exit(0);
    }
    const auto symbol = make_shared<SimpleSymbol>(dynamic_cast_id->token, 0, dynamic_cast_type->general_type);
    // std::cout << "[DEBUG] Adding Variable... " << std::endl;
    symbol_table.AddVariable(symbol);
}

Tptr Utils::pNot( Tptr bool_exp, int ln)
{
    // exp can be a bool literal or an id
    if (semantic_checks.checkFunction(bool_exp->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }

    if (!semantic_checks.checkBool(bool_exp->general_type))
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    return bool_exp;
}

Tptr Utils::pAnd( Tptr bool_exp1, Tptr bool_exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkFunction(bool_exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkBool(bool_exp1->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    check_semantics= semantic_checks.checkBool(bool_exp2->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    return bool_exp1;
}

void Utils::parseStateAssignment( Tptr id, Tptr exp, int ln)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    bool check_semantics  = semantic_checks.checkSymbolDefined(dyn_cast_id->token);
    if (!check_semantics)
    {
        errorIsUndefined(ln, dyn_cast_id->token);
        exit(0);
    }

    auto sym_from_id = symbol_table.GetDefinedSymbol(dyn_cast_id->token);
    check_semantics = semantic_checks.checkFunction(exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkFunction(sym_from_id->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(sym_from_id);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkAssigned(sym_from_id->general_type, exp->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
}

void Utils::parseStateRet(int ln)
{
    bool check_semantics = semantic_checks.checkReturn(VOID_TYPE);
    if (!check_semantics){
        errorDoesNotMatch(ln);
        exit(0);
    }
}

Tptr Utils::pCast( Tptr type, Tptr exp, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp->general_type);
    auto dynamic_cast_type = std::dynamic_pointer_cast<STypeCType>(type);

    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkCast(dynamic_cast_type->general_type, exp->general_type);
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
    symbol_table.PushScope(SCOPE_WHILE);
}

SymListPtr Utils::pFormals(Tptr formals, int ln)
{

    auto fp = make_shared<SimpleSymbolList>();
    for (auto sym = std::dynamic_pointer_cast<SimpleSymbolList>(formals)->symbols_list.rbegin();
         sym != std::dynamic_pointer_cast<SimpleSymbolList>(formals)->symbols_list.rend(); sym++)
    {

        fp->symbols_list.push_back(*sym);
    }
    return fp;
}

Tptr Utils::pCallExpression(Tptr call_exp, int ln)
{
    return call_exp;
}

void Utils::pPopScope(int ln)
{
    symbol_table.PopScope();
}

TypePtr Utils::pAuto(int ln) {
    return std::make_shared<STypeCType>(AUTO_TYPE);
}

void Utils::parseStateRetExpression(Tptr exp, int ln)
{
     bool check_semantics = semantic_checks.checkFunction(exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = (!semantic_checks.checkReturn(exp->general_type)) || (semantic_checks.checkVoid(exp->general_type));
    if (check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
}

Tptr Utils::pParen(Tptr exp, int ln)
{
    return exp;
}

Tptr Utils::pBinaryOp( Tptr exp1, Tptr exp2, int ln)
{
    if (semantic_checks.checkFunction(exp1->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }

    if (semantic_checks.checkFunction(exp2->general_type))
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }

    if (semantic_checks.checkBinop(exp1->general_type, exp2->general_type) == OTHER_TYPE)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    return exp1;
}

Tptr Utils::pId(Tptr id, int ln)
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

void Utils::parseStateBreak(int ln)
{
    bool check_semantics=semantic_checks.checkBreak();
    if (!check_semantics)
    {
        errorBreak(ln);
        exit(0);
    }
}

BoolTypePtr Utils::pFalse(int ln)
{
    auto bool_pointer = make_shared<STypeBool>(false);
    return bool_pointer;
}

Tptr Utils::pOr(Tptr bool_exp1, Tptr bool_exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkFunction(bool_exp2->general_type); 
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkBool(bool_exp1->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    check_semantics = semantic_checks.checkBool(bool_exp2->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }

    return bool_exp1;
}

BoolTypePtr Utils::pRelOprator(Tptr exp1, Tptr exp2, int ln)
{
    bool check_semantics = semantic_checks.checkFunction(exp1->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp1);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkFunction(exp2->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(exp2);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics = semantic_checks.checkRelop(exp1->general_type, exp2->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
    return make_shared<STypeBool>(true);
}

void Utils::parseStateContinue(int ln)
{
    bool check_semantics=semantic_checks.checkContinue();
    if (!check_semantics)
    {
        errorContinue(ln);
        exit(0);
    }
}

Tptr Utils::pCall(Tptr id, Tptr exp_list, int ln)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dyn_cast_id->token);

    if (!semantic_checks.checkFunction(symbol_from_id->general_type))
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

    if (!semantic_checks.checkCall(dyn_cast_func, reversed_exp_list))
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

Tptr Utils::pCall(Tptr id, int ln)
{
    auto dyn_cast_id = std::dynamic_pointer_cast<STypeString>(id);

    if (!semantic_checks.checkSymbolDefined(dyn_cast_id->token))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }

    auto symbol_from_id = symbol_table.GetDefinedSymbol(dyn_cast_id->token);

    if (!semantic_checks.checkFunction(symbol_from_id->general_type))
    {
        errorUndefinedFunction(ln, dyn_cast_id->token);
        exit(0);
    }

    auto dynamic_cast_func = std::dynamic_pointer_cast<STypeFunctionSymbol>(symbol_from_id);
    auto empty_exp_list = make_shared<STypeExpList>();

    if (!semantic_checks.checkCall(dynamic_cast_func, empty_exp_list))
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

NumberTypePtr Utils::pNum(Tptr num, int ln)
{
    auto dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    return dynamic_cast_num;
}

Tptr Utils::pNumB(Tptr num,  int ln)
{
    auto dynamic_cast_num = std::dynamic_pointer_cast<STypeNumber>(num);

    if (!semantic_checks.checkOFByte(dynamic_cast_num->token))
    {
        errorByteIsTooBig(ln, to_string(dynamic_cast_num->token));
        exit(0);
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
    auto exp_list_pointer = make_shared<STypeExpList>();
    exp_list_pointer->exp_list.push_back(*exp);
    return exp_list_pointer;
}

Tptr Utils::pReturnType(Tptr type, int ln)
{
    return type;
}

SymListPtr Utils::pFormList(Tptr formal,  int ln)
{
    auto arg_list_pointer = make_shared<SimpleSymbolList>();
    arg_list_pointer->symbols_list.push_back(*std::dynamic_pointer_cast<SimpleSymbol>(formal));
    return arg_list_pointer;
}

void Utils::pCheckBool(Tptr bool_exp,  int ln)
{
    bool check_semantics = semantic_checks.checkFunction(bool_exp->general_type);
    if (check_semantics)
    {
        auto cast_function = std::dynamic_pointer_cast<STypeFunctionSymbol>(bool_exp);
        errorIsUndefined(ln, cast_function->name);
        exit(0);
    }
    check_semantics =  semantic_checks.checkBool(bool_exp->general_type);
    if (!check_semantics)
    {
        errorDoesNotMatch(ln);
        exit(0);
    }
}