#ifndef HW3_SEMANTICS_H
#define HW3_SEMANTICS_H
#include "sym_table.h"

class checkSemantics
{
public:
    bool checkContinue();
    bool checkMainIsDefined();
    bool checkFunction(Type t);
    bool checkOFByte(int &num);
    bool checkReturn(Type t);
    bool checkRelop(Type f, Type s);
    bool checkAssigned(Type f, Type s);
    bool checkBool(Type t);
    bool checkBreak();
    SymTable &table_ref;
    explicit checkSemantics(SymTable &table);
    Type checkBinop(Type f, Type s);
    bool checkCall(STypeFunctionSymbolPtr &function, STypeExpListPtr &expression_list);
    bool checkSymbolDefined(string &name);
    bool checkCast(Type f, Type s);
    bool checkVoid(Type t);
};

#endif // HW3_SEMANTICS_H