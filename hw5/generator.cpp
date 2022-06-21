#include "generator.h"

#define TRUE_LABEL "_convert_true"
#define BP_TRUE "br label @"
#define FALSE_LABEL "_convert_false"
#define BP_FALSE "br label @"
#define CONVERT_LABEL "_convert_final"

Generator::Generator(Validations &semantic_ref) : num_of_regs(0), buff(), validator_ref(semantic_ref)
{
    addGlobalFuncs();
}

name_of_register Generator::GenerateReg()
{

    name_of_register ret;
    return (ret + "%r" + to_string(num_of_regs++));
}

StatementTypePtr Generator::addStatRetExpression(const BaseTypePtr &exp)
{
    auto stat = make_shared<StatementType>(br_list());
    string register_result;

    if (exp->generation_type != BOOL_TYPE)
    {
        register_result = findNoBoolExpStr(exp);
    }
    else
    {
        register_result = GenerateReg();
        addBoolExpToReg(exp, register_result);
    }
    string to_emit = "ret ";
    to_emit += findTypeOfLLVM(exp->generation_type);
    to_emit += " ";
    to_emit += register_result;
    buff.emit(to_emit);
    return stat;
}

StatementTypePtr Generator::addStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                      const BaseTypePtr &if_list_as_statement)
{

    auto dynamic_cast_if_label = dynamic_pointer_cast<StringType>(if_label);
    auto dynamic_cast_if_list_as_statement = dynamic_pointer_cast<StatementType>(if_list_as_statement);
    auto dynamic_cast_if_statement = dynamic_pointer_cast<StatementType>(if_statement);
    auto dyn_cast_boolean_expression = dynamic_pointer_cast<BoolExpType>(exp);

    buff.bpatch(dyn_cast_boolean_expression->true_list, dynamic_cast_if_label->token);

    auto stat = dynamic_cast_if_statement;
    stat->next_list = Buff::merge(dynamic_cast_if_list_as_statement->next_list,
                                  Buff::merge(stat->next_list,
                                              dyn_cast_boolean_expression->false_list));

    return stat;
}

name_of_register Generator::GenerateGlobalReg()
{

    name_of_register ret;
    return (ret + "@r" + to_string(num_of_regs++));
}

void Generator::emitPrinti()
{
    buff.emitGlobal(R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")");
    buff.emitGlobal("define void @printi(i32) {");
    buff.emitGlobal("    %spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0");
    buff.emitGlobal("    call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)");
    buff.emitGlobal("    ret void");
    buff.emitGlobal("}");
}
void Generator::emitErrorDivByZero()
{
    buff.emitGlobal(R"(@err_zero = constant [23 x i8] c"Error division by zero\00")");
}

void Generator::addGlobalFuncs()
{
    Generator::emitPrintf();
    Generator::emitExit();
    Generator::emitPrint();
    Generator::emitPrinti();
    Generator::emitErrorDivByZero();
}

RegisterTypePtr Generator::addBinop(const BaseTypePtr &exp1, string binop, const BaseTypePtr &exp2)
{
    auto dyn_cast_first_expression = findNoBoolExpStr(exp1);
    auto dyn_cast_second_expression = findNoBoolExpStr(exp2);

    auto register_result = make_shared<RegisterType>(GenerateReg(), INT_TYPE);

    if (binop == "/")
    {
        addCheckDivZero(exp2);
    }

    string binary_operator = register_result->reg_name;
    binary_operator += " = ";

    if ('+' == binop[0])
    {
        binary_operator = binary_operator + "add ";
    }
    else if ('/' == binop[0])
    {
        binary_operator = binary_operator + "sdiv ";
    }
    else if ('*' == binop[0])
    {
        binary_operator = binary_operator + "mul ";
    }
    else if ('-' == binop[0])
    {
        binary_operator = binary_operator + "sub ";
    }

    buff.emit(binary_operator + "i32 " + dyn_cast_first_expression + ", " + dyn_cast_second_expression);

    if (!(validator_ref.CheckAndGetBinOpType(exp1->generation_type, exp2->generation_type) == BYTE_TYPE))
    {
        return register_result;
    }
    else
    {
        auto register_old_result = register_result->reg_name;
        register_result->reg_name = GenerateReg();
        register_result->generation_type = BYTE_TYPE;
        string to_emit = register_result->reg_name;
        to_emit += " = and i32 ";
        to_emit += register_old_result;
        to_emit += ", 255";
        buff.emit(to_emit);
    }
}

