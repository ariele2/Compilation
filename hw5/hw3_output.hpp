#ifndef _236360_3_
#define _236360_3_

#include <vector>
#include <string>
using namespace std;

namespace output{
    void endScope();
    void printID(const string& id, int offset, const string& type);

    void errorUndefFunc(int ln, const string& id);
    void errorLex(int ln);
    void errorSyn(int ln);
    string makeFunctionType(const string& retType, vector<string>& argTypes);
    void errorMismatch(int ln);
    void errorPrototypeMismatch(int ln, const string& id, vector<string>& argTypes);
    void errorUnexpectedBreak(int ln);
    
    void errorUndef(int ln, const string& id);
    void errorDef(int ln, const string& id);
    
    void errorUnexpectedContinue(int ln);
    void errorMainMissing();
    void errorByteTooLarge(int ln, const string& value);
    
    
}

#endif
