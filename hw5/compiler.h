#ifndef HW3_PARSE_UTILS_H
#define HW3_PARSE_UTILS_H

#include "typedefs.h"
#include "symbol_table.h"
#include "hw3_output.hpp"
#include "semantics.h"
#include "code_gen.h"
#include "bp.hpp"

using namespace output;

void yyerror(const char *);

// note: maybe it's wiser to create a `Parser` class which is stored here and move the functions there.
// it would require a lot of new functions though.

// note: should've removed all the lineno args and used the global instead.
class Compiler {
public:
    SymbolTable symbol_table;
    SemanticChecks semantic_checks;
    CodeGen code_gen;

    Compiler();

    void ParseProgram(int lineno);

    void ParseFuncs(int lineno);

    void ParseFuncDecl(int lineno, const BaseTypePtr &statements, const BaseTypePtr &next_label);

    void ParseFuncHead(int lineno, const BaseTypePtr &ret_type, const BaseTypePtr &id, const BaseTypePtr &formals);

    static BaseTypePtr ParseRetType(int lineno, BaseTypePtr type);

    static CTypePtr ParseRetType(int lineno);

    static ArgListTypePtr ParseFormals(int lineno);

    static ArgListTypePtr ParseFormals(int lineno, const BaseTypePtr &formals);

    static ArgListTypePtr ParseFormalsList(int lineno, const BaseTypePtr &formal);

    static ArgListTypePtr ParseFormalsList(int lineno, const BaseTypePtr &formal, const BaseTypePtr &formals_list);

    SymbolTypePtr ParseFormalDecl(int lineno, const BaseTypePtr &type, const BaseTypePtr &id);

    static StatementTypePtr
    ParseStatements(int lineno, BaseTypePtr &statement, const BaseTypePtr &next_list_as_statement);

    StatementTypePtr
    ParseStatements(int lineno, const BaseTypePtr &statements, const BaseTypePtr &old_next_list_as_statement,
                    const BaseTypePtr &next_label,
                    const BaseTypePtr &next_statement, const BaseTypePtr &my_next_list_as_statement);

    static StatementTypePtr ParseStatementOfStatements(int lineno, BaseTypePtr &statements);

    StatementTypePtr ParseStatementType(int lineno, const BaseTypePtr &type, const BaseTypePtr &id);

    StatementTypePtr
    ParseStatementTypeAssign(int lineno, const BaseTypePtr &type, const BaseTypePtr &id, const BaseTypePtr &exp);

    StatementTypePtr ParseStatementAssign(int lineno, const BaseTypePtr &id, const BaseTypePtr &exp);

    static StatementTypePtr ParseStatementCall(int lineno);

    StatementTypePtr ParseStatementReturn(int lineno);

    StatementTypePtr ParseStatementReturnExp(int lineno, const BaseTypePtr &exp);

    StatementTypePtr
    ParseStatementIf(int lineno, const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                     const BaseTypePtr &if_list_as_statement);

    StatementTypePtr
    ParseStatementIfElse(int lineno, const BaseTypePtr &exp, const BaseTypePtr &if_label, const BaseTypePtr &if_statement,
                         BaseTypePtr if_list_as_statement, const BaseTypePtr &else_label,
                         const BaseTypePtr &else_statement);

    StatementTypePtr
    ParseStatementWhile(int lineno, BaseTypePtr start_list_as_statement, const BaseTypePtr &while_head_label,
                        const BaseTypePtr &exp,
                        const BaseTypePtr &while_body_label, const BaseTypePtr &while_statement,
                        const BaseTypePtr &end_list_as_statement);

    StatementTypePtr ParseStatementBreak(int lineno);

    StatementTypePtr ParseStatementContinue(int lineno);

    StatementTypePtr
    ParseStatementSwitch(int lineno, BaseTypePtr exp, BaseTypePtr switch_list_as_statement, BaseTypePtr case_list);

    BaseTypePtr ParseCall(int lineno, const BaseTypePtr &id, const BaseTypePtr &exp_list);

