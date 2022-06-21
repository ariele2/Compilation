#ifndef TYPES_H
#define TYPES_H

#include <cstdlib>
#include <iostream>
#include <stack>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "bp.hpp"

#define YYSTYPE BaseTypePtr


using std::endl;
using std::shared_ptr;
using std::to_string;
using std::cout;
using std::unordered_map;
using std::dynamic_pointer_cast;
using std::vector;
using std::move;
using std::stack;
using std::string;
using std::pair;



extern const bool PRINT_DEBUG;
extern int yylineno;
extern char textbuff[1024];
extern char *yytext;
extern char *textbuffptr;
extern int yylex();

class Scope;
typedef shared_ptr<Scope> ScopePointer;

enum GeneralType
{
    STATEMENT_TYPE,
    INT_TYPE,
    AUTO_TYPE,
    STRING_TYPE,
    VOID_TYPE,
    BYTE_TYPE,
    BOOL_TYPE,
    FUNCTION_TYPE,
    ERROR_TYPE
};



typedef pair<int, BranchLabelIndex> branch_pair;
typedef vector<branch_pair> br_list;
typedef shared_ptr<br_list> br_list_pointer;
typedef string label_of_name;
typedef string name_of_register;
typedef GeneralType Ty;

class TerminalBase
{

public:
    explicit TerminalBase(Ty type);
    const std::string class_name = "TerminalBase";
    virtual ~TerminalBase() = default;
    Ty generation_type;
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
    explicit CType(Ty type);
};

typedef shared_ptr<CType> CTypePtr;

class StringType : public TerminalBase
{
public:
    explicit StringType(string &token);
    const std::string class_name = "StringType";
    string token;
};

typedef shared_ptr<StringType> StringTypePtr;

class RegisterType : public TerminalBase
{
public:
    RegisterType(name_of_register reg_name, Ty type);
    const std::string class_name = "RegisterType";
    name_of_register reg_name;
};

typedef shared_ptr<RegisterType> RegisterTypePtr;

class BoolExpType : public TerminalBase
{
public:
    const std::string class_name = "BoolExpType";
    BoolExpType(br_list true_list, br_list false_list);
    br_list false_list;
    br_list true_list;
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
    explicit StatementType(br_list next_list);
    const std::string class_name = "StatementType";
    br_list next_list;
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
    SymbolType(string &name, int offset, Ty type);
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
    Ty ret_type;
    FuncSymType(string &symbol_name, Ty symbol_type, ArgList &arg_list);
    ArgList params;
    
};

typedef shared_ptr<FuncSymType> FuncSymbolTypePtr;

extern string TypeToString(Ty type);

extern void ArgListToStrings(ArgList &arg_list, vector<string> &string_vector);



#endif // TYPES_H