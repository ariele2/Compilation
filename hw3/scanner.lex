%{
#include "scanner.h"
#include "hw3_output.hpp"
%}

%option yylineno
%option noyywrap
%option nounput

whitespace	    ([\t\n\r ])
equalUnequal    (==|!=)
notEquality     (<|>|<=|>=)



%%

void						  return LexToken(VOID, yytext);
int						    return LexToken(INT, yytext);
byte 						  return LexToken(BYTE, yytext);
b							    return LexToken(B, yytext);
bool						  return LexToken(BOOL, yytext);
and 					    return LexToken(AND, yytext);
or 						    return LexToken(OR, yytext);
not 						  return LexToken(NOT, yytext);
auto                          return LexToken(AUTO, yytext);
true 						  return LexToken(TRUE, yytext);
false 					  return LexToken(FALSE, yytext);
return 					  return LexToken(RETURN, yytext);
if 						    return LexToken(IF, yytext);
else 						  return LexToken(ELSE, yytext);
while 					  return LexToken(WHILE, yytext);
break 					  return LexToken(BREAK, yytext);
continue				  return LexToken(CONTINUE, yytext);
\:	        			return LexToken(COLON, yytext);
\; 						  return LexToken(SC, yytext);
\, 						  return LexToken(COMMA, yytext);
\( 						  return LexToken(LPAREN, yytext);
\) 						  return LexToken(RPAREN, yytext);
\{ 						  return LexToken(LBRACE, yytext);
\} 						  return LexToken(RBRACE, yytext);
{equalUnequal}		      return LexToken(EQUAL_UNEQUAL, yytext);
{notEquality}			return LexToken(NOT_EQUALITY, yytext);
\= 						  return LexToken(ASSIGN, yytext);
\*				        return LexToken(MUL, yytext);
\/					      return LexToken(DIV, yytext);
\+					      return LexToken(ADD, yytext);
\-					      return LexToken(SUB, yytext);
\/\/[^\r\n]*[\r|\n|\r\n]?                     ; 
[a-zA-Z][a-zA-Z0-9]*			       return LexToken(ID, yytext);
0|[1-9][0-9]*         			     return LexToken(NUM, yytext);
\"([^\n\r\"\\]|\\[rnt"\\])+\"	   return LexToken(STRING, yytext);
{whitespace}				                                     ;
.							                  {
                                output::errorInLexical(yylineno); 
                                exit(0); 
                                }
%%
