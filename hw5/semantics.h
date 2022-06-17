#ifndef HW3_SEMANTICS_H
#define HW3_SEMANTICS_H
#include "symbol_table.h"

class SemanticChecks {
public:
    SymbolTable &table_ref;
    explicit SemanticChecks(SymbolTable &table);

    
    Type CheckAndGetBinOpType(Type f, Type s);
    bool CheckCast(Type f, Type s);
    bool CheckCall(FuncSymbolTypePtr &function, ExpListTypePtr &expression_list);
    bool CheckReturn(Type t);
    bool CheckBool(Type t);
    bool CheckSymDefined(string &n);
    bool CheckGeneralType(Type t, GeneralType gt);
    bool CheckTypeType(Type t1, Type t);
    bool CheckMainIsDefined();
    bool CheckOFByte(int &num);
    bool CheckFunction(Type t);
    bool CheckBreak();
    bool CheckVoid(Type t);
    bool CheckAssigned(Type f, Type s);
    bool CheckContinue();
    bool CheckRelop(Type f, Type s);
    
    

};


#endif //HW3_SEMANTICS_H
