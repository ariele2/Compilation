#include "parser.tab.hpp"
#include "parse_utils.h"



int main(){
    yyparse();

    // moved parse program here because syntax error happens after the reduce
    Utils::instance().ParseProgram(yylineno);
}