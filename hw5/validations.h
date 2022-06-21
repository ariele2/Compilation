#ifndef VALIDATIONS_H
#define VALIDATIONS_H
#include "sym_table.h"

class Validations {
public:
    SymbolTable &table_ref;
    explicit Validations(SymbolTable &table);

    
    Ty CheckAndGetBinOpType(Ty f, Ty s);
    bool CheckCast(Ty f, Ty s);
    bool CheckCall(FuncSymbolTypePtr &function, ExpListTypePtr &expression_list);
    bool CheckReturn(Ty t);
    bool CheckBool(Ty t);
    bool CheckSymDefined(string &n);
    bool CheckGeneralType(Ty t, GeneralType gt);
    bool CheckTypeType(Ty t1, Ty t);
    bool CheckMainIsDefined();
    bool CheckOFByte(int &num);
    bool CheckFunction(Ty t);
    bool CheckBreak();
    bool CheckVoid(Ty t);
    bool CheckAssigned(Ty f, Ty s);
    bool CheckContinue();
    bool CheckRelop(Ty f, Ty s);
    
    

};


#endif // VALIDATIONS_H
