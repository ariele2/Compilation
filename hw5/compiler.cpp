#include "compiler.h"

#include <memory>

void Compiler::handleErrorLex(int ln)
{
    errorLex(ln);
    exit(0);
}
void Compiler::handleErrorSyn(int ln)
{
    errorSyn(ln);
    exit(0);
}
void Compiler::handleErrorUndef(int ln, const string &identification)
{
    errorUndef(ln, identification);
    exit(0);
}
void Compiler::handleErrorDef(int ln, const string &identification)
{
    errorDef(ln, identification);
    exit(0);
}
void Compiler::handleErrorUndefFunc(int ln, const string &identification)
{
    errorUndefFunc(ln, identification);
    exit(0);
}
void Compiler::handleErrorMismatch(int ln)
{
    errorMismatch(ln);
    exit(0);
}
void Compiler::handleErrorPrototypeMismatch(int ln, const string &identification, vector<string> &arguments_types)
{
    errorPrototypeMismatch(ln, identification, arguments_types);
    exit(0);
}
void Compiler::handleErrorUnexpectedBreak(int ln)
{
    errorUnexpectedBreak(ln);
    exit(0);
}
void Compiler::handleErrorUnexpectedContinue(int ln)
{
    errorUnexpectedContinue(ln);
    exit(0);
}
void Compiler::handleErrorMainMissing()
{
    errorMainMissing();
    exit(0);
}
void Compiler::handleErrorByteTooLarge(int ln, const string &val)
{
    errorByteTooLarge(ln, val);
    exit(0);
}

Compiler::Compiler() : sym_tab(), validations(sym_tab), code_genreation(validations)
{
}

Compiler &Compiler::instance()
{
    static Compiler comp;
    return comp;
}

void Compiler::pProgram(int ln)
{
    if (validations.CheckMainIsDefined())
    {
        sym_tab.PopScope();
        code_genreation.addProg();
    }
    else {
        handleErrorMainMissing();
    }
}

void Compiler::pFuncs(int ln)
{
}

void Compiler::pFuncHead( const BaseTypePtr &type_returned, const BaseTypePtr &identification, const BaseTypePtr &list_of_formals, int ln)
{
    auto identification_dyn_cast = dynamic_pointer_cast<StringType>(identification);

    if (!sym_tab.IsSymbolDefined(identification_dyn_cast->token))
    {
        auto func_sym = make_shared<FuncSymType>(identification_dyn_cast->token, dynamic_pointer_cast<CType>(type_returned)->generation_type,
                                                        dynamic_pointer_cast<ArgListType>(list_of_formals)->arguments_list);
        sym_tab.AddFunction(func_sym);
        sym_tab.PushFunctionScope(dynamic_pointer_cast<CType>(type_returned)->generation_type);
        for (const auto &param : func_sym->params)
        {
            auto param_symbol = make_shared<SymbolType>(param);
            if (!validations.CheckSymDefined(param_symbol->name))
            {
                sym_tab.AddParam(param_symbol);
            }
            else
            {
                handleErrorDef(ln, param_symbol->name);
            }
        }

        code_genreation.addFunctionHead(func_sym);
    }
    else
    {
        handleErrorDef(ln, identification_dyn_cast->token);
    }
}
// no
void Compiler::pFuncDecl( const BaseTypePtr &stats, const BaseTypePtr &n_label, int ln)
{
    code_genreation.addFuncDeclaration(stats, n_label);
}
// no
BaseTypePtr Compiler::pRetType( BaseTypePtr t, int ln)
{
    return t;
}

CTypePtr Compiler::pRetType(int ln)
{
    return make_shared<CType>(VOID_TYPE);
}

ArgListTypePtr Compiler::pFs(int ln)
{

    return make_shared<ArgListType>();
}

ArgListTypePtr Compiler::pFs( const BaseTypePtr &formals_list, int ln)
{

    auto pointer_to_formals_list = make_shared<ArgListType>();
    for (auto i = dynamic_pointer_cast<ArgListType>(formals_list)->arguments_list.rbegin();
         i != dynamic_pointer_cast<ArgListType>(formals_list)->arguments_list.rend(); i++)
    {

        pointer_to_formals_list->arguments_list.push_back(*i);
    }
    return pointer_to_formals_list;
}

