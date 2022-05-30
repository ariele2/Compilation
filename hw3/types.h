#ifndef TYPES_H
#define TYPES_H
#include <iostream>
#include <stack>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <memory>
#include "hw3_output.hpp"

using namespace output;
#define YYSTYPE Tptr

extern int yylineno;
extern char *yytext;

extern int yylex();

enum GeneralType {
    BOOL_TYPE,
    FUNCTION_TYPE,
    STRING_TYPE,
    VOID_TYPE,
    AUTO_TYPE,
    BYTE_TYPE,
    INT_TYPE,
    OTHER_TYPE
};

typedef GeneralType Type;

class TermianlBase {
public:
    Type g_type;
     virtual ~TermianlBase() = default;
    explicit TermianlBase(Type t);
    const std::string class_name = "TerminalBase";

    TermianlBase();
   
};

typedef std::shared_ptr<TermianlBase> Tptr;
typedef std::vector<TermianlBase> ExpList;

class TExpList : public TermianlBase {
public:
    const std::string class_name = "TExpList";
    TExpList();
    ExpList expression_list;
    explicit TExpList(ExpList &exp_list);
};

typedef std::shared_ptr<TExpList> TExpListPtr;

class CType : public TermianlBase {
public:
    explicit CType(Type t);
    const std::string class_name = "CType";
};

typedef std::shared_ptr<CType> TypePtr;

class StringType : public TermianlBase {
public:
    std::string token;
    const std::string class_name = "StringType";
    explicit StringType(std::string &token);
};

typedef std::shared_ptr<StringType> StringTypePtr;

class NumberType : public TermianlBase {
public:
    int token;
    const std::string class_name = "NumberType";
    explicit NumberType(std::string &token_string);
};

typedef std::shared_ptr<NumberType> NumberTypePtr;

class BoolType : public TermianlBase {
public:
    bool token;
    const std::string class_name = "BoolType";
    explicit BoolType(bool token);
};

typedef std::shared_ptr<BoolType> BoolTypePtr;

class AutoType : public TermianlBase {
public:
    std::string token;
    const std::string class_name = "AutoType";
    explicit AutoType(std::string &token);
   
};

typedef std::shared_ptr<AutoType> AutoTypePtr;


class SimpleSymbol : public TermianlBase {
public:
    std::string n;
    int offs;
    const std::string class_name = "SimpleSymbol";
    SimpleSymbol(std::string &name, int offset, Type t);
    virtual ~SimpleSymbol() = default;
};

typedef std::shared_ptr<SimpleSymbol> SymPtr;
typedef std::vector<SimpleSymbol> SimpleSymsList;


class SimpleSymbolList : public TermianlBase {
public:
    SimpleSymsList syms_list;
    const std::string class_name = "SimpleSymbolList";
    SimpleSymbolList();
    explicit SimpleSymbolList(SimpleSymsList &symbols_list);
};

typedef std::shared_ptr<SimpleSymbolList> SymListPtr;

class FuncSymType : public SimpleSymbol {
public:
    SimpleSymsList parameters;
    Type ret_type;
    const std::string class_name = "FuncSymType";
    FuncSymType(std::string &sym_n, Type sym_t, SimpleSymsList &symbols_list);
};



class Scope;
typedef std::shared_ptr<Scope> ScopePtr;
extern void SSListToStrings(SimpleSymsList &symbols_list, std::vector<std::string> &string_vector);
extern std::string TypeToString(Type t);

typedef std::shared_ptr<FuncSymType> FuncSymTypePtr;

#endif // TYPES_H