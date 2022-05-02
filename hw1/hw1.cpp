#include "iostream"
#include <cstring>
#include "tokens.hpp"


using std::cout;
using std::endl;


std::string newString;

int main()
{
	int token;
	while ((token = yylex())) 
	{
		std::cout << yylineno << " ";
		switch (token) {
			case COMMENT:
				printf("COMMENT");
				break;
			case ID:
				printf( "ID");
				break;
			case OR:
				printf( "OR");
				break;
			case TRUE:
				printf( "TRUE");
				break;
			case FALSE:
				printf( "FALSE");
				break;
			case BINOP:
				printf( "BINOP");
				break;
			case BOOL:
				printf( "BOOL");
				break;
			case BYTE:
				printf( "BYTE");
				break;
			case AND:
				printf( "AND");
				break;
			case VOID:
				printf( "VOID");
				break;
			case RETURN:
				printf( "RETURN");
				break;
			case IF:
				printf( "IF");
				break;
			case SC:
				printf( "SC");
				break;
			case LPAREN:
				printf( "LPAREN");
				break;
			case RPAREN:
				printf( "RPAREN");
				break;
			case ASSIGN:
				printf( "ASSIGN");
				break;
			case LBRACE:
				printf( "LBRACE");
				break;
			case B:
				printf( "B");
				break;
			case WHILE:
				printf( "WHILE");
				break;
			case BREAK:
				printf( "BREAK");
				break;
			case CONTINUE:
				printf( "CONTINUE");
				break;
			case ELSE:
				printf( "ELSE");
				break;
			case COMMA:
				printf( "COMMA");
				break;
			case INT:
				printf( "INT");
				break;
			case RBRACE:
				printf( "RBRACE");
				break;
			case RELOP:
				printf( "RELOP");
				break;
			case NUM:
				printf( "NUM");
				break;
			case STRING:
				printf( "STRING");
				break;
			case NOT:
				printf( "NOT");
				break;
			case AUTO:
				printf( "AUTO");
			default:
				break;
		}
		printf (" "); 
		
		if (token == COMMENT)
		{
			printf("//") ;
		}
		else if(token == STRING)
		{
			cout << newString; 
			newString.clear();
		}
		else
		{
			printf (yytext); 
		}
		cout << endl;

	}
	return 0;
}
