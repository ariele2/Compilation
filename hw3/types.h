#ifndef TYPES_H
#define TYPES_H
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <stack>
#include <cassert>
#include <string>
#include <vector>
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
    Type general_type;
     virtual ~TermianlBase() = default;
    explicit TermianlBase(Type t);

    TermianlBase();
   
};

typedef std::shared_ptr<TermianlBase> Tptr;
typedef std::vector<TermianlBase> ExpList;

class TExpList : public TermianlBase {
public:
    
    TExpList();
    ExpList expression_list;
    explicit TExpList(ExpList &exp_list);
};

typedef std::shared_ptr<TExpList> TExpListPtr;

class CType : public TermianlBase {
public:
    explicit CType(Type t);
};

typedef std::shared_ptr<CType> TypePtr;

class StringType : public TermianlBase {
public:
    std::string token;
    explicit StringType(std::string &token);
};

typedef std::shared_ptr<StringType> StringTypePtr;

class NumberType : public TermianlBase {
public:
    int token;
    explicit NumberType(std::string &token_string);
};

typedef std::shared_ptr<NumberType> NumberTypePtr;

class BoolType : public TermianlBase {
public:
    bool token;
    explicit BoolType(bool token);
};

typedef std::shared_ptr<BoolType> BoolTypePtr;

class AutoType : public TermianlBase {
public:
    std::string token;
    explicit AutoType(std::string &token);
   
};

typedef std::shared_ptr<AutoType> AutoTypePtr;


class SimpleSymbol : public TermianlBase {
public:
    std::string n;
    int offs;
    SimpleSymbol(std::string &name, int offset, Type t);
    virtual ~SimpleSymbol() = default;
};

typedef std::shared_ptr<SimpleSymbol> SymPtr;
typedef std::vector<SimpleSymbol> SimpleSymsList;


class SimpleSymbolList : public TermianlBase {
public:
    SimpleSymsList syms_list;
    SimpleSymbolList();
    explicit SimpleSymbolList(SimpleSymsList &symbols_list);
};

typedef std::shared_ptr<SimpleSymbolList> SymListPtr;

class FuncSymType : public SimpleSymbol {
public:
    SimpleSymsList parameters;
    Type ret_type;
    FuncSymType(std::string &sym_n, Type sym_t, SimpleSymsList &symbols_list);
};



class Scope;
typedef std::shared_ptr<Scope> ScopePtr;
extern void SSListToStrings(SimpleSymsList &symbols_list, std::vector<std::string> &string_vector);
extern std::string TypeToString(Type t);

typedef std::shared_ptr<FuncSymType> FuncSymTypePtr;

#endif // TYPES_H