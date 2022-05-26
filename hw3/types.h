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



enum GeneralTypeEnum {
    VOID_TYPE,
    INT_TYPE,  // this might be a number literal or a variable (same for the others)
    BYTE_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    FUNCTION_TYPE,
    OTHER_TYPE
};

typedef GeneralTypeEnum Type;

class TBase {
    // must have at least one virtual method
public:
    explicit TBase(Type type);  // also used for expressions and types
    Type general_type;
    TBase();
    virtual ~TBase() = default;
};

typedef std::shared_ptr<TBase> Tptr;
typedef std::vector<TBase> ExpList;

class STypeExpList : public TBase {
public:
    ExpList exp_list;
    STypeExpList();
    explicit STypeExpList(ExpList &exp_list);
};

typedef std::shared_ptr<STypeExpList> STypeExpListPtr;

class STypeCType : public TBase {
public:
    explicit STypeCType(Type type);
};

typedef std::shared_ptr<STypeCType> TypePtr;

class STypeString : public TBase {
public:
    std::string token;
    explicit STypeString(std::string& token);
};

typedef std::shared_ptr<STypeString> StringTypePtr;

class STypeNumber : public TBase {
public:
    int token;
    explicit STypeNumber(std::string& token_string);
};

typedef std::shared_ptr<STypeNumber> NumberTypePtr;

class STypeBool : public TBase {
public:
    bool token;
    explicit STypeBool(bool token);
};

typedef std::shared_ptr<STypeBool> BoolTypePtr;


// its the base of a symbol inside the table which has a general type, name and offset
class SimpleSymbol : public TBase {
public:
    std::string name;
    int offset;
    SimpleSymbol(std::string &name, int offset, Type type);
    virtual ~SimpleSymbol() = default;
};

typedef std::shared_ptr<SimpleSymbol> SymbolPtr;
typedef std::vector<SimpleSymbol> SSList;

// holds all of the symbol table arguments 
class SimpleSymbolList : public TBase { 
public:
    SSList symbols_list;
    SimpleSymbolList();
    explicit SimpleSymbolList(SSList &symbols_list);
};

typedef std::shared_ptr<SimpleSymbolList> SymListPtr;


// function symbol should have list of argumets and a return type in addition to the function name, offset and the general type
class STypeFunctionSymbol : public SimpleSymbol { 
public:
    SSList parameters;
    Type ret_type;
    STypeFunctionSymbol(std::string &symbol_name, Type symbol_type, SSList &symbols_list);
};

typedef std::shared_ptr<STypeFunctionSymbol> STypeFunctionSymbolPtr;

extern std::string TypeToString(Type type);
extern void SSListToStrings(SSList &symbols_list, std::vector<std::string> &string_vector);

class Scope;
typedef std::shared_ptr<Scope> ScopePtr;

#endif // TYPES_H