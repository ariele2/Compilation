#ifndef HW3_VALIDATIONS
#define HW3_VALIDATIONS
#include "sym_table.h"

class Validator
{
public:
    
    bool checkReturn(Type t);
    bool checkRelop(Type f, Type s);
    bool checkAssigned(Type f, Type s);
    bool checkBool(Type t);
    bool checkBreak();
    SymTable &table_ref;
    bool checkContinue();
    bool checkMainIsDefined();
    bool checkFunction(Type t);
    bool checkOFByte(int &num);
    explicit Validator(SymTable &table);
    Type checkBinop(Type f, Type s);
    bool checkCall(FuncSymTypePtr &function, TExpListPtr &expression_list);
    bool checkSymbolDefined(string &name);
    bool checkCast(Type f, Type s);
    bool checkVoid(Type t);
    bool checkGeneralType(Type t, GeneralType gt);
    bool checkTypeType(Type t, Type t1);
};

#endif // HW3_VALIDATIONS