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
        code_genreation.EmitProgram();
    }
    handleErrorMainMissing();
}

void Compiler::pFuncs(int ln)
{
}

void Compiler::pFuncHead(int ln, const BaseTypePtr &type_returned, const BaseTypePtr &identification, const BaseTypePtr &list_of_formals)
{
    auto identification_dyn_cast = dynamic_pointer_cast<StringType>(identification);

    if (!sym_tab.IsSymbolDefined(identification_dyn_cast->t))
    {
        auto func_sym = make_shared<FuncSymType>(identification_dyn_cast->t, dynamic_pointer_cast<CType>(type_returned)->generation_type,
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

        code_genreation.EmitFuncHead(func_sym);
    }
    else
    {
        handleErrorDef(ln, identification_dyn_cast->t);
    }
}
// no
void Compiler::pFuncDecl(int ln, const BaseTypePtr &stats, const BaseTypePtr &n_label)
{
    code_genreation.EmitFuncDecl(stats, n_label);
}
// no
BaseTypePtr Compiler::pRetType(int ln, BaseTypePtr t)
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

ArgListTypePtr Compiler::pFs(int ln, const BaseTypePtr &formals_list)
{

    auto pointer_to_formals_list = make_shared<ArgListType>();
    for (auto i = dynamic_pointer_cast<ArgListType>(formals_list)->arguments_list.rbegin();
         i != dynamic_pointer_cast<ArgListType>(formals_list)->arguments_list.rend(); i++)
    {

        pointer_to_formals_list->arguments_list.push_back(*i);
    }
    return pointer_to_formals_list;
}

ArgListTypePtr Compiler::pFsList(int ln, const BaseTypePtr &f)
{

    auto arg_list_pointer = make_shared<ArgListType>();
    arg_list_pointer->arguments_list.push_back(*dynamic_pointer_cast<SymbolType>(f));
    return arg_list_pointer;
}

ArgListTypePtr Compiler::pFsList(int ln, const BaseTypePtr &f, const BaseTypePtr &formals_list)
{
    auto dyn_cast_formals = dynamic_pointer_cast<ArgListType>(formals_list);
    dyn_cast_formals->arguments_list.push_back(*dynamic_pointer_cast<SymbolType>(f));
    return dyn_cast_formals;
}

SymbolTypePtr Compiler::pFDecl(int ln, const BaseTypePtr &t, const BaseTypePtr &identification)
{
    auto symbol_pointer = make_shared<SymbolType>(dynamic_pointer_cast<StringType>(identification)->t, sym_tab.scope_stack.top()->offset,
                                                  dynamic_pointer_cast<CType>(t)->generation_type);
    return symbol_pointer;
}

StatementTypePtr Compiler::pStat(int ln, BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement)
{
    auto dynamic_cast_statement = dynamic_pointer_cast<StatementType>(statement);

    dynamic_cast_statement->next_list = CodeBuffer::merge(dynamic_cast_statement->next_list,
                                                          dynamic_pointer_cast<StatementType>(next_list_as_statement)->next_list);

    return dynamic_cast_statement;
}

StatementTypePtr Compiler::pStat(int ln, const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
                                 const BaseTypePtr &next_label,
                                 const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement)
{

    auto dynamic_cast_next_statement = dynamic_pointer_cast<StatementType>(next_statement);

    code_genreation.code_buffer.bpatch(dynamic_pointer_cast<StatementType>(statements)->next_list, dynamic_pointer_cast<StringType>(next_label)->t);
    code_genreation.code_buffer.bpatch(dynamic_pointer_cast<StatementType>(old_next_list_as_statement)->next_list, dynamic_pointer_cast<StringType>(next_label)->t);

    dynamic_cast_next_statement->next_list = CodeBuffer::merge(dynamic_cast_next_statement->next_list,
                                                               dynamic_pointer_cast<StatementType>(my_next_list_as_statement)->next_list);

    return dynamic_cast_next_statement;
}

StatementTypePtr Compiler::pStatOfStats(int ln, BaseTypePtr &statements)
{
    return dynamic_pointer_cast<StatementType>(statements);
}

StatementTypePtr Compiler::pStatType(int ln, const BaseTypePtr &t, const BaseTypePtr &identification)
{

    if (!validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->t))
    {
        const auto sym = make_shared<SymbolType>(dynamic_pointer_cast<StringType>(identification)->t, 0, dynamic_pointer_cast<CType>(t)->generation_type);
        sym_tab.AddVariable(sym);

        return code_genreation.EmitStatementType(dynamic_pointer_cast<StringType>(identification)->t);
    }
    else
    {
        handleErrorDef(ln, dynamic_pointer_cast<StringType>(identification)->t);
    }
}

