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

extern const bool PRINT_DEBUG;
extern int yylineno;
extern char textbuff[1024];
extern char *yytext;
extern char *textbuffptr;
extern int yylex();

class Scope;
typedef shared_ptr<Scope> ScopePtr;

enum GeneralType
{
    BOOL_TYPE,
    FUNCTION_TYPE,
    STRING_TYPE,
    VOID_TYPE,
    AUTO_TYPE,
    BYTE_TYPE,
    STATEMENT_TYPE,
    INT_TYPE,
    ERROR_TYPE
};

typedef pair<int, string> p_case_label;
typedef vector<p_case_label> case_label_list;
typedef shared_ptr<case_label_list> case_label_list_ptr;
typedef pair<int, BranchLabelIndex> branch_pair;
typedef vector<branch_pair> branch_list;
typedef shared_ptr<branch_list> branch_list_ptr;
typedef string label_name;
typedef string register_name;
typedef GeneralType Type;

class TerminalBase
{

public:
    explicit TerminalBase(Type type);
    const std::string class_name = "TerminalBase";
    virtual ~TerminalBase() = default;
    Type generation_type;
    TerminalBase();
};

typedef shared_ptr<TerminalBase> BaseTypePtr;
typedef vector<BaseTypePtr> ExpList;

class TExpList : public TerminalBase
{
public:
    TExpList();
    ExpList exp_list;
    const std::string class_name = "TExpList";
    explicit TExpList(ExpList &exp_list);
};

typedef shared_ptr<TExpList> ExpListTypePtr;

class CType : public TerminalBase
{
public:
    const std::string class_name = "CType";
    explicit CType(Type type);
};

typedef shared_ptr<CType> CTypePtr;

class StringType : public TerminalBase
{
public:
    explicit StringType(string &token);
    const std::string class_name = "StringType";
    string t;
};

typedef shared_ptr<StringType> StringTypePtr;

class RegisterType : public TerminalBase
{
public:
    RegisterType(register_name reg_name, Type type);
    const std::string class_name = "RegisterType";
    register_name reg_name;
};

typedef shared_ptr<RegisterType> RegisterTypePtr;

class BoolExpType : public TerminalBase
{
public:
    const std::string class_name = "BoolExpType";
    BoolExpType(branch_list true_list, branch_list false_list);
    branch_list false_list;
    branch_list true_list;
};

typedef shared_ptr<BoolExpType> BoolExpTypePtr;

class AutoType : public TerminalBase
{
public:
    explicit AutoType(std::string &token);
    std::string token;
    const std::string class_name = "AutoType";
};

typedef std::shared_ptr<AutoType> AutoTypePtr;

class StatementType : public TerminalBase
{
public:
    explicit StatementType(branch_list next_list);
    const std::string class_name = "StatementType";
    branch_list next_list;
};

typedef shared_ptr<StatementType> StatementTypePtr;

class NumberType : public TerminalBase
{
public:
    explicit NumberType(string &token_string);
    int token;
    const std::string class_name = "NumberType";
};

typedef shared_ptr<NumberType> NumberTypePtr;

class SymbolType : public TerminalBase
{
    
public:
    SymbolType(string &name, int offset, Type type);
    const std::string class_name = "SymbolType";
    string name;
    int offset;
    
};

typedef vector<SymbolType> ArgList;
typedef shared_ptr<SymbolType> SymbolTypePtr;

class ArgListType : public TerminalBase
{
public:
explicit ArgListType(ArgList &arg_list);
    ArgListType();
    ArgList arguments_list;
    const std::string class_name = "ArgListType";
};

typedef shared_ptr<ArgListType> ArgListTypePtr;

class FuncSymType : public SymbolType
{
public:
    
    const std::string class_name = "FuncSymType";
    Type ret_type;
    FuncSymType(string &symbol_name, Type symbol_type, ArgList &arg_list);
    ArgList params;
    
};

typedef shared_ptr<FuncSymType> FuncSymbolTypePtr;

class CaseListType : public TerminalBase
{
public:
    CaseListType(case_label_list case_list, string default_label, branch_list next_list);
    case_label_list case_list;
    const std::string class_name = "CaseListType";
    branch_list next_list;
    string default_label;
};

typedef shared_ptr<CaseListType> CaseListTypePtr;

class CaseDeclType : public TerminalBase
{
public:
    CaseDeclType(int case_num, string case_label, branch_list next_list);
    string case_label;
    const std::string class_name = "CaseDeclType";
    branch_list next_list;
    int case_num;
   
};

typedef shared_ptr<CaseDeclType> CaseDeclTypePtr;

extern string TypeToString(Type type);

extern void ArgListToStrings(ArgList &arg_list, vector<string> &string_vector);



#endif // HWw3_TYPEDEFS_H