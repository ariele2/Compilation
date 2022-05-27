#include "parser.tab.hpp"
#include "parse_utils.h"



int main(){
    yyparse();
    Utils::instance().pProgram(yylineno);
}