ArgListTypePtr Compiler::pFsList( const BaseTypePtr &f, int ln)
{

    auto arg_list_pointer = make_shared<ArgListType>();
    arg_list_pointer->arguments_list.push_back(*dynamic_pointer_cast<SymbolType>(f));
    return arg_list_pointer;
}

ArgListTypePtr Compiler::pFsList( const BaseTypePtr &f, const BaseTypePtr &formals_list, int ln)
{
    auto dyn_cast_formals = dynamic_pointer_cast<ArgListType>(formals_list);
    dyn_cast_formals->arguments_list.push_back(*dynamic_pointer_cast<SymbolType>(f));
    return dyn_cast_formals;
}

SymbolTypePtr Compiler::pFDecl( const BaseTypePtr &t, const BaseTypePtr &identification, int ln)
{
    auto symbol_pointer = make_shared<SymbolType>(dynamic_pointer_cast<StringType>(identification)->token, sym_tab.scope_stack.top()->offset,
                                                  dynamic_pointer_cast<CType>(t)->generation_type);
    return symbol_pointer;
}

StatementTypePtr Compiler::pStat( BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement, int ln)
{
    auto dynamic_cast_statement = dynamic_pointer_cast<StatementType>(statement);

    dynamic_cast_statement->next_list = Buff::merge(dynamic_cast_statement->next_list,
                                                          dynamic_pointer_cast<StatementType>(next_list_as_statement)->next_list);

    return dynamic_cast_statement;
}

StatementTypePtr Compiler::pStat( const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
                                 const BaseTypePtr &next_label,
                                 const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement, int ln)
{

    auto dynamic_cast_next_statement = dynamic_pointer_cast<StatementType>(next_statement);

    code_genreation.buff.bpatch(dynamic_pointer_cast<StatementType>(statements)->next_list, dynamic_pointer_cast<StringType>(next_label)->token);
    code_genreation.buff.bpatch(dynamic_pointer_cast<StatementType>(old_next_list_as_statement)->next_list, dynamic_pointer_cast<StringType>(next_label)->token);

    dynamic_cast_next_statement->next_list = Buff::merge(dynamic_cast_next_statement->next_list,
                                                               dynamic_pointer_cast<StatementType>(my_next_list_as_statement)->next_list);

    return dynamic_cast_next_statement;
}

StatementTypePtr Compiler::pStatOfStats( BaseTypePtr &statements, int ln)
{
    return dynamic_pointer_cast<StatementType>(statements);
}

StatementTypePtr Compiler::pStatType( const BaseTypePtr &t, const BaseTypePtr &identification, int ln)
{

    if (!validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->token))
    {
        const auto sym = make_shared<SymbolType>(dynamic_pointer_cast<StringType>(identification)->token, 0, dynamic_pointer_cast<CType>(t)->generation_type);
        sym_tab.AddVariable(sym);

        return code_genreation.addStatType(dynamic_pointer_cast<StringType>(identification)->token);
    }
    else
    {
        handleErrorDef(ln, dynamic_pointer_cast<StringType>(identification)->token);
    }
}

StatementTypePtr
Compiler::pStatTypeAssign( const BaseTypePtr &t, const BaseTypePtr &identification, const BaseTypePtr &expression, int ln)
{

    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(identification);

    if (validations.CheckFunction(expression->generation_type))
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(expression);
        handleErrorUndef(ln, func_cast->name);
    }

    if (!validations.CheckAssigned(dynamic_pointer_cast<CType>(t)->generation_type, expression->generation_type))
    {
        handleErrorMismatch(ln);
    }
    if (!validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->token))
    {
        const auto sym = make_shared<SymbolType>(dynamic_pointer_cast<StringType>(identification)->token, 0, dynamic_pointer_cast<CType>(t)->generation_type);
        sym_tab.AddVariable(sym);

        return code_genreation.addStatAssign(dynamic_pointer_cast<StringType>(identification)->token, expression);
    }
    else
    {
        handleErrorDef(ln, dynamic_pointer_cast<StringType>(identification)->token);
    }
}