StatementTypePtr
Compiler::pStatTypeAssign(int ln, const BaseTypePtr &t, const BaseTypePtr &identification, const BaseTypePtr &expression)
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
    if (!validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->t))
    {
        const auto sym = make_shared<SymbolType>(dynamic_pointer_cast<StringType>(identification)->t, 0, dynamic_pointer_cast<CType>(t)->generation_type);
        sym_tab.AddVariable(sym);

        return code_genreation.EmitStatementAssign(dynamic_pointer_cast<StringType>(identification)->t, expression);
    }
    else
    {
        handleErrorDef(ln, dynamic_pointer_cast<StringType>(identification)->t);
    }
}

StatementTypePtr Compiler::pStatAssign(int ln, const BaseTypePtr &identification, const BaseTypePtr &expression)
{
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(identification);

    if (!validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->t))
    {
        handleErrorUndef(ln, dynamic_pointer_cast<StringType>(identification)->t);
        exit(0);
    }

    auto sym_retured_id = sym_tab.GetDefinedSymbol(dynamic_pointer_cast<StringType>(identification)->t);

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

        return code_genreation.EmitStatementAssign(dynamic_pointer_cast<StringType>(identification)->t, expression);
    }
    else
    {
        handleErrorMismatch(ln);
    }
}

StatementTypePtr Compiler::pStatCall(int ln)
{
    return CodeGen::EmitStatementCall();
}

StatementTypePtr Compiler::pStatRet(int ln)
{
    if (validations.CheckReturn(VOID_TYPE))
    {
        return code_genreation.EmitStatementReturn();
    }
    else
    {
        handleErrorMismatch(ln);
    }
}

