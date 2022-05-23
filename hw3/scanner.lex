%{

/* Declarations section */
#include <stdio.h>
#include "hw3_output.hpp"
#include "parser.tab.hpp"

%}


%option noyywrap
%option yylineno
equalUnequal    (==|!=)
notEquality      (<|>|<=|>=)
whitespace	([\t\n\r ])


%%
auto		return AUTO;
void		return VOID;
int			return INT;
byte		return BYTE;
bool		return BOOL;
break		return BREAK;
b			return B;
or			return OR;
and			return AND;
not			return NOT;
false		return FALSE;
true		return TRUE;
return		return RETURN;
if			return IF;
while		return WHILE;
else		return ELSE;
continue	return CONTINUE;
;			return SC;
,			return COMMA;
\)			return RPAREN;
\(			return LPAREN;
\}			return RBRACE;
\{			return LBRACE;
=			return ASSIGN;
{equalUnequal}		return EQUAL_UNEQUAL;
{notEquality}		return NOT_EQUALITY;
\+  		return ADD;
\-  		return SUB;
\*  		return MUL;
\/  	    return DIV;
\/\/[^\r\n]*[\r|\n|\r\n]?       ; 
[a-zA-Z][a-zA-Z0-9]*	        return ID;
\"([^\n\r\"\\]|\\[rnt"\\])+\"	return STRING;
0|[1-9][0-9]*         			return NUM;
{whitespace}					;
.			{ 
              printf(yytext); 
              printf("   "); 
              output::errorLex(yylineno); 
              exit(0); 
            }

%%