#include "code_gen.h"

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

void Generator::addGlobalFuncs()
{

    buff.emitGlobal("declare i32 @printf(i8*, ...)");

    // exit
    buff.emitGlobal("declare void @exit(i32)");

    // print
    buff.emitGlobal(R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")");
    buff.emitGlobal("define void @print(i8*) {");
    buff.emitGlobal("    %spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0");
    buff.emitGlobal("    call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)");
    buff.emitGlobal("    ret void");
    buff.emitGlobal("}");

    // printi
    buff.emitGlobal(R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")");
    buff.emitGlobal("define void @printi(i32) {");
    buff.emitGlobal("    %spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0");
    buff.emitGlobal("    call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)");
    buff.emitGlobal("    ret void");
    buff.emitGlobal("}");

    // zero
    buff.emitGlobal(R"(@err_zero = constant [23 x i8] c"Error division by zero\00")");
}

RegisterTypePtr Generator::addBinop(const BaseTypePtr &exp1, string binop, const BaseTypePtr &exp2)
{
    auto dynamic_cast_exp1 = findNotBooleanExpressionStr(exp1);
    auto dynamic_cast_exp2 = findNotBooleanExpressionStr(exp2);

    auto reg_result = make_shared<RegisterType>(GenerateReg(), INT_TYPE);

    if (binop == "/")
    {
        addCheckDivZero(exp2);
    }

    string binop_str = reg_result->reg_name + " = ";

    switch (binop[0]) {
        case '+':
            binop_str += "add ";
            break;
        case '-':
            binop_str += "sub ";
            break;
        case '*':
            binop_str += "mul ";
            break;
        case '/':
            binop_str += "sdiv ";
            break;
    }


    // all types are i32
    binop_str += "i32 ";

    binop_str += dynamic_cast_exp1;

    binop_str += ", ";

    binop_str += dynamic_cast_exp2;

    buff.emit(binop_str);

    if (BYTE_TYPE == validator_ref.CheckAndGetBinOpType(exp1->generation_type, exp2->generation_type))
    {

        auto reg_old_result = reg_result->reg_name;
        reg_result->reg_name = GenerateReg();
        reg_result->generation_type = BYTE_TYPE;
        buff.emit(reg_result->reg_name + " = and i32 " + reg_old_result + ", 255");
    }

    return reg_result;
}

void Generator::addCheckDivZero(const BaseTypePtr &exp)
{

    auto dynamic_cast_num_exp = dynamic_pointer_cast<NumberType>(exp);
    auto dynamic_cast_reg_exp = dynamic_pointer_cast<RegisterType>(exp);

    auto is_zero = GenerateReg();
    auto err_str = GenerateReg();

      if (dynamic_cast_reg_exp) {
        buff.emit(is_zero + " = icmp eq i32 " + dynamic_cast_reg_exp->reg_name + ", 0");
    } else {
        buff.emit(is_zero + " = icmp eq i32 " + to_string(dynamic_cast_num_exp->token) + ", 0");
    }

    
    auto bp_zero_and_non = buff.emit("br i1 " + is_zero + ", label @, label @");
    auto label_err_zero = buff.genLabel("_err_zero");
    vector<pair<int, BranchLabelIndex>> bp_zero_vec;
    bp_zero_vec.emplace_back(bp_zero_and_non, FIRST);
    buff.bpatch(bp_zero_vec, label_err_zero);

    buff.emit(err_str + " = getelementptr [23 x i8], [23 x i8]* @err_zero, i32 0, i32 0");
    buff.emit("call void (i8*) @print(i8* " + err_str + ")");
    buff.emit("call void (i32) @exit(i32 0)");
    buff.emit("unreachable");  // this prevents no branch before label error

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
    out_str += llvm_type + " @" + func->name + "(";

    for (size_t i = 0; i < exp_list->exp_list.size(); ++i)
    {
        if (i > 0)
        {
            out_str += ", ";
        }
        out_str += findTypeOfLLVM(exp_list->exp_list[i]->generation_type) + " ";

        {
            out_str += findNotBooleanExpressionStr(exp_list->exp_list[i]);
        }
    }
    out_str += ")";

    buff.emit(out_str);
    return reg_result;
}