string Generator::findTypeOfLLVM(const Ty &type)
{
    string llvm_type;
    if (type == VOID_TYPE)
    {
        llvm_type = "void";
    }
    else if (type == STRING_TYPE)
    {
        llvm_type = "i8*";
    }
    else
    {
        llvm_type = "i32";
    }
    return llvm_type;
}

string Generator::findNoBoolExpStr(const BaseTypePtr &exp)
{

    auto dyn_cast_number_expression = dynamic_pointer_cast<NumberType>(exp);
    auto dyn_cast_register_expression = dynamic_pointer_cast<RegisterType>(exp);
    auto dyn_cast_string_expression = dynamic_pointer_cast<StringType>(exp);
    auto dyn_cast_symbol_expression = dynamic_pointer_cast<SymbolType>(exp);

    if (dyn_cast_number_expression)
    {
        return to_string(dyn_cast_number_expression->token);
    }
    else if (dyn_cast_symbol_expression)
    {
        return addLoadReg(dyn_cast_symbol_expression->offset, dyn_cast_symbol_expression->generation_type)->reg_name;
    }
    else if (dyn_cast_register_expression)
    {
        return dyn_cast_register_expression->reg_name;
    }
    else
    {
        assert(false);
    }
}

void Generator::addProg()
{
    buff.printGlobalBuffer();
    buff.printCodeBuffer();
}

BoolExpTypePtr Generator::addRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2)
{
    auto first_expression_value = findNoBoolExpStr(exp1);
    auto second_expression_value = findNoBoolExpStr(exp2);

    auto register_icmp = GenerateReg();
    string icmp_string(register_icmp + " = icmp ");

    auto dyn_cast_relop = dynamic_pointer_cast<StringType>(relop);
    auto relop_string = dyn_cast_relop->token;

    if (relop_string == "<")
    {
        icmp_string = icmp_string + "slt";
    }
    else if (relop_string == "==")
    {
        icmp_string = icmp_string + "eq";
    }
    else if (relop_string == "<=")
    {
        icmp_string = icmp_string + "sle";
    }
    else if (relop_string == ">=")
    {
        icmp_string = icmp_string + "sge";
    }
    else if (relop_string == ">")
    {
        icmp_string = icmp_string + "sgt";
    }
    else if (relop_string == "!=")
    {
        icmp_string = icmp_string + "ne";
    }

    else
    {
        assert(false);
    }

    icmp_string = icmp_string + " i32 ";
    icmp_string = icmp_string + first_expression_value;
    icmp_string = icmp_string + ", ";
    icmp_string = icmp_string + second_expression_value;
    buff.emit(icmp_string);
    string to_emit = "br i1 ";
    to_emit = to_emit + register_icmp;
    to_emit = to_emit + ", label @";
    to_emit = to_emit + ", label @";
    auto branch_addr = buff.emit(to_emit);

    auto true_list = Buff::makelist({branch_addr, FIRST});
    auto false_list = Buff::makelist({branch_addr, SECOND});

    return make_shared<BoolExpType>(true_list, false_list);
}
void Generator::addFuncDeclaration(const BaseTypePtr &statements, const BaseTypePtr &next_label)
{
    auto curr_func_ret_type = validator_ref.table_ref.scope_stack.top()->return_type;
    if (curr_func_ret_type != VOID_TYPE)
    {
        buff.emit("ret i32 0");
    }
    else
    {
        buff.emit("ret void");
    }

    buff.emit("}");
    stack_register.clear();
    validator_ref.table_ref.PopScope();

    auto dynamic_cast_statements = dynamic_pointer_cast<StatementType>(statements);
    auto dynamic_cast_next_label = dynamic_pointer_cast<StringType>(next_label);

    buff.bpatch(dynamic_cast_statements->next_list, dynamic_cast_next_label->token);
}

BoolExpTypePtr Generator::addTrue()
{
    return make_shared<BoolExpType>(Buff::makelist({buff.emit("br label @"), FIRST}), br_list());
}

StatementTypePtr Generator::addStatType(string id)
{
    auto symbol = validator_ref.table_ref.GetDefinedSymbol(id);
    auto stat = make_shared<StatementType>(br_list());

    auto reg_val = GenerateReg();
    buff.emit(reg_val + " = add i32 0, 0");

    addStoreReg(symbol->offset, reg_val);

    return stat;
}

