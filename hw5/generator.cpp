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

    name_of_register returned_value;
    return (returned_value + "%r" + to_string(num_of_regs++));
}

name_of_register Generator::GenerateGlobalReg()
{

    name_of_register returned_value;
    return (returned_value + "@r" + to_string(num_of_regs++));
}
void Generator::emitPrintf()
{
    buff.emitGlobal("declare i32 @printf(i8*, ...)");
}
void Generator::emitExit()
{
    buff.emitGlobal("declare void @exit(i32)");
}
void Generator::emitPrint()
{
    buff.emitGlobal(R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")");
    buff.emitGlobal("define void @print(i8*) {");
    buff.emitGlobal("    %spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0");
    buff.emitGlobal("    call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)");
    buff.emitGlobal("    returned_value void");
    buff.emitGlobal("}");
}

void Generator::emitPrinti()
{
    buff.emitGlobal(R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")");
    buff.emitGlobal("define void @printi(i32) {");
    buff.emitGlobal("    %spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0");
    buff.emitGlobal("    call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)");
    buff.emitGlobal("    returned_value void");
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
    auto dynamic_cast_first_expression = findNoBoolExpStr(exp1);
    auto dynamic_cast_second_expression = findNoBoolExpStr(exp2);

    auto register_result = make_shared<RegisterType>(GenerateReg(), INT_TYPE);

    if (binop == "/")
    {
        addCheckDivZero(exp2);
    }

    string binop_str = register_result->reg_name;
    binop_str += " = ";

    if ('+' == binop[0])
    {
        binop_str = binop_str + "add ";
    }
    else if ('/' == binop[0])
    {
        binop_str = binop_str + "sdiv ";
    }
    else if ('*' == binop[0])
    {
        binop_str = binop_str + "mul ";
    }
    else if ('-' == binop[0])
    {
        binop_str = binop_str + "sub ";
    }

    buff.emit(binop_str + "i32 " + dynamic_cast_first_expression + ", " + dynamic_cast_second_expression);

    if (!(validator_ref.CheckAndGetBinOpType(exp1->generation_type, exp2->generation_type) == BYTE_TYPE))
    {
        return register_result;
    }
    else
    {
        auto reg_old_result = register_result->reg_name;
        register_result->reg_name = GenerateReg();
        register_result->generation_type = BYTE_TYPE;
        string to_emit = register_result->reg_name;
        to_emit += " = and i32 ";
        to_emit += reg_old_result;
        to_emit += ", 255";
        buff.emit(to_emit);
    }
}

