%{
    #include "tokens.hpp"
    #include "string.h"
    #include <stdio.h>
    
    #define OFFSET 1
    #define BASE 16

    void printErrorAndExit (int error_number);
    void generateString();
    void convertToHex();
    void appendCorccetSlash();
%}

%option noyywrap
%option yylineno

d_and_ascii                  ([0-9]|[A-Fa-f])
l                            ([a-zA-Z])
allowed_ascii                 [0-7]([0-9]|[a-fA-F])
d                            ([0-9])
not_d_or_ascii               ([^\r\n\"A-F0-9])

%x handle_hex
%x quote
%x handle_slash 
%x comment


%%
void                                return VOID; 
int                                 return INT;
while                               return WHILE;
break                               return BREAK;
continue                            return CONTINUE;
;                                   return SC;
,                                   return COMMA;
\(                                  return LPAREN;
true                                return TRUE;
\}                                  return RBRACE;
=                                   return ASSIGN;
false                               return FALSE;
or                                  return OR;
not                                 return NOT;
\)                                  return RPAREN;
\{                                  return LBRACE;
return                              return RETURN;
if                                  return IF;
else                                return ELSE;
byte                                return BYTE;
b                                   return B;
bool                                return BOOL;
auto                                return AUTO;
and                                 return AND;
(==|!=|<|>|<=|>=)                   return RELOP;
([\+\-\*\/])                        return BINOP;
(\/\/[^\n]*)                        BEGIN(comment); return COMMENT;
{l}({l}|{d})*         return ID;
([1-9]{d}*)|{d}             return NUM;
([\n\t\r ])                      ;


<comment>\n|\r|\r\n                                            BEGIN(INITIAL);
(\"$)                                                       printErrorAndExit(ERORR_UNCLOSED_STRING);
\"                                                          BEGIN(quote);
<quote>([ -!#-\[\]-~]*\")                BEGIN(INITIAL); generateString() ; return(STRING);
<quote>([ -!#-\[\]-~])*\\                generateString();BEGIN(handle_slash);
<quote>([ -!#-\[\]-~])*^[ -~]     printErrorAndExit(ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_CHAR);
<quote>(\r|\n)                                              printErrorAndExit(ERORR_UNCLOSED_STRING);
<quote><<EOF>>                                              printErrorAndExit(ERORR_UNCLOSED_STRING);
<quote>.                                                    printErrorAndExit(ERORR_UNCLOSED_STRING);


<handle_slash>(n|t|0|r|\\|\")                                BEGIN(quote); appendCorccetSlash();
<handle_slash>x                                             BEGIN(handle_hex);
<handle_slash><<EOF>>                                       printErrorAndExit(ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_EOF);
<handle_slash>.                                             printErrorAndExit(ERORR_UNDEFINED_ESCAPE_SEQ_STRING);

<handle_hex>{allowed_ascii}                                   BEGIN(quote);convertToHex();
<handle_hex><<EOF>>                                         printErrorAndExit(ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_NOT_ROGHT_LEN_HEX);
<handle_hex>{d_and_ascii}{d_and_ascii}?|{not_d_or_ascii}{not_d_or_ascii}?|{d_and_ascii}({not_d_or_ascii})?|({not_d_or_ascii})({d_and_ascii}?)             printErrorAndExit(ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_NOT_HEX_DECODE);
<handle_hex>.                                               printErrorAndExit(ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_NOT_ROGHT_LEN_HEX);                     

.                                                           printf("Error %s\n", yytext); exit(0); 

%%
void printErrorAndExit (int error_number){
    switch(error_number){
        case ERORR_UNCLOSED_STRING:
            printf("Error unclosed string\n");
            break;
        case ERORR_UNDEFINED_ESCAPE_SEQ_STRING:
            printf("Error undefined escape sequence %s\n", yytext);
            break;
        case ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_CHAR:
            printf("Error undefined escape sequence %c\n", yytext[yyleng - OFFSET]);
            break;
        case ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_NOT_HEX_DECODE:
            printf("Error undefined escape sequence x%s\n", yytext);
            break;
        case ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_NOT_ROGHT_LEN_HEX:
            printf("Error undefined escape sequence x\n");
            break;
        case ERORR_UNCLOSED_UNDEFINED_ESCAPE_SEQ_EOF:
            printf("Error undefined escape sequence\n");
            break;
    }
    exit(0); 
}

void generateString()
{
    newString.append(yytext, 0 , yyleng -OFFSET);
}

void appendCorccetSlash()
{
    char slash_to_add ;
    if(yytext[0] == 'n')
        slash_to_add='\n';
    else if (yytext[0] == '0' )
        slash_to_add = '\0';
    else if (yytext[0] == 't')
        slash_to_add = '\t';
    else if (yytext[0] == 'r')
        slash_to_add = '\r';
    else
        slash_to_add = yytext[0];
    newString = newString + slash_to_add;
}

void convertToHex()
{
    char string_of_hex[2], hex1, hex2;
    string_of_hex[1] = yytext[1];
    string_of_hex[0] = yytext[0];
    hex1 = (int)strtol(string_of_hex, NULL, BASE);
    hex2 = (int)strtol(string_of_hex, NULL, BASE);
    newString = newString + hex1;
}