StatementTypePtr Generator::addStatAssign(string id, const BaseTypePtr &exp)
{
    auto symbol = validator_ref.table_ref.GetDefinedSymbol(id);
    auto stat = make_shared<StatementType>(br_list());
    auto register_result = GenerateReg();

    if (exp->generation_type != BOOL_TYPE)
    {
        auto exp_str = findNoBoolExpStr(exp);
        string string_to_emit = register_result;
        string_to_emit += " = add i32 ";
        string_to_emit += exp_str;
        string_to_emit += ", 0";
        buff.emit(string_to_emit);
    }
    else
    {
        addBoolExpToReg(exp, register_result);
    }
    addStoreReg(symbol->offset, register_result);
    return stat;
}

void Generator::emitPrintf()
{
    buff.emitGlobal("declare i32 @printf(i8*, ...)");
}
void Generator::emitExit()
{
    buff.emitGlobal("declare void @exit(i32)");
}

void Generator::addFunctionHead(const FuncSymbolTypePtr &symbol)
{

    string emit_string("define ");

    if (symbol->ret_type != VOID_TYPE)
    {
        emit_string += "i32 @";
    }
    else
    {
        emit_string += "void @";
    }

    emit_string += symbol->name + "(";

    size_t count = 0;
    while (count < symbol->params.size())
    {
        if (count > 0)
        {
            emit_string += ", ";
        }
        emit_string += "i32";
        ++count;
    }

    emit_string = emit_string + ")";
    emit_string = emit_string + "{";

    buff.emit(emit_string);

    stack_register = GenerateReg();
    buff.emit(stack_register + " = alloca i32, i32 " + to_string(STACK_SIZE));
}
BaseTypePtr Generator::regToBooleanExpression(string &reg_source)
{
    auto reg_bitcast = GenerateReg();
    string to_emit1 = reg_bitcast;
    string to_emit2;
    to_emit1 += " = trunc i32 ";
    to_emit1 += reg_source;
    to_emit1 += " to i1";

    to_emit2 = "br i1 ";
    to_emit2 = to_emit2 + reg_bitcast;
    to_emit2 = to_emit2 + ", label @, label @";

    buff.emit(to_emit1);
    auto branch_addr = buff.emit(to_emit2);

    return make_shared<BoolExpType>(Buff::makelist({branch_addr, FIRST}),
                                    Buff::makelist({branch_addr, SECOND}));
}

StatementTypePtr Generator::addStatCall()
{

    return make_shared<StatementType>(br_list());
}

void Generator::addStoreReg(int offset, const name_of_register &reg_to_store)
{
    assert(!stack_register.empty());
    auto register_pointer_to_the_stack = GenerateReg();

    if (offset < 0)
    {
        return;
    }
    string to_emit = register_pointer_to_the_stack;
    to_emit = to_emit + " = getelementptr i32, i32* ";
    to_emit = to_emit + stack_register;
    to_emit = to_emit + ", i32 ";
    to_emit = to_emit + to_string(offset);

    buff.emit(to_emit);
    to_emit = "store i32 ";
    to_emit = to_emit + reg_to_store;
    to_emit = to_emit + ", i32* ";
    to_emit = to_emit + register_pointer_to_the_stack;

    buff.emit(to_emit);
}

RegisterTypePtr Generator::addLoadReg(int offset, Ty type)
{
    assert(!stack_register.empty());
    auto register_result = GenerateReg();
    string to_emit;
    if (offset < 0)
    {
        auto reg_argument = "%" + to_string(-offset - 1);
        to_emit = register_result;
        to_emit = to_emit + " = add";
        to_emit = to_emit + " i32 0, ";
        to_emit = to_emit + reg_argument;

        buff.emit(to_emit);
    }
    else
    {
        auto reg_stack_offset = GenerateReg();
        string string_to_emit = reg_stack_offset + " = getelementptr i32, i32* ";
        string_to_emit += stack_register;
        string_to_emit += ", i32 " + to_string(offset);
        buff.emit(string_to_emit);
        string_to_emit = register_result;
        string_to_emit = string_to_emit + " = load i32, i32* ";
        string_to_emit = string_to_emit + reg_stack_offset;
        buff.emit(string_to_emit);
    }

    return make_shared<RegisterType>(register_result, type);
}

StatementTypePtr Generator::addStatRet()
{
    std::shared_ptr<StatementType> stat = make_shared<StatementType>(br_list());
    buff.emit("ret void");
    return stat;
}

