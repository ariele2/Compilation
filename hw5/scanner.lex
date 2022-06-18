
%{
#include "scanner.h"
%}

%option yylineno
%option noyywrap
%option nounput

whitespace		[ \t\r\n]
strsign			\"
equalUnequal    (==|!=)
inequality      (<|>|<=|>=)

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
{equalUnequal}				return LexToken(EQUAL_UNEQUAL, yytext);
{inequality}				return LexToken(INEQUALITY, yytext);

(\*|[/])					return LexToken(MULT_DIV, yytext);
 (\+|-)					return LexToken(PLUS_MINUS, yytext);

\= 						  return LexToken(ASSIGN, yytext);

\/\/[^\r\n]*[\r|\n|\r\n]?                     ; 
[a-zA-Z][a-zA-Z0-9]*			       return LexToken(ID, yytext);
0|[1-9][0-9]*         			     return LexToken(NUM, yytext);
\"([^\n\r\"\\]|\\[rnt"\\])+\"	   return LexToken(STRING, yytext);
{whitespace}				                                     ;
.							                  {
                                output::errorLex(yylineno); 
                                exit(0); 
                                }
%%