RegisterTypePtr Generator::addCall(const FuncSymbolTypePtr &func)
{
    auto reg_result = make_shared<RegisterType>(GenerateReg(), func->ret_type);
    string out_str;

    if (func->ret_type != VOID_TYPE)
    {
        out_str += reg_result->reg_name + " = ";
    }

    out_str += "call ";
    string llvm_type = findTypeOfLLVM(func->ret_type);

    out_str += llvm_type + " @" + func->name + "()";

    buff.emit(out_str);
    return reg_result;
}

string Generator::findTypeOfLLVM(const Ty &type)
{
    string llvm_type;
    switch (type)
    {
    case VOID_TYPE:
        llvm_type = "void";
        break;
    case STRING_TYPE:
        llvm_type = "i8*";
        break;
    default:
        llvm_type = "i32";
        break;
    }
    return llvm_type;
}

string Generator::findNotBooleanExpressionStr(const BaseTypePtr &exp)
{
    // supports bools that are in register representation
    auto dynamic_cast_num_exp = dynamic_pointer_cast<NumberType>(exp);
    auto dynamic_cast_reg_exp = dynamic_pointer_cast<RegisterType>(exp);
    auto dynamic_cast_str_exp = dynamic_pointer_cast<StringType>(exp);
    auto dynamic_cast_sym_exp = dynamic_pointer_cast<SymbolType>(exp);

    if (dynamic_cast_num_exp)
    {
        return to_string(dynamic_cast_num_exp->token);
    }
    else if (dynamic_cast_reg_exp)
    {
        return dynamic_cast_reg_exp->reg_name;
    }
    else if (dynamic_cast_sym_exp)
    {
        return addLoadReg(dynamic_cast_sym_exp->offset, dynamic_cast_sym_exp->generation_type)->reg_name;
    }
    else
    {
        assert(false);
    }
}

void Generator::addProg()
{
    if (PRINT_DEBUG)
    {
        cout << "code starts here:\n";
    }
    buff.printGlobalBuffer();
    buff.printCodeBuffer();
}