void Generator::addCheckDivZero(const BaseTypePtr &exp)
{

    auto dyn_cast_number_expression = dynamic_pointer_cast<NumberType>(exp);
    auto dyn_cast_register_expression = dynamic_pointer_cast<RegisterType>(exp);

    auto check_is_zero = GenerateReg();
    auto error_strring = GenerateReg();

    if (!dyn_cast_register_expression)
    {
        auto str_to_emit = check_is_zero;
        str_to_emit += " = icmp eq i32 ";
        str_to_emit += to_string(dyn_cast_number_expression->token);
        str_to_emit += ", 0";
        buff.emit(str_to_emit);
    }
    else
    {
        auto str_to_emit = check_is_zero;
        str_to_emit += " = icmp eq i32 ";
        str_to_emit += dyn_cast_register_expression->reg_name;
        str_to_emit += ", 0";
        buff.emit(str_to_emit);
    }
    std::string str_to_emit = "br i1 ";
    str_to_emit += check_is_zero;
    str_to_emit += ", label @, label @";

    auto bp_non_and_zero = buff.emit(str_to_emit);
    auto label_error_zero = buff.genLabel("_err_zero");
    vector<pair<int, BranchLabelIndex>> bp_zero_vector;
    bp_zero_vector.emplace_back(bp_non_and_zero, FIRST);
    buff.bpatch(bp_zero_vector, label_error_zero);

    buff.emit(error_strring + " = getelementptr [23 x i8], [23 x i8]* @err_zero, i32 0, i32 0");
    buff.emit("call void (i8*) @print(i8* " + error_strring + ")");
    buff.emit("call void (i32) @exit(i32 0)");
    buff.emit("unreachable");

    auto label_of_not_zero = buff.genLabel("_non_zero");
    vector<pair<int, BranchLabelIndex>> bp_none_vector;
    bp_none_vector.emplace_back(bp_non_and_zero, SECOND);
    buff.bpatch(bp_none_vector, label_of_not_zero);
}

RegisterTypePtr Generator::addCall(const FuncSymbolTypePtr &func, const ExpListTypePtr &exp_list)
{
    auto register_result = make_shared<RegisterType>(GenerateReg(), func->ret_type);
    string output_string_to_emit;

    if (func->ret_type != VOID_TYPE)
    {
        output_string_to_emit += register_result->reg_name + " = ";
    }

    output_string_to_emit += "call ";
    string llvm_type = findTypeOfLLVM(func->ret_type);
    output_string_to_emit = output_string_to_emit + llvm_type;
    output_string_to_emit = output_string_to_emit + " @";
    output_string_to_emit = output_string_to_emit + func->name + "(";
    size_t count = 0;
    while (count < exp_list->exp_list.size())
    {
        if (count > 0)
        {
            output_string_to_emit = output_string_to_emit + ", ";
        }
        output_string_to_emit = output_string_to_emit + findTypeOfLLVM(exp_list->exp_list[count]->generation_type) + " ";
        {
            output_string_to_emit = output_string_to_emit + findNoBoolExpStr(exp_list->exp_list[count]);
        }
        ++count;
    }
    buff.emit(output_string_to_emit + ")");
    return register_result;
}

RegisterTypePtr Generator::addCall(const FuncSymbolTypePtr &func)
{
    auto register_result = make_shared<RegisterType>(GenerateReg(), func->ret_type);
    string output_string_to_emit;

    if (func->ret_type == VOID_TYPE)
    {
        output_string_to_emit = output_string_to_emit + "call ";
        string llvm_type = findTypeOfLLVM(func->ret_type);
        output_string_to_emit = output_string_to_emit + llvm_type;
        output_string_to_emit = output_string_to_emit + " @";
        output_string_to_emit = output_string_to_emit + func->name;
        output_string_to_emit = output_string_to_emit + "()";
        buff.emit(output_string_to_emit);
        return register_result;
    }
    else
    {
        output_string_to_emit = output_string_to_emit + register_result->reg_name + " = " + "call ";

        string llvm_type = findTypeOfLLVM(func->ret_type);
        output_string_to_emit = output_string_to_emit + llvm_type;
        output_string_to_emit = output_string_to_emit + " @";
        output_string_to_emit = output_string_to_emit + func->name;
        output_string_to_emit = output_string_to_emit + "()";
        buff.emit(output_string_to_emit);
        return register_result;
    }
}