StatementTypePtr Compiler::pStatRetExp(int ln, const BaseTypePtr &expression)
{
    if (!validations.CheckFunction(expression->generation_type))
    {

        if (!((validations.CheckVoid(expression->generation_type) || (!validations.CheckReturn(expression->generation_type)))))
        {
            return code_genreation.EmitStatementReturnExp(expression);
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

StatementTypePtr Compiler::pStatIf(int ln, const BaseTypePtr &expression, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                   const BaseTypePtr &if_list_as_statement)
{
    return code_genreation.EmitStatementIf(expression, if_label, if_statement, if_list_as_statement);
}

StatementTypePtr Compiler::pStatIfElse(int ln, const BaseTypePtr &expression, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                       BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
                                       const BaseTypePtr &else_statement)
{
    return code_genreation.EmitStatementIfElse(expression, if_label, if_statement, move(if_list_as_statement), else_label,
                                        else_statement);
}
// chnage here
StatementTypePtr Compiler::pStatWhile(int ln, BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
                                      const BaseTypePtr &expression,
                                      const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                                      const BaseTypePtr &end_list_as_statement)
{

    assert(sym_tab.scope_stack.top()->scope_type == WHILE_SCOPE);
    auto break_list = sym_tab.scope_stack.top()->break_list;
    sym_tab.PopScope();

    return code_genreation.EmitStatementWhile(move(start_list_as_statement), while_head_label, expression, while_body_label,
                                       while_statement,
                                       end_list_as_statement,
                                       break_list);
}


StatementTypePtr Compiler::pStatBreak(int ln)
{
    if (validations.CheckBreak())
    {
        return code_genreation.EmitStatementBreak();
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
        return code_genreation.EmitStatementContinue();
    }
    else
    {
        handleErrorUnexpectedContinue(ln);
    }
}

BaseTypePtr Compiler::pCall(int ln, const BaseTypePtr &identification, const BaseTypePtr &exp_list)
{

    if (validations.CheckSymDefined(dynamic_pointer_cast<StringType>(identification)->t))
    {
        auto sym_retured_id = sym_tab.GetDefinedSymbol(dynamic_pointer_cast<StringType>(identification)->t);

        if (validations.CheckFunction(sym_retured_id->generation_type))
        {
            auto dynamic_cast_exp_list = dynamic_pointer_cast<TExpList>(exp_list);
            auto dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(sym_retured_id);

            if (validations.CheckCall(dynamic_cast_func, dynamic_cast_exp_list))
            {
                return code_genreation.EmitCall(dynamic_pointer_cast<FuncSymType>(sym_retured_id), dynamic_pointer_cast<TExpList>(exp_list));
            }
            else
            {
                vector<string> expected_args;
                for (const auto &sym : dynamic_pointer_cast<FuncSymType>(sym_retured_id)->params)
                {
                    expected_args.push_back(TypeToString(sym.generation_type));
                }
                handleErrorPrototypeMismatch(ln, dynamic_pointer_cast<StringType>(identification)->t, expected_args);
            }
        }
        else
        {
            handleErrorUndefFunc(ln, dynamic_pointer_cast<StringType>(identification)->t);
        }
    }
    else
    {
        handleErrorUndefFunc(ln, dynamic_pointer_cast<StringType>(identification)->t);
    }
}

BaseTypePtr Compiler::pCall(int ln, const BaseTypePtr &identification)
{
    auto dynamic_cast_id = dynamic_pointer_cast<StringType>(identification);

    if (validations.CheckSymDefined(dynamic_cast_id->t))
    {
        auto sym_retured_id = sym_tab.GetDefinedSymbol(dynamic_cast_id->t);

        if (!validations.CheckFunction(sym_tab.GetDefinedSymbol(dynamic_cast_id->t)->generation_type))
        {
            handleErrorUndefFunc(ln, dynamic_cast_id->t);
            exit(0);
        }
        std::shared_ptr<TExpList> empty_exp_list = make_shared<TExpList>();
        std::shared_ptr<FuncSymType> dynamic_cast_func = dynamic_pointer_cast<FuncSymType>(sym_tab.GetDefinedSymbol(dynamic_cast_id->t));

        if (validations.CheckCall(dynamic_cast_func, empty_exp_list))
        {
            return code_genreation.EmitCall(dynamic_pointer_cast<FuncSymType>(sym_tab.GetDefinedSymbol(dynamic_cast_id->t)));
        }
        else
        {
            vector<string> expected_args;
            for (const auto &sym : dynamic_pointer_cast<FuncSymType>(sym_tab.GetDefinedSymbol(dynamic_cast_id->t))->params)
            {
                expected_args.push_back(TypeToString(sym.generation_type));
            }
            handleErrorPrototypeMismatch(ln, dynamic_cast_id->t, expected_args);
        }
    }
    else
    {
        handleErrorUndefFunc(ln, dynamic_cast_id->t);
    }
}


BaseTypePtr Compiler::pExplist(int ln, const BaseTypePtr &expression)
{
    ExpListTypePtr exp_list_pointer = make_shared<TExpList>();

    exp_list_pointer->exp_list.insert(exp_list_pointer->exp_list.begin(), expression);
    return exp_list_pointer;
}

BaseTypePtr Compiler::pExplist(int ln, const BaseTypePtr &expression, const BaseTypePtr &exp_list)
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

BaseTypePtr Compiler::pParen(int ln, BaseTypePtr expression)
{
    return expression;
}

RegisterTypePtr Compiler::pBinop(int ln, const BaseTypePtr &first_expression, BaseTypePtr &binop, const BaseTypePtr &second_expression)
{
    if (!validations.CheckFunction(first_expression->generation_type))
    {
        if (!validations.CheckFunction(second_expression->generation_type))
        {
            if (validations.CheckAndGetBinOpType(first_expression->generation_type, second_expression->generation_type) == ERROR_TYPE)
            {
                handleErrorMismatch(ln);
            }

            return code_genreation.EmitBinop(first_expression, dynamic_pointer_cast<StringType>(binop)->t, second_expression);
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

BaseTypePtr Compiler::pID(int ln, const BaseTypePtr &identification)
{

    if (sym_tab.IsSymbolDefined(dynamic_pointer_cast<StringType>(identification)->t))
    {

        return code_genreation.EmitID(sym_tab.GetDefinedSymbol(dynamic_pointer_cast<StringType>(identification)->t));
    }
    else
    {
        handleErrorUndef(ln, dynamic_pointer_cast<StringType>(identification)->t);
    }
}
// change
BaseTypePtr Compiler::pCallExp(int ln, BaseTypePtr call_exp)
{
    return code_genreation.EmitCallExp(move(call_exp));
}

NumberTypePtr Compiler::pNum(int ln, const BaseTypePtr &num)
{

    return dynamic_pointer_cast<NumberType>(num);
}

BaseTypePtr Compiler::pNumB(int ln, BaseTypePtr num)
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
RegisterTypePtr Compiler::pString(int ln, const BaseTypePtr &stype_string)
{

    return code_genreation.EmitString(stype_string);
}
// also
BoolExpTypePtr Compiler::pTrue(int ln)
{
    return code_genreation.EmitTrue();
}
// also
BoolExpTypePtr Compiler::pFalse(int ln)
{
    return code_genreation.EmitFalse();
}

BoolExpTypePtr Compiler::pNot(int ln, const BaseTypePtr &boolean_expression)
{

    if (!validations.CheckFunction(boolean_expression->generation_type))
    {
        if (validations.CheckBool(boolean_expression->generation_type))
        {
            return CodeGen::EmitNot(boolean_expression);
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
Compiler::pAnd(int ln, const BaseTypePtr &first_boolean_exppression, const BaseTypePtr &label_of_and, const BaseTypePtr &second_boolean_exppression)
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
                return code_genreation.EmitAnd(first_boolean_exppression, label_of_and, second_boolean_exppression);
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
Compiler::pOr(int ln, const BaseTypePtr &first_boolean_exppression, const BaseTypePtr &label_of_or, const BaseTypePtr &second_boolean_exppression)
{
    if (!validations.CheckFunction(first_boolean_exppression->generation_type))
    {

        if (!validations.CheckFunction(second_boolean_exppression->generation_type))
        {
            if (validations.CheckBool(first_boolean_exppression->generation_type))
            {
                if (validations.CheckBool(second_boolean_exppression->generation_type))
                {
                    return code_genreation.EmitOr(first_boolean_exppression, label_of_or, second_boolean_exppression);
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

BoolExpTypePtr Compiler::pRelOp(int ln, const BaseTypePtr &first_expression, BaseTypePtr &relop, const BaseTypePtr &second_expression)
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
            return code_genreation.EmitRelOp(first_expression, relop, second_expression);
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

void Compiler::pAddWhileScope(int ln, const BaseTypePtr &label_of_while_head)
{

    auto break_list = make_shared<branch_list>();

    sym_tab.PushScope(WHILE_SCOPE);
    sym_tab.scope_stack.top()->inside_while = true;
    sym_tab.scope_stack.top()->break_list = break_list;

    sym_tab.scope_stack.top()->while_continue_label = dynamic_pointer_cast<StringType>(label_of_while_head)->t;
}

void Compiler::pRemoveScope(int ln)
{
    sym_tab.PopScope();
}

void Compiler::pCheckBool(int ln, const BaseTypePtr &boolean_expression)
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
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_IF);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateElseL(int ln)
{
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_ELSE);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateWhileBodyL(int ln)
{
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_WHILE_BODY);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateWhileHeadL(int ln)
{
   
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_WHILE_HEAD);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateAndL(int ln)
{
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_AND);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateOrL(int ln)
{
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_OR);
    return make_shared<StringType>(lable_of_name);
}

StringTypePtr Compiler::pGenerateNextL(int ln)
{
    auto lable_of_name = code_genreation.code_buffer.genLabel(GEN_NEXT);
    return make_shared<StringType>(lable_of_name);
}

StatementTypePtr Compiler::pBNext(int ln)
{

    return code_genreation.EmitBranchNext();
}

StatementTypePtr Compiler::pBWhileH(int ln)
{
    return code_genreation.EmitBranchWhileHead();
}

StatementTypePtr Compiler::pBIfNext(int ln)
{
    return code_genreation.EmitParseBranchIfNext();
}

BaseTypePtr Compiler::pConvBool(int ln, BaseTypePtr expression)
{
    if (expression->generation_type == BOOL_TYPE)
    {
        auto dyn_cast_sym = dynamic_pointer_cast<SymbolType>(expression);
        auto register_reserved_bool = code_genreation.GenRegister();
        auto result_register_pointer = make_shared<RegisterType>(register_reserved_bool, BOOL_TYPE);

        if (dyn_cast_sym)
        {
            result_register_pointer = code_genreation.EmitLoadRegister(dyn_cast_sym->offset,
                                                       dyn_cast_sym->generation_type);
        }
        else
        {
            code_genreation.EmitBoolExpToRegister(expression, register_reserved_bool);
        }

        return result_register_pointer;
    }
    else
    {
        return expression;
    }
}