void Generator::addCheckDivZero(const BaseTypePtr &exp)
{

    auto dynamic_cast_num_exp = dynamic_pointer_cast<NumberType>(exp);
    auto dynamic_cast_reg_exp = dynamic_pointer_cast<RegisterType>(exp);

    auto is_zero = GenerateReg();
    auto err_str = GenerateReg();

    if (!dynamic_cast_reg_exp)
    {
        auto str_to_emit = is_zero;
        str_to_emit += " = icmp eq i32 ";
        str_to_emit += to_string(dynamic_cast_num_exp->token);
        str_to_emit += ", 0";
        buff.emit(str_to_emit);
    }
    else
    {
        auto str_to_emit = is_zero;
        str_to_emit += " = icmp eq i32 ";
        str_to_emit += dynamic_cast_reg_exp->reg_name;
        str_to_emit += ", 0";
        buff.emit(str_to_emit);
    }
    std::string str_to_emit = "br i1 ";
    str_to_emit += is_zero;
    str_to_emit += ", label @, label @";

    auto bp_zero_and_non = buff.emit(str_to_emit);
    auto label_err_zero = buff.genLabel("_err_zero");
    vector<pair<int, BranchLabelIndex>> bp_zero_vec;
    bp_zero_vec.emplace_back(bp_zero_and_non, FIRST);
    buff.bpatch(bp_zero_vec, label_err_zero);

    buff.emit(err_str + " = getelementptr [23 x i8], [23 x i8]* @err_zero, i32 0, i32 0");
    buff.emit("call void (i8*) @print(i8* " + err_str + ")");
    buff.emit("call void (i32) @exit(i32 0)");
    buff.emit("unreachable");

    auto label_non_zero = buff.genLabel("_non_zero");
    vector<pair<int, BranchLabelIndex>> bp_non_vec;
    bp_non_vec.emplace_back(bp_zero_and_non, SECOND);
    buff.bpatch(bp_non_vec, label_non_zero);
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

StatementTypePtr Generator::addStatType(string id)
{
    auto symbol = validator_ref.table_ref.GetDefinedSymbol(id);
    auto statement = make_shared<StatementType>(br_list());

    auto reg_val = GenerateReg();
    buff.emit(reg_val + " = add i32 0, 0");

    addStoreReg(symbol->offset, reg_val);

    return statement;
}

StatementTypePtr Generator::addStatAssign(string id, const BaseTypePtr &exp)
{
    auto symbol = validator_ref.table_ref.GetDefinedSymbol(id);
    auto statement = make_shared<StatementType>(br_list());
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
    return statement;
}

void Generator::addBoolExpToReg(const BaseTypePtr &exp, const name_of_register &register_result)
{

    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto true_label = buff.genLabel(TRUE_LABEL);
    auto bp_true = buff.emit(BP_TRUE);
    auto false_label = buff.genLabel(FALSE_LABEL);
    auto bp_false = buff.emit(BP_FALSE);
    auto convert_label = buff.genLabel(CONVERT_LABEL);

    auto convert_true_list = Buff::makelist(branch_pair(bp_true, FIRST));
    auto convert_false_list = Buff::makelist(branch_pair(bp_false, FIRST));

    buff.bpatch(dynamic_cast_bool_exp->true_list, true_label);
    buff.bpatch(dynamic_cast_bool_exp->false_list, false_label);
    buff.bpatch(convert_true_list, convert_label);
    buff.bpatch(convert_false_list, convert_label);

    buff.emit(register_result + " = phi i32 [1, %" + true_label + "], [0, %" + false_label + "]");
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

    size_t i = 0;
    while (i < symbol->params.size())
    {
        if (i > 0)
        {
            emit_string += ", ";
        }
        emit_string += "i32";
        ++i;
    }

    emit_string = emit_string + ")";
    emit_string = emit_string + "{";

    buff.emit(emit_string);

    stack_register = GenerateReg();
    buff.emit(stack_register + " = alloca i32, i32 " + to_string(STACK_SIZE));
}

void Generator::addStoreReg(int offset, const name_of_register &reg_to_store)
{
    assert(!stack_register.empty());
    auto reg_pointer_to_stack = GenerateReg();

    if (offset < 0)
    {
        return;
    }
    string to_emit = reg_pointer_to_stack;
    to_emit = to_emit + " = getelementptr i32, i32* ";
    to_emit = to_emit + stack_register;
    to_emit = to_emit + ", i32 ";
    to_emit = to_emit + to_string(offset);

    buff.emit(to_emit);
    to_emit = "store i32 ";
    to_emit = to_emit + reg_to_store;
    to_emit = to_emit + ", i32* ";
    to_emit = to_emit + reg_pointer_to_stack;

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

StatementTypePtr Generator::addStatRet()
{
    std::shared_ptr<StatementType> statement = make_shared<StatementType>(br_list());
    buff.emit("returned_value void");
    return statement;
}

StatementTypePtr Generator::addStatRetExpression(const BaseTypePtr &exp)
{
    auto statement = make_shared<StatementType>(br_list());
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
    string to_emit = "returned_value ";
    to_emit += findTypeOfLLVM(exp->generation_type);
    to_emit += " ";
    to_emit += register_result;
    buff.emit(to_emit);
    return statement;
}

StatementTypePtr
Generator::addStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                     const BaseTypePtr &if_list_as_statement)
{

    auto dynamic_cast_if_label = dynamic_pointer_cast<StringType>(if_label);
    auto dynamic_cast_if_list_as_statement = dynamic_pointer_cast<StatementType>(if_list_as_statement);
    auto dynamic_cast_if_statement = dynamic_pointer_cast<StatementType>(if_statement);
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);

    buff.bpatch(dynamic_cast_bool_exp->true_list, dynamic_cast_if_label->token);

    auto statement = dynamic_cast_if_statement;
    statement->next_list = Buff::merge(dynamic_cast_if_list_as_statement->next_list,
                                       Buff::merge(statement->next_list,
                                                   dynamic_cast_bool_exp->false_list));

    return statement;
}

