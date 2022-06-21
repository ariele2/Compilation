#include "parser.tab.hpp"
#include "compiler.h"

char textbuff[1024];
char* textbuffptr;
const bool PRINT_DEBUG = false;

int main(){

    yyparse();

  
    Compiler::instance().pProgram(yylineno);
}