StatementTypePtr Generator::addStatIfAndElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                                             BaseTypePtr if_list_as_statement,
                                             const BaseTypePtr &else_label, const BaseTypePtr &else_statement)
{

    auto dynamic_cast_if_list_as_statement = dynamic_pointer_cast<StatementType>(if_list_as_statement);
    auto dynamic_cast_if_statement = dynamic_pointer_cast<StatementType>(if_statement);
    auto dynamic_cast_if_label = dynamic_pointer_cast<StringType>(if_label);
    auto dyn_cast_boolean_expression = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_else_label = dynamic_pointer_cast<StringType>(else_label);
    auto dynamic_cast_else_statement = dynamic_pointer_cast<StatementType>(else_statement);

    buff.bpatch(dyn_cast_boolean_expression->true_list, dynamic_cast_if_label->token);
    buff.bpatch(dyn_cast_boolean_expression->false_list, dynamic_cast_else_label->token);

    auto stat = dynamic_cast_if_statement;
    stat->next_list = Buff::merge(
        Buff::merge(stat->next_list, dynamic_cast_else_statement->next_list),
        dynamic_cast_if_list_as_statement->next_list);

    return stat;
}

StatementTypePtr Generator::addStatWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label, const BaseTypePtr &exp,
                                         const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                                         const BaseTypePtr &end_list_as_statement, const br_list_pointer &break_list)
{
    auto dyn_cast_boolean_expression = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_start_list_as_statement = dynamic_pointer_cast<StatementType>(start_list_as_statement);
    auto dynamic_cast_while_statement = dynamic_pointer_cast<StatementType>(while_statement);
    auto dynamic_cast_end_list_as_statement = dynamic_pointer_cast<StatementType>(end_list_as_statement);
    auto dynamic_cast_while_body_label = dynamic_pointer_cast<StringType>(while_body_label);
    auto dynamic_cast_while_head_label = dynamic_pointer_cast<StringType>(while_head_label);

    buff.bpatch(dyn_cast_boolean_expression->true_list, dynamic_cast_while_body_label->token);
    buff.bpatch(dynamic_cast_while_statement->next_list, dynamic_cast_while_head_label->token);
    buff.bpatch(dynamic_cast_end_list_as_statement->next_list, dynamic_cast_while_head_label->token);
    buff.bpatch(dynamic_cast_start_list_as_statement->next_list, dynamic_cast_while_head_label->token);

    auto stat = make_shared<StatementType>(Buff::merge(dyn_cast_boolean_expression->false_list,
                                                       *break_list));

    return stat;
}

void Generator::emitPrint()
{
    buff.emitGlobal(R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")");
    buff.emitGlobal("define void @print(i8*) {");
    buff.emitGlobal("    %spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0");
    buff.emitGlobal("    call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)");
    buff.emitGlobal("    ret void");
    buff.emitGlobal("}");
}
StatementTypePtr Generator::addStatBreak()
{
    string break_label = "br label @  ; break";
    validator_ref.table_ref.scope_stack.top()->break_list->push_back({buff.emit(break_label), FIRST});

    auto stat = make_shared<StatementType>(br_list());
    return stat;
}

BoolExpTypePtr Generator::addAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2)
{
    auto dyn_cast_boolean_expression_second = dynamic_pointer_cast<BoolExpType>(bool_exp2);
    auto dynamic_cast_and_label = dynamic_pointer_cast<StringType>(and_label);
    auto dyn_cast_boolean_expression_first = dynamic_pointer_cast<BoolExpType>(bool_exp1);

    buff.bpatch(dyn_cast_boolean_expression_first->true_list, dynamic_cast_and_label->token);

    return make_shared<BoolExpType>(dyn_cast_boolean_expression_second->true_list,
                                    Buff::merge(dyn_cast_boolean_expression_first->false_list,
                                                dyn_cast_boolean_expression_second->false_list));
}

BoolExpTypePtr Generator::addOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2)
{
    auto dyn_cast_boolean_expression_second = dynamic_pointer_cast<BoolExpType>(bool_exp2);
    auto dyn_cast_or_label = dynamic_pointer_cast<StringType>(or_label);
    auto dyn_cast_boolean_expression_first = dynamic_pointer_cast<BoolExpType>(bool_exp1);

    buff.bpatch(dyn_cast_boolean_expression_first->false_list, dyn_cast_or_label->token);

    return make_shared<BoolExpType>(Buff::merge(dyn_cast_boolean_expression_first->true_list,
                                                dyn_cast_boolean_expression_second->true_list),
                                    dyn_cast_boolean_expression_second->false_list);
}

StatementTypePtr Generator::addBranchNext()
{
    string empty_stat_string = "br label @  ; end of stat";
    auto fake_stat = make_shared<StatementType>(Buff::makelist({buff.emit(empty_stat_string), FIRST}));
    return fake_stat;
}

