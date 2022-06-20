#include "code_gen.h"

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

name_of_register Generator::GenerateGlobalReg()
{

    name_of_register ret;
    return (ret + "@r" + to_string(num_of_regs++));
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
    buff.emitGlobal("    ret void");
    buff.emitGlobal("}");
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
    auto dynamic_cast_exp1 = findNoBoolExpStr(exp1);
    auto dynamic_cast_exp2 = findNoBoolExpStr(exp2);

    auto reg_result = make_shared<RegisterType>(GenerateReg(), INT_TYPE);

    if (binop == "/")
    {
        addCheckDivZero(exp2);
    }

    string binop_str = reg_result->reg_name;
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

    buff.emit(binop_str + "i32 " + dynamic_cast_exp1 + ", " + dynamic_cast_exp2);

    if (!(validator_ref.CheckAndGetBinOpType(exp1->generation_type, exp2->generation_type) == BYTE_TYPE))
    {
        return reg_result;
    }
    else
    {
        auto reg_old_result = reg_result->reg_name;
        reg_result->reg_name = GenerateReg();
        reg_result->generation_type = BYTE_TYPE;
        string to_emit = reg_result->reg_name;
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
    auto reg_result = make_shared<RegisterType>(GenerateReg(), func->ret_type);
    string out_str;

    if (func->ret_type != VOID_TYPE)
    {
        out_str += reg_result->reg_name + " = ";
    }

    out_str += "call ";
    string llvm_type = findTypeOfLLVM(func->ret_type);
    out_str = out_str + llvm_type;
    out_str = out_str + " @";
    out_str = out_str + func->name + "(";
    size_t count = 0;
    while (count < exp_list->exp_list.size())
    {
        if (count > 0)
        {
            out_str = out_str + ", ";
        }
        out_str = out_str + findTypeOfLLVM(exp_list->exp_list[count]->generation_type) + " ";
        {
            out_str = out_str + findNoBoolExpStr(exp_list->exp_list[count]);
        }
        ++count;
    }
    buff.emit(out_str + ")");
    return reg_result;
}

RegisterTypePtr Generator::addCall(const FuncSymbolTypePtr &func)
{
    auto reg_result = make_shared<RegisterType>(GenerateReg(), func->ret_type);
    string out_str;

    if (func->ret_type == VOID_TYPE)
    {
        out_str = out_str + "call ";
        string llvm_type = findTypeOfLLVM(func->ret_type);
        out_str = out_str + llvm_type;
        out_str = out_str + " @";
        out_str = out_str + func->name;
        out_str = out_str + "()";
        buff.emit(out_str);
        return reg_result;
    }
    else
    {
        out_str = out_str + reg_result->reg_name + " = " + "call ";

        string llvm_type = findTypeOfLLVM(func->ret_type);
        out_str = out_str + llvm_type;
        out_str = out_str + " @";
        out_str = out_str + func->name;
        out_str = out_str + "()";
        buff.emit(out_str);
        return reg_result;
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

    auto dyn_cast_num_exp = dynamic_pointer_cast<NumberType>(exp);
    auto dyn_cast_reg_exp = dynamic_pointer_cast<RegisterType>(exp);
    auto dyn_cast_str_exp = dynamic_pointer_cast<StringType>(exp);
    auto dyn_cast_sym_exp = dynamic_pointer_cast<SymbolType>(exp);

    if (dyn_cast_num_exp)
    {
        return to_string(dyn_cast_num_exp->token);
    }
    else if (dyn_cast_sym_exp)
    {
        return addLoadReg(dyn_cast_sym_exp->offset, dyn_cast_sym_exp->generation_type)->reg_name;
    }
    else if (dyn_cast_reg_exp)
    {
        return dyn_cast_reg_exp->reg_name;
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
    auto exp1_value = findNoBoolExpStr(exp1);
    auto exp2_value = findNoBoolExpStr(exp2);

    auto reg_icmp = GenerateReg();
    string icmp_string(reg_icmp + " = icmp ");

    auto dynamic_cast_relop = dynamic_pointer_cast<StringType>(relop);
    auto relop_str = dynamic_cast_relop->token;

    if (relop_str == "<")
    {
        icmp_string += "slt";
    }
    else if (relop_str == "<=")
    {
        icmp_string += "sle";
    }
    else if (relop_str == ">")
    {
        icmp_string += "sgt";
    }
    else if (relop_str == ">=")
    {
        icmp_string += "sge";
    }
    else if (relop_str == "!=")
    {
        icmp_string += "ne";
    }
    else if (relop_str == "==")
    {
        icmp_string += "eq";
    }
    else
    {
        assert(false);
    }

    icmp_string += " i32 " + exp1_value + ", " + exp2_value;
    buff.emit(icmp_string);
    auto branch_addr = buff.emit("br i1 " + reg_icmp + ", label @, label @");

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
    auto reg_result = GenerateReg();

    if (exp->generation_type != BOOL_TYPE)
    {
        auto exp_str = findNoBoolExpStr(exp);
        string out = reg_result;
        out += " = add i32 ";
        out += exp_str;
        out += ", 0";
        buff.emit(out);
    }
    else
    {
        addBoolExpToReg(exp, reg_result);
    }
    addStoreReg(symbol->offset, reg_result);
    return statement;
}



void Generator::addBoolExpToReg(const BaseTypePtr &exp, const name_of_register &reg_result)
{

    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto true_label = buff.genLabel(TRUE_LABEL);
    auto bp_true =  buff.emit(BP_TRUE);
    string to_emit = reg_result;
    auto false_label = buff.genLabel(FALSE_LABEL);
    auto bp_false =  buff.emit(BP_FALSE);
    auto convert_label = buff.genLabel(CONVERT_LABEL);
    auto convert_false_list = Buff::makelist(branch_pair(bp_false, FIRST));
    auto convert_true_list = Buff::makelist(branch_pair(bp_true, FIRST));
    buff.bpatch(dynamic_cast_bool_exp->true_list, true_label);
    buff.bpatch(dynamic_cast_bool_exp->false_list, false_label);
    buff.bpatch(convert_true_list, convert_label);
    buff.bpatch(convert_false_list, convert_label);
    to_emit = to_emit + " = phi i32 [1, %";
    to_emit = to_emit + true_label;
    to_emit = to_emit + "], [0, %";
    to_emit = to_emit + false_label;
    to_emit = to_emit + "]";
    buff.emit(to_emit);
}

void Generator::addFunctionHead(const FuncSymbolTypePtr &symbol)
{

    string emit_string("define ");
    string s_void = "void @";
    string s_i32 = "i32 @";

    if (symbol->ret_type != VOID_TYPE)
    {
        emit_string = emit_string + s_i32;
    }
    else
    {
        emit_string = emit_string + s_void;
    }

   
    for (size_t count = 0; count < symbol->params.size(); ++count) {
    
        if (count == 0)
        {
            emit_string = emit_string + symbol->name;
            emit_string = emit_string + "(";
        }
        if (!(count <= 0))
        {
            emit_string += "i32";
           
        }
        else
        {
            emit_string += ", ";
            emit_string += "i32";
            
        }
    }

    emit_string = emit_string + ")";
    emit_string = emit_string + " {";

    buff.emit(emit_string);

    stack_register = GenerateReg();
    string to_emit = stack_register;
    to_emit = to_emit + " = ";
    to_emit = to_emit + "alloca";
    to_emit = to_emit + " i32, ";
    to_emit = to_emit + "i32 ";
    to_emit = to_emit + to_string(STACK_SIZE);
    buff.emit(to_emit);
}

void Generator::addStoreReg(int offset, const name_of_register &reg_to_store)
{
    assert(!stack_register.empty());
    auto reg_pointer_to_stack = GenerateReg();

    if (!(offset < 0))
    {
        string to_emit = reg_pointer_to_stack;
        string to_emit2 = "store";
        to_emit = to_emit + " = getelementptr";
        to_emit = to_emit + " i32, i32* ";
        to_emit = to_emit + stack_register;
        to_emit = to_emit + ", i32 ";
        to_emit = to_emit + to_string(offset);
        to_emit2 = to_emit2 + " i32 ";
        to_emit2 = to_emit2 + reg_to_store;
        to_emit2 = to_emit2 + ", i32* ";
        to_emit2 = to_emit2 + reg_pointer_to_stack;
        buff.emit(to_emit);
        buff.emit(to_emit2);
    }
}

RegisterTypePtr Generator::addLoadReg(int offset, Ty type)
{
    assert(!stack_register.empty());
    auto reg_result = GenerateReg();
    string out;

    if (!(offset < 0))
    {
        auto reg_stack_offset = GenerateReg();
        out = reg_stack_offset + " = getelementptr i32, i32* ";
        out += stack_register;
        out += ", i32 " + to_string(offset);
        string out2 = reg_result + " = load i32, i32* " + reg_stack_offset;
        buff.emit(out);
        buff.emit(out2);
        return make_shared<RegisterType>(reg_result, type);
    }
    else
    {
        auto reg_argument = "%" + to_string(-offset - 1);
        string out = reg_result;
        out = out + " = add i32 0, ";
        out = out + reg_argument;
        buff.emit(out);
        return make_shared<RegisterType>(reg_result, type);
    }
}

BaseTypePtr Generator::regToBooleanExpression(string &reg_source)
{
    auto reg_bitcast = GenerateReg();
    string to_emit1 = reg_bitcast;
    to_emit1 =  to_emit1  + " = trunc i32 "    ;
    to_emit1 =  to_emit1  +  reg_source   ;
     to_emit1 =  to_emit1  +  " to i1"   ;

    string to_emit2= "br i1 ";
    to_emit2 =  to_emit2  +     reg_bitcast;
    to_emit2 =  to_emit2  +    ", label @," ;
    to_emit2 =  to_emit2  +     " label @"; 
    buff.emit(to_emit1);
    auto branch_addr = buff.emit(to_emit2);

    return make_shared<BoolExpType>(Buff::makelist({branch_addr, FIRST}),
                                    Buff::makelist({branch_addr, SECOND}));
}

StatementTypePtr Generator::addStatCall()
{
    auto statement = make_shared<StatementType>(br_list());
    return statement;
}

StatementTypePtr Generator::addStatRet()
{
    auto statement = make_shared<StatementType>(br_list());
    buff.emit("ret void");
    return statement;
}

StatementTypePtr Generator::addStatRetExpression(const BaseTypePtr &exp)
{
    auto statement = make_shared<StatementType>(br_list());
    string reg_result;

    if (exp->generation_type == BOOL_TYPE)
    {
        reg_result = GenerateReg();
        addBoolExpToReg(exp, reg_result);
    }
    else
    {
        reg_result = findNoBoolExpStr(exp);
    }

    buff.emit("ret " + findTypeOfLLVM(exp->generation_type) + " " + reg_result);
    return statement;
}

StatementTypePtr
Generator::addStatIf(const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                     const BaseTypePtr &if_list_as_statement)
{
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_if_statement = dynamic_pointer_cast<StatementType>(if_statement);
    auto dynamic_cast_if_label = dynamic_pointer_cast<StringType>(if_label);
    auto dynamic_cast_if_list_as_statement = dynamic_pointer_cast<StatementType>(if_list_as_statement);

    // bpatch true
    buff.bpatch(dynamic_cast_bool_exp->true_list, dynamic_cast_if_label->token);

    // merge false and next
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
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_if_statement = dynamic_pointer_cast<StatementType>(if_statement);
    auto dynamic_cast_else_statement = dynamic_pointer_cast<StatementType>(else_statement);
    auto dynamic_cast_if_label = dynamic_pointer_cast<StringType>(if_label);
    auto dynamic_cast_else_label = dynamic_pointer_cast<StringType>(else_label);
    auto dynamic_cast_if_list_as_statement = dynamic_pointer_cast<StatementType>(if_list_as_statement);

    // bpatch true and false
    buff.bpatch(dynamic_cast_bool_exp->true_list, dynamic_cast_if_label->token);
    buff.bpatch(dynamic_cast_bool_exp->false_list, dynamic_cast_else_label->token);

    // merge if's next (includes if_list) and else's next
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
    auto dynamic_cast_start_list_as_statement = dynamic_pointer_cast<StatementType>(start_list_as_statement);
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);
    auto dynamic_cast_while_statement = dynamic_pointer_cast<StatementType>(while_statement);
    auto dynamic_cast_while_head_label = dynamic_pointer_cast<StringType>(while_head_label);
    auto dynamic_cast_while_body_label = dynamic_pointer_cast<StringType>(while_body_label);
    auto dynamic_cast_end_list_as_statement = dynamic_pointer_cast<StatementType>(end_list_as_statement);

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

    validator_ref.table_ref.scope_stack.top()->break_list->push_back({buff.emit("br label @  ; break"), FIRST});

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
    if (curr_func_ret_type == VOID_TYPE)
    {
        buff.emit("ret void");
    }
    else
    {
        buff.emit("ret i32 0");
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
    auto dynamic_cast_bool_exp1 = dynamic_pointer_cast<BoolExpType>(bool_exp1);
    auto dynamic_cast_and_label = dynamic_pointer_cast<StringType>(and_label);
    auto dynamic_cast_bool_exp2 = dynamic_pointer_cast<BoolExpType>(bool_exp2);

    buff.bpatch(dynamic_cast_bool_exp1->true_list, dynamic_cast_and_label->token);

    return make_shared<BoolExpType>(dynamic_cast_bool_exp2->true_list,
                                    Buff::merge(dynamic_cast_bool_exp1->false_list,
                                                dynamic_cast_bool_exp2->false_list));
}

BoolExpTypePtr Generator::addOr(const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2)
{
    auto dynamic_cast_bool_exp1 = dynamic_pointer_cast<BoolExpType>(bool_exp1);
    auto dynamic_cast_or_label = dynamic_pointer_cast<StringType>(or_label);
    auto dynamic_cast_bool_exp2 = dynamic_pointer_cast<BoolExpType>(bool_exp2);

    buff.bpatch(dynamic_cast_bool_exp1->false_list, dynamic_cast_or_label->token);

    return make_shared<BoolExpType>(Buff::merge(dynamic_cast_bool_exp1->true_list,
                                                dynamic_cast_bool_exp2->true_list),
                                    dynamic_cast_bool_exp2->false_list);
}

RegisterTypePtr Generator::addString(const BaseTypePtr &stype_string)
{
    auto dynamic_cast_string = dynamic_pointer_cast<StringType>(stype_string);
    dynamic_cast_string->token = dynamic_cast_string->token.substr(1, dynamic_cast_string->token.size() - 2);
    auto reg_result = make_shared<RegisterType>(GenerateReg(), STRING_TYPE);
    auto reg_global = GenerateGlobalReg();
    auto string_size_string = to_string(dynamic_cast_string->token.size() + 1);

    buff.emitGlobal(
        reg_global + " = constant [" + string_size_string +
        " x i8] c\"" + dynamic_cast_string->token + "\\00\"");

    buff.emit(reg_result->reg_name + " = getelementptr [" + string_size_string +
              " x i8], [" + string_size_string + " x i8]* " + reg_global + ", i32 0, i32 0");

    return reg_result;
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

    auto fake_statement = make_shared<StatementType>(
        Buff::makelist({buff.emit("br label @  ; end of statement"), FIRST}));
    return fake_statement;
}

StatementTypePtr Generator::addBWhileHead()
{
    auto fake_statement = make_shared<StatementType>(
        Buff::makelist({buff.emit("br label @  ; start/end of while"), FIRST}));
    return fake_statement;
}

StatementTypePtr Generator::addPBIfNext()
{
    auto fake_statement = make_shared<StatementType>(
        Buff::makelist({buff.emit("br label @  ; end of if"), FIRST}));
    return fake_statement;
}

BaseTypePtr Generator::addCallExpression(BaseTypePtr call_exp)
{
    auto dynamic_cast_call_exp = dynamic_pointer_cast<RegisterType>(call_exp);
    if (call_exp->generation_type == BOOL_TYPE)
    {
        return regToBooleanExpression(dynamic_cast_call_exp->reg_name);
    }
    else
    {
        return call_exp;
    }
}
