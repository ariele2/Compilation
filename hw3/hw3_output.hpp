#define _236360_3_
#include <string>
#include <vector>

using namespace std;
// rm - changed all names, and order, indlude order, names of parms
namespace output
{
    void errorUndefinedFunction(int ln, const string &id);
    void errorDoesNotMatch(int ln);
    string makeFunctionType(const string &returnTypes, vector<string> &argumentsTypes);
    void errorInLexical(int ln);
    void endOfScope();
    void errorMissingMain();
    void errorPrototypeMismatch(int ln, const string &id, vector<string> &argumentsTypes);
    void idPrint(const string &id, int offset, const string &type);
    void errorBreak(int ln);
    void errorInSyntax(int ln);
    void errorIsUndefined(int ln, const string &id);
    void errorInDefintion(int ln, const string &id);
    void errorByteIsTooBig(int ln, const string &value);
    void errorContinue(int ln);

}