BoolExpTypePtr Generator::addRelOp(const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2)
{
    auto exp1_value = findNotBooleanExpressionStr(exp1);
    auto exp2_value = findNotBooleanExpressionStr(exp2);

    auto reg_icmp = GenerateReg();
    string icmp_string(reg_icmp + " = icmp ");

    auto dynamic_cast_relop = dynamic_pointer_cast<StringType>(relop);
    auto relop_str = dynamic_cast_relop->token;

    if (relop_str == "<")
    {
        icmp_string += "slt";
    }
    else if (relop_str == ">")
    {
        icmp_string += "sgt";
    }
    else if (relop_str == "<=")
    {
        icmp_string += "sle";
    }
    else if (relop_str == ">=")
    {
        icmp_string += "sge";
    }
    else if (relop_str == "==")
    {
        icmp_string += "eq";
    }
    else if (relop_str == "!=")
    {
        icmp_string += "ne";
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

    // create default value and store in stack
    // r = 0+0 (including bool)
    auto reg_default_value = GenerateReg();
    buff.emit(reg_default_value + " = add i32 0, 0");

    addStoreReg(symbol->offset, reg_default_value);

    return statement;
}

StatementTypePtr Generator::addStatAssign(string id, const BaseTypePtr &exp)
{
    auto symbol = validator_ref.table_ref.GetDefinedSymbol(id);
    auto statement = make_shared<StatementType>(br_list());
    auto reg_result = GenerateReg();

    if (exp->generation_type == BOOL_TYPE)
    {
        addBooleanExpressionToRegister(exp, reg_result);
    }
    else
    {
        auto exp_str = findNotBooleanExpressionStr(exp);
        buff.emit(reg_result + " = add i32 " + exp_str + ", 0");
    }

    // store and return
    addStoreReg(symbol->offset, reg_result);

    return statement;
}

void Generator::addBooleanExpressionToRegister(const BaseTypePtr &exp, const name_of_register &reg_result)
{
    auto dynamic_cast_bool_exp = dynamic_pointer_cast<BoolExpType>(exp);

    // create phi junction
    auto true_label = buff.genLabel("_convert_true");
    auto bp_true = buff.emit("br label @");
    auto false_label = buff.genLabel("_convert_false");
    auto bp_false = buff.emit("br label @");
    auto convert_label = buff.genLabel("_convert_final");

    auto convert_true_list = Buff::makelist(branch_pair(bp_true, FIRST));
    auto convert_false_list = Buff::makelist(branch_pair(bp_false, FIRST));

    buff.bpatch(dynamic_cast_bool_exp->true_list, true_label);
    buff.bpatch(dynamic_cast_bool_exp->false_list, false_label);
    buff.bpatch(convert_true_list, convert_label);
    buff.bpatch(convert_false_list, convert_label);

    buff.emit(reg_result + " = phi i32 [1, %" + true_label + "], [0, %" + false_label + "]");
}

void Generator::addFunctionHead(const FuncSymbolTypePtr &symbol)
{
    // emit definition
    string emit_string("define ");

    if (symbol->ret_type == VOID_TYPE)
    {
        emit_string += "void @";
    }
    else
    {
        emit_string += "i32 @";
    }

    emit_string += symbol->name;
    emit_string += "(";

    for (size_t i = 0; i < symbol->params.size(); ++i)
    {
        if (i > 0)
        {
            emit_string += ", ";
        }
        emit_string += "i32";
    }

    emit_string += ") {";

    buff.emit(emit_string);

    // allocate stack
    stack_register = GenerateReg();
    buff.emit(stack_register + " = alloca i32, i32 " + to_string(STACK_SIZE));
}

void Generator::addStoreReg(int offset, const name_of_register &reg_to_store)
{
    assert(!stack_register.empty());
    auto reg_pointer_to_stack = GenerateReg();

    if (offset >= 0)
    {
        // get address on stack
        buff.emit(reg_pointer_to_stack + " = getelementptr i32, i32* " + stack_register +
                  ", i32 " + to_string(offset));

        // store in stack
        buff.emit("store i32 " + reg_to_store + ", i32* " + reg_pointer_to_stack);
    }
}

RegisterTypePtr Generator::addLoadReg(int offset, Ty type)
{
    assert(!stack_register.empty());

    auto reg_result = GenerateReg();

    if (offset >= 0)
    {
        // non-negative offset - calculate real address and load
        auto reg_stack_offset = GenerateReg();

        buff.emit(reg_stack_offset + " = getelementptr i32, i32* " +
                  stack_register + ", i32 " + to_string(offset));
        buff.emit(reg_result + " = load i32, i32* " + reg_stack_offset);
    }
    else
    {
        // negative offset - get func register and assign
        auto reg_argument = "%" + to_string(-offset - 1);
        buff.emit(reg_result + " = add i32 0, " + reg_argument);
    }

    return make_shared<RegisterType>(reg_result, type);
}

BaseTypePtr Generator::regToBooleanExpression(string &reg_source)
{
    auto reg_bitcast = GenerateReg();

    buff.emit(
        reg_bitcast + " = trunc i32 " + reg_source + " to i1");

    auto branch_addr = buff.emit("br i1 " + reg_bitcast + ", label @, label @");

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
        addBooleanExpressionToRegister(exp, reg_result);
    }
    else
    {
        reg_result = findNotBooleanExpressionStr(exp);
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
    dynamic_cast_string->token = dynamic_cast_string->token.substr(1,dynamic_cast_string->token.size()-1);
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