StatementTypePtr Generator::addBWhileHead()
{
    string empty_stat_string = "br label @  ; start/end of while";
    auto fake_stat = make_shared<StatementType>(Buff::makelist({buff.emit(empty_stat_string), FIRST}));
    return fake_stat;
}
StatementTypePtr Generator::addStatContinue()
{
    buff.emit("br label %" + validator_ref.table_ref.scope_stack.top()->while_continue_label);
    auto stat = make_shared<StatementType>(br_list());
    return stat;
}

BoolExpTypePtr Generator::addFalse()
{
    return make_shared<BoolExpType>(br_list(), Buff::makelist({buff.emit("br label @"), FIRST}));
}

BoolExpTypePtr Generator::addNot(const BaseTypePtr &bool_exp)
{
    auto dyn_cast_boolean_expression = dynamic_pointer_cast<BoolExpType>(bool_exp);

    br_list old_true_list(dyn_cast_boolean_expression->true_list);
    dyn_cast_boolean_expression->true_list = dyn_cast_boolean_expression->false_list;
    dyn_cast_boolean_expression->false_list = old_true_list;

    return dyn_cast_boolean_expression;
}

RegisterTypePtr Generator::addString(const BaseTypePtr &stype_string)
{
    auto dyn_cast_string = dynamic_pointer_cast<StringType>(stype_string);
    dyn_cast_string->token = dyn_cast_string->token.substr(1, dyn_cast_string->token.size() - 2);
    auto register_result = make_shared<RegisterType>(GenerateReg(), STRING_TYPE);
    auto register_glob = GenerateGlobalReg();
    auto string_size_string = to_string(dyn_cast_string->token.size() + 1);

    string to_emit = register_glob + " = constant [";
    to_emit += string_size_string + " x i8] c\"";
    to_emit += dyn_cast_string->token + "\\00\"";
    buff.emitGlobal(to_emit);

    to_emit = register_result->reg_name + " = getelementptr [";
    to_emit += string_size_string + " x i8], [";
    to_emit += string_size_string + " x i8]* ";
    to_emit += register_glob + ", i32 0, i32 0";
    buff.emit(to_emit);

    return register_result;
}

BaseTypePtr Generator::addIdentification(const SymbolTypePtr &symbol)
{
    auto dyn_cast_sym = dynamic_pointer_cast<SymbolType>(symbol);

    if (FUNCTION_TYPE == dyn_cast_sym->generation_type)
    {
        return dyn_cast_sym;
    }

    auto exp_reg = addLoadReg(dyn_cast_sym->offset, dyn_cast_sym->generation_type);
    if (exp_reg->generation_type != BOOL_TYPE)
    {
        return exp_reg;
    }
    else
    {
        return regToBooleanExpression(exp_reg->reg_name);
    }
}

StatementTypePtr Generator::addPBIfNext()
{
    string empty_stat_string = "br label @  ; end of if";
    auto empty_statement = make_shared<StatementType>(Buff::makelist({buff.emit(empty_stat_string), FIRST}));
    return empty_statement;
}

BaseTypePtr Generator::addCallExpression(BaseTypePtr call_exp)
{
    auto dyn_cast_call_expression = dynamic_pointer_cast<RegisterType>(call_exp);
    if (call_exp->generation_type != BOOL_TYPE)
    {
        return call_exp;
    }
    else
    {
        return regToBooleanExpression(dyn_cast_call_expression->reg_name);
    }
}

void Generator::addBoolExpToReg(const BaseTypePtr &exp, const name_of_register &register_result)
{

    auto dyn_cast_boolean_expression = dynamic_pointer_cast<BoolExpType>(exp);
    auto true_label = buff.genLabel(TRUE_LABEL);
    auto bp_true = buff.emit(BP_TRUE);
    auto false_label = buff.genLabel(FALSE_LABEL);
    auto bp_false = buff.emit(BP_FALSE);
    auto convert_label = buff.genLabel(CONVERT_LABEL);

    auto convert_true_l = Buff::makelist(branch_pair(bp_true, FIRST));
    auto convert_false_l = Buff::makelist(branch_pair(bp_false, FIRST));

    buff.bpatch(dyn_cast_boolean_expression->true_list, true_label);
    buff.bpatch(dyn_cast_boolean_expression->false_list, false_label);
    buff.bpatch(convert_true_l, convert_label);
    buff.bpatch(convert_false_l, convert_label);

    buff.emit(register_result + " = phi i32 [1, %" + true_label + "], [0, %" + false_label + "]");
}