StatementTypePtr Compiler::pStatAssign( const BaseTypePtr &identification, const BaseTypePtr &expression, int ln)
{
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(identification);

    if (!validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->token))
    {
        handleErrorUndef(ln, dynamic_pointer_cast<StringType>(identification)->token);
        exit(0);
    }

    auto sym_retured_id = sym_tab.GetDefinedSymbol(dynamic_pointer_cast<StringType>(identification)->token);

    if (validations.CheckFunction(expression->generation_type))
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(expression);
        handleErrorUndef(ln, func_cast->name);
        exit(0);
    }

    if (validations.CheckFunction(sym_retured_id->generation_type))
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(sym_retured_id);
        handleErrorUndef(ln, func_cast->name);
        exit(0);
    }

    if (validations.CheckAssigned(sym_retured_id->generation_type, expression->generation_type))
    {

        return code_genreation.addStatAssign(dynamic_pointer_cast<StringType>(identification)->token, expression);
    }
    else
    {
        handleErrorMismatch(ln);
    }
}

StatementTypePtr Compiler::pStatCall(int ln)
{
    return Generator::addStatCall();
}

StatementTypePtr Compiler::pStatRet(int ln)
{
    if (validations.CheckReturn(VOID_TYPE))
    {
        return code_genreation.addStatRet();
    }
    else
    {
        handleErrorMismatch(ln);
    }
}

StatementTypePtr Compiler::pStatRetExp( const BaseTypePtr &expression, int ln)
{
    if (!validations.CheckFunction(expression->generation_type))
    {

        if (!((validations.CheckVoid(expression->generation_type) || (!validations.CheckReturn(expression->generation_type)))))
        {
            return code_genreation.addStatRetExpression(expression);
        }
        else
        {
            handleErrorMismatch(ln);
        }
    }
    else
    {
        handleErrorUndef(ln, dynamic_pointer_cast<FuncSymType>(expression)->name);
    }
}

StatementTypePtr Compiler::pStatIf( const BaseTypePtr &expression, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                   const BaseTypePtr &if_list_as_statement, int ln)
{
    return code_genreation.addStatIf(expression, if_label, if_statement, if_list_as_statement);
}

StatementTypePtr Compiler::pStatIfElse( const BaseTypePtr &expression, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                       BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
                                       const BaseTypePtr &else_statement, int ln)
{
    return code_genreation.addStatIfAndElse(expression, if_label, if_statement, move(if_list_as_statement), else_label,
                                        else_statement);
}
// chnage here
StatementTypePtr Compiler::pStatWhile( BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
                                      const BaseTypePtr &expression,
                                      const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                                      const BaseTypePtr &end_list_as_statement, int ln)
{

    assert(sym_tab.scope_stack.top()->scope_type == WHILE_SCOPE);
    auto break_list = sym_tab.scope_stack.top()->break_list;
    sym_tab.PopScope();

    return code_genreation.addStatWhile(move(start_list_as_statement), while_head_label, expression, while_body_label,
                                       while_statement,
                                       end_list_as_statement,
                                       break_list);
}


StatementTypePtr Compiler::pStatBreak(int ln)
{
    if (validations.CheckBreak())
    {
        return code_genreation.addStatBreak();
    }
    else
    {

        handleErrorUnexpectedBreak(ln);
    }
}

StatementTypePtr Compiler::pStatContinue(int ln)
{
    if (validations.CheckContinue())
    {
        return code_genreation.addStatContinue();
    }
    else
    {
        handleErrorUnexpectedContinue(ln);
    }
}