    BaseTypePtr ParseCall(int lineno, const BaseTypePtr &id);

    static BaseTypePtr ParseExplist(int lineno, const BaseTypePtr &exp);

    static BaseTypePtr ParseExplist(int lineno, const BaseTypePtr &exp, const BaseTypePtr &exp_list);

    static CTypePtr ParseInt(int lineno);

    static CTypePtr ParseByte(int lineno);

    static CTypePtr ParseBool(int lineno);

    static BaseTypePtr ParseParentheses(int lineno, BaseTypePtr exp);

    RegisterTypePtr ParseBinop(int lineno, const BaseTypePtr &exp1, BaseTypePtr &binop, const BaseTypePtr &exp2);

    BaseTypePtr ParseID(int lineno, const BaseTypePtr &id);

    BaseTypePtr ParseCallExp(int lineno, BaseTypePtr call_exp);

    static NumberTypePtr ParseNum(int lineno, const BaseTypePtr &num);

    BaseTypePtr ParseNumB(int lineno, BaseTypePtr num);

    RegisterTypePtr ParseString(int lineno, const BaseTypePtr &stype_string);

    BoolExpTypePtr ParseTrue(int lineno);

    BoolExpTypePtr ParseFalse(int lineno);

    BoolExpTypePtr ParseNot(int lineno, const BaseTypePtr &bool_exp);

    BoolExpTypePtr
    ParseAnd(int lineno, const BaseTypePtr &bool_exp1, const BaseTypePtr &and_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr ParseOr(int lineno, const BaseTypePtr &bool_exp1, const BaseTypePtr &or_label, const BaseTypePtr &bool_exp2);

    BoolExpTypePtr ParseRelOp(int lineno, const BaseTypePtr &exp1, BaseTypePtr &relop, const BaseTypePtr &exp2);

    StringTypePtr ParseGenIfLabel(int lineno);

    StringTypePtr ParseGenElseLabel(int lineno);

    StringTypePtr ParseGenWhileHeadLabel(int lineno);

    StringTypePtr ParseGenWhileBodyLabel(int lineno);

    StringTypePtr ParseGenAndLabel(int lineno);

    StringTypePtr ParseGenOrLabel(int lineno);

    StringTypePtr ParseGenNextLabel(int lineno);

    StringTypePtr ParseGenDefaultLabel(int lineno);

    StringTypePtr ParseGenCaseDeclLabel(int lineno);

    CaseListTypePtr ParseCaseList(int lineno, BaseTypePtr case_decl, BaseTypePtr next_list, BaseTypePtr case_list);

    CaseListTypePtr ParseCaseList(int lineno, BaseTypePtr case_decl);

    CaseListTypePtr
    ParseCaseDefault(int lineno, BaseTypePtr list_as_statement, BaseTypePtr default_label, BaseTypePtr statements);

    CaseDeclTypePtr ParseCaseDecl(int lineno, BaseTypePtr num, BaseTypePtr list_as_statement, BaseTypePtr case_decl_label,
                                   BaseTypePtr statements);

    void ParsePushStatementScope(int lineno);

    void ParsePushWhileScope(int lineno, const BaseTypePtr &while_head_label);

    void ParsePushSwitchScope(int lineno, const BaseTypePtr &switch_head_label);

    void ParsePopScope(int lineno);

    void ParseCheckSwitchExp(int lineno, const BaseTypePtr &num_exp);

    void ParseCheckBool(int lineno, const BaseTypePtr &bool_exp);

    StatementTypePtr ParseBranchNext(int lineno);

    StatementTypePtr ParseBranchWhileHead(int lineno);

    StatementTypePtr ParseBranchIfNext(int lineno);

    StatementTypePtr ParseBranchSwitchHead(int lineno);

    StatementTypePtr ParseBranchCaseHead(int lineno);

    StatementTypePtr ParseBranchDefaultHead(int lineno);

    BaseTypePtr ParseConvertBool(int lineno, BaseTypePtr exp);

    static Compiler &instance();

};


#endif //HW3_PARSE_UTILS_H