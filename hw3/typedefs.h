#ifndef HW3_TYPEDEFS_H
#define HW3_TYPEDEFS_H
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

typedef std::shared_ptr<STypeCType> STypeCTypePtr;

class STypeString : public TBase {
public:
    std::string token;
    explicit STypeString(std::string& token);
};

typedef std::shared_ptr<STypeString> STypeStringPtr;

class STypeNumber : public TBase {
public:
    int token;
    explicit STypeNumber(std::string& token_string);
};

typedef std::shared_ptr<STypeNumber> STypeNumberPtr;

class STypeBool : public TBase {
public:
    bool token;
    explicit STypeBool(bool token);
};

typedef std::shared_ptr<STypeBool> STypeBoolPtr;

class STypeSymbol : public TBase {
    // must have at least one virtual method
public:
    std::string name;
    int offset;
    STypeSymbol(std::string &name, int offset, Type type);
    virtual ~STypeSymbol() = default;
};

typedef std::shared_ptr<STypeSymbol> STypeSymbolPtr;
typedef std::vector<STypeSymbol> ArgList;

class STypeArgList : public TBase { // the symbol table stack for arguments
public:
    ArgList arg_list;
    STypeArgList();
    explicit STypeArgList(ArgList &arg_list);
};

typedef std::shared_ptr<STypeArgList> STypeArgListPtr;


class STypeFunctionSymbol : public STypeSymbol { // the symbol table stack for functions
public:
    ArgList parameters;
    Type ret_type;
    STypeFunctionSymbol(std::string &symbol_name, Type symbol_type, ArgList &arg_list);
};

typedef std::shared_ptr<STypeFunctionSymbol> STypeFunctionSymbolPtr;

extern std::string TypeToString(Type type);
extern void ArgListToStrings(ArgList &arg_list, std::vector<std::string> &string_vector);

class Scope;
typedef std::shared_ptr<Scope> ScopePtr;

#endif //HWw3_TYPEDEFS_H