BaseTypePtr Compiler::pCall( const BaseTypePtr &identification, const BaseTypePtr &exp_list, int ln)
{

    if (validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->token))
    {
        auto sym_retured_id = sym_tab.GetDefinedSymbol(dynamic_pointer_cast<StringType>(identification)->token);

        if (validations.CheckFunction(sym_retured_id->generation_type))
        {
            auto dynamic_cast_exp_list = dynamic_pointer_cast<TExpList>(exp_list);
            auto dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(sym_retured_id);

            if (validations.CheckCall(dynamic_cast_func, dynamic_cast_exp_list))
            {
                return code_genreation.addCall(dynamic_pointer_cast<FuncSymType>(sym_retured_id), dynamic_pointer_cast<TExpList>(exp_list));
            }
            else
            {
                vector<string> expected_args;
                for (const auto &sym : dynamic_pointer_cast<FuncSymType>(sym_retured_id)->params)
                {
                    expected_args.push_back(TypeToString(sym.generation_type));
                }
                handleErrorPrototypeMismatch(ln, dynamic_pointer_cast<StringType>(identification)->token, expected_args);
            }
        }
        else
        {
            handleErrorUndefFunc(ln, dynamic_pointer_cast<StringType>(identification)->token);
        }
    }
    else
    {
        handleErrorUndefFunc(ln, dynamic_pointer_cast<StringType>(identification)->token);
    }
}

BaseTypePtr Compiler::pCall( const BaseTypePtr &identification, int ln)
{
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(identification);

    if (validations.CheckSymDefined(dynamic_cast_id->token))
    {
        auto sym_retured_id = sym_tab.GetDefinedSymbol(dynamic_cast_id->token);

        if (!validations.CheckFunction(sym_tab.GetDefinedSymbol(dynamic_cast_id->token)->generation_type))
        {
            handleErrorUndefFunc(ln, dynamic_cast_id->token);
            exit(0);
        }
        std::shared_ptr<TExpList> empty_exp_list = make_shared<TExpList>();
        std::shared_ptr<FuncSymType> dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(sym_tab.GetDefinedSymbol(dynamic_cast_id->token));

        if (validations.CheckCall(dynamic_cast_func, empty_exp_list))
        {
            return code_genreation.addCall(dynamic_pointer_cast<FuncSymType>(sym_tab.GetDefinedSymbol(dynamic_cast_id->token)));
        }
        else
        {
            vector<string> expected_args;
            for (const auto &sym : dynamic_pointer_cast<FuncSymType>(sym_tab.GetDefinedSymbol(dynamic_cast_id->token))->params)
            {
                expected_args.push_back(TypeToString(sym.generation_type));
            }
            handleErrorPrototypeMismatch(ln, dynamic_cast_id->token, expected_args);
        }
    }
    else
    {
        handleErrorUndefFunc(ln, dynamic_cast_id->token);
    }
}


BaseTypePtr Compiler::pExplist( const BaseTypePtr &expression, int ln)
{
    ExpListTypePtr exp_list_pointer = make_shared<TExpList>();

    exp_list_pointer->exp_list.insert(exp_list_pointer->exp_list.begin(), expression);
    return exp_list_pointer;
}

BaseTypePtr Compiler::pExplist( const BaseTypePtr &expression, const BaseTypePtr &exp_list, int ln)
{
    ExpListTypePtr dynamic_cast_exp_list = dynamic_pointer_cast<TExpList>(exp_list);

    dynamic_cast_exp_list->exp_list.insert(dynamic_cast_exp_list->exp_list.begin(), expression);
    return dynamic_cast_exp_list;
}

CTypePtr Compiler::pInt(int ln)
{
    auto val = make_shared<CType>(INT_TYPE);
    return val;
}

CTypePtr Compiler::pByte(int ln)
{

    auto val = make_shared<CType>(BYTE_TYPE);
    return val;
}

CTypePtr Compiler::pBool(int ln)
{
    auto val = make_shared<CType>(BOOL_TYPE);
    return val;
}

BaseTypePtr Compiler::pParen( BaseTypePtr expression, int ln)
{
    return expression;
}

RegisterTypePtr Compiler::pBinop( const BaseTypePtr &first_expression, BaseTypePtr &binop, const BaseTypePtr &second_expression, int ln)
{
    if (!validations.CheckFunction(first_expression->generation_type))
    {
        if (!validations.CheckFunction(second_expression->generation_type))
        {
            if (validations.CheckAndGetBinOpType(first_expression->generation_type, second_expression->generation_type) == ERROR_TYPE)
            {
                handleErrorMismatch(ln);
            }

            return code_genreation.addBinop(first_expression, dynamic_pointer_cast<StringType>(binop)->token, second_expression);
        }
        else
        {

            handleErrorUndef(ln, dynamic_pointer_cast<FuncSymType>(second_expression)->name);
        }
    }
    else
    {

        handleErrorUndef(ln, dynamic_pointer_cast<FuncSymType>(first_expression)->name);
    }
}

