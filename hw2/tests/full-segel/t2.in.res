32: Type -> BOOL
5: RetType -> Type
33: TypeAnnotation -> epsilon
30: Type -> INT
11: FormalDecl -> Type ID
33: TypeAnnotation -> epsilon
30: Type -> INT
11: FormalDecl -> Type ID
9: FormalsList -> FormalDecl
10: FormalsList -> FormalDecl COMMA FormalsList
8: Formals -> FormalsList
37: Exp -> ID
37: Exp -> ID
47: Exp -> Exp RELOP Exp
20: Statement -> RETURN Exp SC
12: Statements -> Statement
4: FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE
30: Type -> INT
5: RetType -> Type
7: Formals -> epsilon
39: Exp -> NUM
20: Statement -> RETURN Exp SC
12: Statements -> Statement
4: FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE
6: RetType ->  VOID
7: Formals -> epsilon
42: Exp -> TRUE
16: Statement -> AUTO ID ASSIGN Exp SC
12: Statements -> Statement
37: Exp -> ID
44: Exp -> NOT Exp
33: TypeAnnotation -> epsilon
30: Type -> INT
27: Call -> ID LPAREN RPAREN
38: Exp -> Call
16: Statement -> TypeAnnotation Type ID ASSIGN Exp SC
12: Statements -> Statement
37: Exp -> ID
28: ExpList -> Exp
26: Call -> ID LPAREN ExpList RPAREN
18: Statement -> Call SC
13: Statements -> Statements Statement
30: Type -> INT
37: Exp -> ID
48: Exp -> LPAREN TypeAnnotation Type RPAREN Exp
17: Statement -> ID ASSIGN Exp SC
13: Statements -> Statements Statement
14: Statement -> LBRACE Statements RBRACE
line 19: syntax error
