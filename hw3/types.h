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

#define YYSTYPE Tptr

extern int yylineno;
extern char *yytext;

extern int yylex();

enum GeneralType
{

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

class TermianlBase
{

public:
    Type general_type;
     virtual ~TermianlBase() = default;
    explicit TermianlBase(Type t);

    TermianlBase();
   
};

typedef std::shared_ptr<TermianlBase> Tptr;
typedef std::vector<TermianlBase> ExpList;

class TExpList : public TermianlBase
{
public:
    
    TExpList();
    ExpList expression_list;
    explicit TExpList(ExpList &exp_list);
};

typedef std::shared_ptr<TExpList> TExpListPtr;

class STypeCType : public TermianlBase
{
public:
    explicit STypeCType(Type t);
};

typedef std::shared_ptr<STypeCType> TypePtr;

class STypeString : public TermianlBase
{
public:
    std::string token;
    explicit STypeString(std::string &token);
};

typedef std::shared_ptr<STypeString> StringTypePtr;

class STypeNumber : public TermianlBase
{
public:
    int token;
    explicit STypeNumber(std::string &token_string);
};

typedef std::shared_ptr<STypeNumber> NumberTypePtr;

class STypeBool : public TermianlBase
{
public:
    bool token;
    explicit STypeBool(bool token);
};

typedef std::shared_ptr<STypeBool> BoolTypePtr;

class AutoType : public TermianlBase
{
public:
    std::string token;
    explicit AutoType(std::string &token);
   
};

typedef std::shared_ptr<AutoType> AutoTypePtr;


class SimpleSymbol : public TermianlBase
{
public:
    std::string n;
    int offs;
    SimpleSymbol(std::string &name, int offset, Type t);
    virtual ~SimpleSymbol() = default;
};

typedef std::shared_ptr<SimpleSymbol> SymbolPtr;
typedef std::vector<SimpleSymbol> SSList;


class SimpleSymbolList : public TermianlBase
{
public:
    SSList syms_list;
    SimpleSymbolList();
    explicit SimpleSymbolList(SSList &symbols_list);
};

typedef std::shared_ptr<SimpleSymbolList> SymListPtr;

// function symbol should have list of argumets and a return t in addition to the function name, offset and the general t
class STypeFunctionSymbol : public SimpleSymbol
{
public:
    SSList parameters;
    Type ret_type;
    STypeFunctionSymbol(std::string &symbol_name, Type symbol_type, SSList &symbols_list);
};

typedef std::shared_ptr<STypeFunctionSymbol> STypeFunctionSymbolPtr;

extern std::string TypeToString(Type t);
extern void SSListToStrings(SSList &symbols_list, std::vector<std::string> &string_vector);

class Scope;
typedef std::shared_ptr<Scope> ScopePtr;

#endif // TYPES_H