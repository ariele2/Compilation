#ifndef VALIDATIONS_H
#define VALIDATIONS_H
#include "sym_table.h"

class Validations
{
public:
    SymbolTable &table_ref;
    explicit Validations(SymbolTable &table);
    bool CheckAssigned(Ty f, Ty s);
    bool CheckContinue();
    bool CheckGeneralType(Ty t, GeneralType gt);
    bool CheckOFByte(int &num);
    bool CheckFunction(Ty t);
    bool CheckRelop(Ty f, Ty s);
    bool CheckCast(Ty f, Ty s);
    Ty CheckAndGetBinOpType(Ty f, Ty s);
    bool CheckBool(Ty t);
    bool CheckSymDefined(string &n);
    bool CheckBreak();
    bool CheckTypeType(Ty t1, Ty t);
    bool CheckMainIsDefined();
    bool CheckVoid(Ty t);
    bool CheckCall(FuncSymbolTypePtr &function, ExpListTypePtr &expression_list);
    bool CheckReturn(Ty t);
};

#endif // VALIDATIONS_H
