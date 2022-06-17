#ifndef HW3_TYPEDEFS_H
#define HW3_TYPEDEFS_H

#include <cstdlib>
#include <iostream>
#include <stack>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "bp.hpp"

#define YYSTYPE BaseTypePtr

using std::cout;
using std::dynamic_pointer_cast;
using std::endl;
using std::move;
using std::pair;
using std::shared_ptr;
using std::stack;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

extern int yylineno;
extern char *yytext;

extern int yylex();

extern char textbuff[1024];
extern char *textbuffptr;
extern const bool PRINT_DEBUG;

enum GeneralType
{
    VOID_TYPE,
    INT_TYPE, // this might be a number literal or a variable (same for the others)
    BYTE_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    FUNCTION_TYPE,
    STATEMENT_TYPE,
    ERROR_TYPE
};

typedef GeneralType Type;
typedef string register_name;
typedef string label_name;

typedef pair<int, BranchLabelIndex> branch_pair;
typedef vector<branch_pair> branch_list;
typedef shared_ptr<branch_list> branch_list_ptr;

typedef pair<int, string> case_label_pair;
typedef vector<case_label_pair> case_label_list;
typedef shared_ptr<case_label_list> case_label_list_ptr;


class TerminalBase
{
    
public:
    explicit TerminalBase(Type type); 

    Type g_type;

    TerminalBase();

    virtual ~TerminalBase() = default;

    const std::string class_name = "TerminalBase";
};

typedef shared_ptr<TerminalBase> BaseTypePtr;
typedef vector<BaseTypePtr> ExpList;

class TExpList : public TerminalBase
{
public:
    ExpList exp_list;

    const std::string class_name = "TExpList";

    TExpList();

    explicit TExpList(ExpList &exp_list);
};

typedef shared_ptr<TExpList> ExpListTypePtr;

class CType : public TerminalBase
{
public:
    explicit CType(Type type);
    const std::string class_name = "CType";
};

typedef shared_ptr<CType> CTypePtr;

class StringType : public TerminalBase
{
public:
    string token;

    explicit StringType(string &token);
    const std::string class_name = "StringType";
};

typedef shared_ptr<StringType> StringTypePtr;

class RegisterType : public TerminalBase
{
public:
    register_name reg_name;

    RegisterType(register_name reg_name, Type type);
    const std::string class_name = "RegisterType";
};

typedef shared_ptr<RegisterType> RegisterTypePtr;

class BoolExpType : public TerminalBase
{
public:
    branch_list true_list;
    branch_list false_list;
    const std::string class_name = "BoolExpType";

    BoolExpType(branch_list true_list, branch_list false_list);
};

typedef shared_ptr<BoolExpType> BoolExpTypePtr;

class AutoType : public TerminalBase
{
public:
    std::string token;
    const std::string class_name = "AutoType";
    explicit AutoType(std::string &token);
};

typedef std::shared_ptr<AutoType> AutoTypePtr;

class StatementType : public TerminalBase
{
public:
    branch_list next_list;
    const std::string class_name = "StatementType";

    explicit StatementType(branch_list next_list);
};

typedef shared_ptr<StatementType> StatementTypePtr;

class NumberType : public TerminalBase
{
public:
    int token;
    const std::string class_name = "NumberType";

    explicit NumberType(string &token_string);
};

typedef shared_ptr<NumberType> NumberTypePtr;

class SymbolType : public TerminalBase
{
    // no virtual method needed
public:
    string name;
    int offset;
    const std::string class_name = "SymbolType";

    SymbolType(string &name, int offset, Type type);
};

typedef shared_ptr<SymbolType> SymbolTypePtr;
typedef vector<SymbolType> ArgList;

class ArgListType : public TerminalBase
{
public:
    ArgList arg_list;
    const std::string class_name = "ArgListType";

    ArgListType();

    explicit ArgListType(ArgList &arg_list);
};

typedef shared_ptr<ArgListType> ArgListTypePtr;

class FuncSymType : public SymbolType
{
public:
    ArgList params;
    Type ret_type;
    const std::string class_name = "FuncSymType";

    FuncSymType(string &symbol_name, Type symbol_type, ArgList &arg_list);
};

typedef shared_ptr<FuncSymType> FuncSymbolTypePtr;

class CaseListType : public TerminalBase
{
public:
    case_label_list case_list;
    string default_label;
    branch_list next_list;
    const std::string class_name = "CaseListType";

    CaseListType(case_label_list case_list, string default_label, branch_list next_list);
};

typedef shared_ptr<CaseListType> CaseListTypePtr;

class CaseDeclType : public TerminalBase
{
public:
    int case_num;
    string case_label;
    branch_list next_list;
    const std::string class_name = "CaseDeclType";

    CaseDeclType(int case_num, string case_label, branch_list next_list);
};

typedef shared_ptr<CaseDeclType> CaseDeclTypePtr;

extern string TypeToString(Type type);

extern void ArgListToStrings(ArgList &arg_list, vector<string> &string_vector);

class Scope;

typedef shared_ptr<Scope> ScopePtr;

#endif // HWw3_TYPEDEFS_H