StatementTypePtr
Generator::addStatIfAndElse(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                            BaseTypePtr if_list_as_statement,
                            const BaseTypePtr &else_label, const BaseTypePtr &else_statement)
{

    auto dynamic_cast_if_list_as_statement = dynamic_pointer_cast<StatementType>(if_list_as_statement);
    auto dynamic_cast_if_statement = dynamic_pointer_cast<StatementType>(if_statement);
    auto dynamic_cast_if_label = dynamic_pointer_cast<StringType>(if_label);
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_else_label = dynamic_pointer_cast<StringType>(else_label);
    auto dynamic_cast_else_statement = dynamic_pointer_cast<StatementType>(else_statement);

    buff.bpatch(dynamic_cast_bool_exp->true_list, dynamic_cast_if_label->token);
    buff.bpatch(dynamic_cast_bool_exp->false_list, dynamic_cast_else_label->token);

    auto statement = dynamic_cast_if_statement;
    statement->next_list = Buff::merge(
        Buff::merge(statement->next_list, dynamic_cast_else_statement->next_list),
        dynamic_cast_if_list_as_statement->next_list);

    return statement;
}

StatementTypePtr
Generator::addStatWhile(BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label, const BaseTypePtr &exp,
                        const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                        const BaseTypePtr &end_list_as_statement, const br_list_pointer &break_list)
{
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_start_list_as_statement = dynamic_pointer_cast<StatementType>(start_list_as_statement);
    auto dynamic_cast_while_statement = dynamic_pointer_cast<StatementType>(while_statement);
    auto dynamic_cast_end_list_as_statement = dynamic_pointer_cast<StatementType>(end_list_as_statement);
    auto dynamic_cast_while_body_label = dynamic_pointer_cast<StringType>(while_body_label);
    auto dynamic_cast_while_head_label = dynamic_pointer_cast<StringType>(while_head_label);

    buff.bpatch(dynamic_cast_bool_exp->true_list, dynamic_cast_while_body_label->token);
    buff.bpatch(dynamic_cast_while_statement->next_list, dynamic_cast_while_head_label->token);
    buff.bpatch(dynamic_cast_end_list_as_statement->next_list, dynamic_cast_while_head_label->token);
    buff.bpatch(dynamic_cast_start_list_as_statement->next_list, dynamic_cast_while_head_label->token);

    auto statement = make_shared<StatementType>(Buff::merge(dynamic_cast_bool_exp->false_list,
                                                            *break_list));

    return statement;
}

StatementTypePtr Generator::addStatBreak()
{
    string break_label = "br label @  ; break";
    validator_ref.table_ref.scope_stack.top()->break_list->push_back({buff.emit(break_label), FIRST});

    auto statement = make_shared<StatementType>(br_list());
    return statement;
}

StatementTypePtr Generator::addStatContinue()
{
    buff.emit("br label %" + validator_ref.table_ref.scope_stack.top()->while_continue_label);
    auto statement = make_shared<StatementType>(br_list());
    return statement;
}