BaseTypePtr Compiler::pID( const BaseTypePtr &identification, int ln)
{

    if (sym_tab.IsSymbolDefined(dynamic_pointer_cast<StringType>(identification)->token))
    {

        return code_genreation.addIdentification(sym_tab.GetDefinedSymbol(dynamic_pointer_cast<StringType>(identification)->token));
    }
    else
    {
        handleErrorUndef(ln, dynamic_pointer_cast<StringType>(identification)->token);
    }
}
// change
BaseTypePtr Compiler::pCallExp( BaseTypePtr call_exp, int ln)
{
    return code_genreation.addCallExpression(move(call_exp));
}

NumberTypePtr Compiler::pNum( const BaseTypePtr &num, int ln)
{

    return dynamic_pointer_cast<NumberType>(num);
}

BaseTypePtr Compiler::pNumB( BaseTypePtr num, int ln)
{

    if (validations.CheckOFByte(dynamic_pointer_cast<NumberType>(num)->token))
    {
        num->generation_type = BYTE_TYPE;
        return num;
    }
    else
    {
        handleErrorByteTooLarge(ln, to_string(dynamic_pointer_cast<NumberType>(num)->token));
    }
}

// change here
RegisterTypePtr Compiler::pString( const BaseTypePtr &stype_string, int ln)
{

    return code_genreation.addString(stype_string);
}
// also
BoolExpTypePtr Compiler::pTrue(int ln)
{
    return code_genreation.addTrue();
}
// also
BoolExpTypePtr Compiler::pFalse(int ln)
{
    return code_genreation.addFalse();
}

BoolExpTypePtr Compiler::pNot(const BaseTypePtr &boolean_expression, int ln)
{

    if (!validations.CheckFunction(boolean_expression->generation_type))
    {
        if (validations.CheckBool(boolean_expression->generation_type))
        {
            return Generator::addNot(boolean_expression);
        }
        else
        {
            handleErrorMismatch(ln);
        }
    }
    else
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(boolean_expression);
        handleErrorUndef(ln, func_cast->name);
    }
}

BoolExpTypePtr
Compiler::pAnd( const BaseTypePtr &first_boolean_exppression, const BaseTypePtr &label_of_and, const BaseTypePtr &second_boolean_exppression, int ln)
{
    if (!validations.CheckFunction(first_boolean_exppression->generation_type))
    {
        if (validations.CheckFunction(second_boolean_exppression->generation_type))
        {

            handleErrorUndef(ln, dynamic_pointer_cast<FuncSymType>(second_boolean_exppression)->name);
        }

        if (validations.CheckBool(first_boolean_exppression->generation_type))
        {
            if (validations.CheckBool(second_boolean_exppression->generation_type))
            {
                return code_genreation.addAnd(first_boolean_exppression, label_of_and, second_boolean_exppression);
            }
            else
            {
                handleErrorMismatch(ln);
            }
        }
        else
        {
            handleErrorMismatch(ln);
        }
    }
    else
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(first_boolean_exppression);
        handleErrorUndef(ln, func_cast->name);
    }
}

BoolExpTypePtr
Compiler::pOr( const BaseTypePtr &first_boolean_exppression, const BaseTypePtr &label_of_or, const BaseTypePtr &second_boolean_exppression, int ln)
{
    if (!validations.CheckFunction(first_boolean_exppression->generation_type))
    {

        if (!validations.CheckFunction(second_boolean_exppression->generation_type))
        {
            if (validations.CheckBool(first_boolean_exppression->generation_type))
            {
                if (validations.CheckBool(second_boolean_exppression->generation_type))
                {
                    return code_genreation.addOr(first_boolean_exppression, label_of_or, second_boolean_exppression);
                }
                else
                {
                    handleErrorMismatch(ln);
                }
            }
            else
            {
                handleErrorMismatch(ln);
            }
        }
        else
        {
            auto func_cast = dynamic_pointer_cast<FuncSymType>(second_boolean_exppression);
            handleErrorUndef(ln, dynamic_pointer_cast<FuncSymType>(second_boolean_exppression)->name);
        }
    }
    else
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(first_boolean_exppression);
        handleErrorUndef(ln, func_cast->name);
    }
}

