#include "parser.tab.hpp"
#include "utils.h"



int main(){
    yyparse();
    Utils::instance().pProgram(yylineno);
}