void Generator::addFuncDeclaration(const BaseTypePtr &statements, const BaseTypePtr &next_label)
{
    auto curr_func_ret_type = validator_ref.table_ref.scope_stack.top()->return_type;
    if (curr_func_ret_type != VOID_TYPE)
    {
        buff.emit("returned_value i32 0");
    }
    else
    {
        buff.emit("returned_value void");
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

BoolExpTypePtr Generator::addFalse()
{
    return make_shared<BoolExpType>(br_list(), Buff::makelist({buff.emit("br label @"), FIRST}));
}

BoolExpTypePtr Generator::addNot(const BaseTypePtr &bool_exp)
{
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(bool_exp);

    br_list old_true_list(dynamic_cast_bool_exp->true_list);
    dynamic_cast_bool_exp->true_list = dynamic_cast_bool_exp->false_list;
    dynamic_cast_bool_exp->false_list = old_true_list;

    return dynamic_cast_bool_exp;
}

BoolExpTypePtr Generator::addAnd(const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2)
{
    auto dynamic_cast_bool_exp2 = dynamic_pointer_cast<BoolExpType>(bool_exp2);
    auto dynamic_cast_and_label = dynamic_pointer_cast<StringType>(and_label);
    auto dynamic_cast_bool_exp1 = dynamic_pointer_cast<BoolExpType>(bool_exp1);
   
    

    buff.bpatch(dynamic_cast_bool_exp1->true_list, dynamic_cast_and_label->token);

    return make_shared<BoolExpType>(dynamic_cast_bool_exp2->true_list,
                                    Buff::merge(dynamic_cast_bool_exp1->false_list,
                                                dynamic_cast_bool_exp2->false_list));
}

BoolExpTypePtr Generator::addOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2)
{
    auto dynamic_cast_bool_exp2 = dynamic_pointer_cast<BoolExpType>(bool_exp2);
     auto dynamic_cast_or_label = dynamic_pointer_cast<StringType>(or_label);
    auto dynamic_cast_bool_exp1 = dynamic_pointer_cast<BoolExpType>(bool_exp1);
   
    

    buff.bpatch(dynamic_cast_bool_exp1->false_list, dynamic_cast_or_label->token);

    return make_shared<BoolExpType>(Buff::merge(dynamic_cast_bool_exp1->true_list,
                                                dynamic_cast_bool_exp2->true_list),
                                    dynamic_cast_bool_exp2->false_list);
}

RegisterTypePtr Generator::addString(const BaseTypePtr &stype_string)
{
    auto dynamic_cast_string = dynamic_pointer_cast<StringType>(stype_string);
    dynamic_cast_string->token = dynamic_cast_string->token.substr(1, dynamic_cast_string->token.size() - 2);
    auto register_result = make_shared<RegisterType>(GenerateReg(), STRING_TYPE);
    auto reg_global = GenerateGlobalReg();
    auto string_size_string = to_string(dynamic_cast_string->token.size() + 1);

    string to_emit = reg_global + " = constant [";
    to_emit += string_size_string + " x i8] c\"";
    to_emit += dynamic_cast_string->token + "\\00\"";
    buff.emitGlobal(to_emit);

    to_emit = register_result->reg_name + " = getelementptr [";
    to_emit += string_size_string + " x i8], [";
    to_emit += string_size_string + " x i8]* ";
    to_emit += reg_global + ", i32 0, i32 0";
    buff.emit(to_emit);

    return register_result;
}

BaseTypePtr Generator::addIdentification(const SymbolTypePtr &symbol)
{
    auto dynamic_cast_symbol = dynamic_pointer_cast<SymbolType>(symbol);

    if (dynamic_cast_symbol->generation_type == FUNCTION_TYPE)
    {
        return dynamic_cast_symbol;
    }

    auto exp_reg = addLoadReg(dynamic_cast_symbol->offset, dynamic_cast_symbol->generation_type);
    if (exp_reg->generation_type == BOOL_TYPE)
    {
        return regToBooleanExpression(exp_reg->reg_name);
    }

    return exp_reg;
}

StatementTypePtr Generator::addBranchNext()
{
    string empty_statement_str = "br label @  ; end of statement";
    auto fake_statement = make_shared<StatementType>(Buff::makelist({buff.emit(empty_statement_str), FIRST}));
    return fake_statement;
}

StatementTypePtr Generator::addBWhileHead()
{
    string empty_statement_str = "br label @  ; start/end of while";
    auto fake_statement = make_shared<StatementType>(Buff::makelist({buff.emit(empty_statement_str), FIRST}));
    return fake_statement;
}

StatementTypePtr Generator::addPBIfNext()
{
    string empty_statement_str = "br label @  ; end of if";
    auto empty_statement = make_shared<StatementType>(Buff::makelist({buff.emit(empty_statement_str), FIRST}));
    return empty_statement;
}

BaseTypePtr Generator::addCallExpression(BaseTypePtr call_exp)
{
    auto dynamic_cast_call_exp = dynamic_pointer_cast<RegisterType>(call_exp);
    if (call_exp->generation_type != BOOL_TYPE)
    {
        return call_exp;
    }
    else
    {
        return regToBooleanExpression(dynamic_cast_call_exp->reg_name);
    }
}