BoolExpTypePtr Compiler::pRelOp( const BaseTypePtr &first_expression, BaseTypePtr &relop, const BaseTypePtr &second_expression, int ln)
{
    if (!validations.CheckFunction(first_expression->generation_type))
    {
        if (validations.CheckFunction(second_expression->generation_type))
        {
            auto func_cast = dynamic_pointer_cast<FuncSymType>(second_expression);
            handleErrorUndef(ln, func_cast->name);
        }

        if (validations.CheckRelop(first_expression->generation_type, second_expression->generation_type))
        {
            return code_genreation.addRelOp(first_expression, relop, second_expression);
        }
        else
        {
            handleErrorMismatch(ln);
        }
    }
    else
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(first_expression);
        handleErrorUndef(ln, func_cast->name);
    }
}

void Compiler::pAddStatScope(int ln)
{
    sym_tab.PushScope(STATEMENT_SCOPE);
}

void Compiler::pAddWhileScope( const BaseTypePtr &label_of_while_head, int ln)
{

    auto break_list = make_shared<br_list>();

    sym_tab.PushScope(WHILE_SCOPE);
    sym_tab.scope_stack.top()->inside_while = true;
    sym_tab.scope_stack.top()->break_list = break_list;

    sym_tab.scope_stack.top()->while_continue_label = dynamic_pointer_cast<StringType>(label_of_while_head)->token;
}

void Compiler::pRemoveScope(int ln)
{
    sym_tab.PopScope();
}

void Compiler::pCheckBool( const BaseTypePtr &boolean_expression, int ln)
{
    if (validations.CheckFunction(boolean_expression->generation_type))
    {
        auto func_cast = dynamic_pointer_cast<FuncSymType>(boolean_expression);
        handleErrorUndef(ln, func_cast->name);
        
    }

    if (!validations.CheckBool(boolean_expression->generation_type))
    {
        handleErrorMismatch(ln);
       
    }
}

StringTypePtr Compiler::pGenerateIfL(int ln)
{
    auto lable_of_name = code_genreation.buff.genLabel(GEN_IF);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateElseL(int ln)
{
    auto lable_of_name = code_genreation.buff.genLabel(GEN_ELSE);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateWhileBodyL(int ln)
{
    auto lable_of_name = code_genreation.buff.genLabel(GEN_WHILE_BODY);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateWhileHeadL(int ln)
{
   
    auto lable_of_name = code_genreation.buff.genLabel(GEN_WHILE_HEAD);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateAndL(int ln)
{
    auto lable_of_name = code_genreation.buff.genLabel(GEN_AND);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateOrL(int ln)
{
    auto lable_of_name = code_genreation.buff.genLabel(GEN_OR);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateNextL(int ln)
{
    auto lable_of_name = code_genreation.buff.genLabel(GEN_NEXT);
    return make_shared<StringType>(lable_of_name);
}

StatementTypePtr Compiler::pBNext(int ln)
{

    return code_genreation.addBranchNext();
}

StatementTypePtr Compiler::pBWhileH(int ln)
{
    return code_genreation.addBWhileHead();
}

StatementTypePtr Compiler::pBIfNext(int ln)
{
    return code_genreation.addPBIfNext();
}

BaseTypePtr Compiler::pConvBool( BaseTypePtr expression, int ln)
{
    if (expression->generation_type == BOOL_TYPE)
    {
        auto dyn_cast_sym = dynamic_pointer_cast<SymbolType>(expression);
        auto register_reserved_bool = code_genreation.GenerateReg();
        auto result_register_pointer = make_shared<RegisterType>(register_reserved_bool, BOOL_TYPE);

        if (dyn_cast_sym)
        {
            result_register_pointer = code_genreation.addLoadReg(dyn_cast_sym->offset,
                                                       dyn_cast_sym->generation_type);
        }
        else
        {
            code_genreation.addBooleanExpressionToRegister(expression, register_reserved_bool);
        }

        return result_register_pointer;
    }
    else
    {
        return expression;
    }
}
