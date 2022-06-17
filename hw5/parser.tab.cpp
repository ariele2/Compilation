/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.ypp"

#include "compiler.h"

#line 75 "parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_VOID = 3,                       /* VOID  */
  YYSYMBOL_INT = 4,                        /* INT  */
  YYSYMBOL_BYTE = 5,                       /* BYTE  */
  YYSYMBOL_B = 6,                          /* B  */
  YYSYMBOL_BOOL = 7,                       /* BOOL  */
  YYSYMBOL_TRUE = 8,                       /* TRUE  */
  YYSYMBOL_FALSE = 9,                      /* FALSE  */
  YYSYMBOL_RETURN = 10,                    /* RETURN  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_WHILE = 12,                     /* WHILE  */
  YYSYMBOL_BREAK = 13,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 14,                  /* CONTINUE  */
  YYSYMBOL_SC = 15,                        /* SC  */
  YYSYMBOL_COMMA = 16,                     /* COMMA  */
  YYSYMBOL_ID = 17,                        /* ID  */
  YYSYMBOL_NUM = 18,                       /* NUM  */
  YYSYMBOL_STRING = 19,                    /* STRING  */
  YYSYMBOL_DOTS = 20,                      /* DOTS  */
  YYSYMBOL_SWITCH = 21,                    /* SWITCH  */
  YYSYMBOL_CASE = 22,                      /* CASE  */
  YYSYMBOL_DEFAULT = 23,                   /* DEFAULT  */
  YYSYMBOL_COLON = 24,                     /* COLON  */
  YYSYMBOL_ASSIGN = 25,                    /* ASSIGN  */
  YYSYMBOL_OR = 26,                        /* OR  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_EQUAL_UNEQUAL = 28,             /* EQUAL_UNEQUAL  */
  YYSYMBOL_INEQUALITY = 29,                /* INEQUALITY  */
  YYSYMBOL_PLUS_MINUS = 30,                /* PLUS_MINUS  */
  YYSYMBOL_MULT_DIV = 31,                  /* MULT_DIV  */
  YYSYMBOL_NOT = 32,                       /* NOT  */
  YYSYMBOL_LBRACKET = 33,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 34,                  /* RBRACKET  */
  YYSYMBOL_LBRACE = 35,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 36,                    /* RBRACE  */
  YYSYMBOL_LPAREN = 37,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 38,                    /* RPAREN  */
  YYSYMBOL_ELSE = 39,                      /* ELSE  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_Program = 41,                   /* Program  */
  YYSYMBOL_Funcs = 42,                     /* Funcs  */
  YYSYMBOL_FuncDecl = 43,                  /* FuncDecl  */
  YYSYMBOL_FuncHead = 44,                  /* FuncHead  */
  YYSYMBOL_RetType = 45,                   /* RetType  */
  YYSYMBOL_Formals = 46,                   /* Formals  */
  YYSYMBOL_FormalsList = 47,               /* FormalsList  */
  YYSYMBOL_FormalDecl = 48,                /* FormalDecl  */
  YYSYMBOL_Statements = 49,                /* Statements  */
  YYSYMBOL_Statement = 50,                 /* Statement  */
  YYSYMBOL_Call = 51,                      /* Call  */
  YYSYMBOL_ExpList = 52,                   /* ExpList  */
  YYSYMBOL_Type = 53,                      /* Type  */
  YYSYMBOL_Exp = 54,                       /* Exp  */
  YYSYMBOL_CaseList = 55,                  /* CaseList  */
  YYSYMBOL_CaseDecl = 56,                  /* CaseDecl  */
  YYSYMBOL_PushStatementScope = 57,        /* PushStatementScope  */
  YYSYMBOL_PushWhileScope = 58,            /* PushWhileScope  */
  YYSYMBOL_PushSwitchScope = 59,           /* PushSwitchScope  */
  YYSYMBOL_PopScope = 60,                  /* PopScope  */
  YYSYMBOL_CheckSwitchExp = 61,            /* CheckSwitchExp  */
  YYSYMBOL_CheckBool = 62,                 /* CheckBool  */
  YYSYMBOL_GenIfLabel = 63,                /* GenIfLabel  */
  YYSYMBOL_GenElseLabel = 64,              /* GenElseLabel  */
  YYSYMBOL_GenWhileHeadLabel = 65,         /* GenWhileHeadLabel  */
  YYSYMBOL_GenWhileBodyLabel = 66,         /* GenWhileBodyLabel  */
  YYSYMBOL_GenAndLabel = 67,               /* GenAndLabel  */
  YYSYMBOL_GenOrLabel = 68,                /* GenOrLabel  */
  YYSYMBOL_GenNextLabel = 69,              /* GenNextLabel  */
  YYSYMBOL_GenDefaultLabel = 70,           /* GenDefaultLabel  */
  YYSYMBOL_GenCaseDeclLabel = 71,          /* GenCaseDeclLabel  */
  YYSYMBOL_BranchNext = 72,                /* BranchNext  */
  YYSYMBOL_BranchWhileHead = 73,           /* BranchWhileHead  */
  YYSYMBOL_BranchIfNext = 74,              /* BranchIfNext  */
  YYSYMBOL_BranchSwitchHead = 75,          /* BranchSwitchHead  */
  YYSYMBOL_BranchCaseHead = 76,            /* BranchCaseHead  */
  YYSYMBOL_BranchDefaultHead = 77,         /* BranchDefaultHead  */
  YYSYMBOL_ConvertBool = 78                /* ConvertBool  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   178

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    59,    59,    61,    63,    65,    67,    69,    71,    73,
      75,    77,    79,    81,    83,    85,    87,    89,    91,    93,
      95,    97,    99,   101,   103,   105,   107,   109,   111,   113,
     115,   117,   119,   121,   123,   125,   127,   129,   131,   133,
     135,   137,   139,   141,   143,   145,   147,   149,   151,   153,
     155,   157,   159,   161,   163,   168,   170,   172,   174,   178,
     180,   184,   186,   188,   190,   192,   194,   196,   198,   200,
     204,   206,   208,   210,   212,   214,   218
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "VOID", "INT", "BYTE",
  "B", "BOOL", "TRUE", "FALSE", "RETURN", "IF", "WHILE", "BREAK",
  "CONTINUE", "SC", "COMMA", "ID", "NUM", "STRING", "DOTS", "SWITCH",
  "CASE", "DEFAULT", "COLON", "ASSIGN", "OR", "AND", "EQUAL_UNEQUAL",
  "INEQUALITY", "PLUS_MINUS", "MULT_DIV", "NOT", "LBRACKET", "RBRACKET",
  "LBRACE", "RBRACE", "LPAREN", "RPAREN", "ELSE", "$accept", "Program",
  "Funcs", "FuncDecl", "FuncHead", "RetType", "Formals", "FormalsList",
  "FormalDecl", "Statements", "Statement", "Call", "ExpList", "Type",
  "Exp", "CaseList", "CaseDecl", "PushStatementScope", "PushWhileScope",
  "PushSwitchScope", "PopScope", "CheckSwitchExp", "CheckBool",
  "GenIfLabel", "GenElseLabel", "GenWhileHeadLabel", "GenWhileBodyLabel",
  "GenAndLabel", "GenOrLabel", "GenNextLabel", "GenDefaultLabel",
  "GenCaseDeclLabel", "BranchNext", "BranchWhileHead", "BranchIfNext",
  "BranchSwitchHead", "BranchCaseHead", "BranchDefaultHead", "ConvertBool", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
#endif

#define YYPACT_NINF (-106)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-68)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     146,  -106,  -106,  -106,  -106,    14,  -106,   146,   -16,     8,
    -106,  -106,  -106,   104,     1,    50,    11,    12,    24,    51,
      -5,    35,    38,  -106,    61,    62,    46,   156,  -106,  -106,
    -106,    48,    83,  -106,    75,    75,  -106,   105,    75,  -106,
    -106,  -106,    75,    -2,    75,    54,  -106,  -106,  -106,     3,
     104,    53,  -106,    79,    88,  -106,  -106,    72,  -106,  -106,
    -106,    75,    75,    75,    75,    15,  -106,   111,  -106,    58,
      15,    15,  -106,   104,  -106,    75,    70,  -106,   156,  -106,
    -106,    75,    75,    26,    32,    82,  -106,    81,    75,  -106,
    -106,   136,   124,  -106,   128,  -106,  -106,   117,    99,  -106,
      15,    75,  -106,  -106,  -106,  -106,  -106,   126,  -106,  -106,
     104,  -106,   130,  -106,  -106,    -1,  -106,   104,   148,   143,
     132,   133,   131,  -106,   147,  -106,  -106,    -1,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,   104,   104,   104,   137,  -106,
     -10,  -106
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     8,    33,    34,    35,     0,     2,     3,     0,     0,
       7,     1,     4,    55,     0,     0,     0,     0,     0,     0,
       0,     0,    70,    70,     0,     0,     0,     9,    44,    45,
      21,    39,    41,    43,     0,     0,    40,     0,     0,    71,
      26,    27,     0,     0,     0,     0,    67,    14,    20,     0,
      55,     0,    10,    11,     0,    42,    46,     0,    22,    66,
      65,     0,     0,     0,     0,    60,    63,     0,    30,     0,
      76,    59,     5,    55,    17,     0,    70,     6,     0,    13,
      36,     0,     0,    49,    50,    37,    38,     0,     0,    19,
      29,    31,     0,    70,     0,    58,    12,    48,    47,    55,
      60,     0,    73,    15,    18,    16,    61,     0,    32,    57,
      55,    56,     0,    58,    64,     0,    72,    55,     0,     0,
       0,    67,    23,    71,     0,    75,    28,     0,    55,    25,
      74,    68,    51,    62,    69,    55,    55,    55,    70,    58,
      70,    24
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -106,  -106,   165,  -106,  -106,  -106,  -106,    96,  -106,   -49,
     -63,   -13,    74,     2,   -11,    49,  -106,   -88,  -106,  -106,
    -105,  -106,    77,  -106,  -106,  -106,  -106,  -106,  -106,   -43,
    -106,  -106,   -18,    55,  -106,  -106,  -106,  -106,  -106
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,     9,    51,    52,    53,    22,
      23,    36,    69,    25,    70,   120,   121,    26,   114,   112,
     105,    92,    87,   110,   136,    88,   117,    82,    81,    45,
     135,   137,    46,    66,   122,   109,   134,   131,    91
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      24,    76,    10,    73,    37,    47,    28,    29,   116,    10,
      93,   106,   -54,   -54,    11,    31,    32,    33,    74,    13,
      42,   118,   119,    56,    57,    14,   -54,    65,    75,    54,
      34,    67,    43,    71,   141,    35,    68,    24,    27,    40,
     133,    59,    60,    61,    62,    63,    64,   113,    38,    39,
      83,    84,    85,    86,   123,    62,    63,    64,    28,    29,
      24,   -68,    63,    64,    94,    30,    41,    31,    32,    33,
      97,    98,    44,   139,   -67,   103,    48,   100,   127,    49,
      54,    50,    34,    28,    29,    43,   138,    35,   140,    55,
      72,    77,    31,    32,    33,    78,    90,    24,    59,    60,
      61,    62,    63,    64,    24,    79,    95,    34,     2,     3,
      80,     4,    35,    64,    15,    16,    17,    18,    19,    99,
      58,    20,    24,    24,    24,    21,    89,    61,    62,    63,
      64,    59,    60,    61,    62,    63,    64,    59,    60,    61,
      62,    63,    64,   104,    60,    61,    62,    63,    64,     1,
       2,     3,   101,     4,    59,    60,    61,    62,    63,    64,
       2,     3,   102,     4,   111,   115,   124,   125,   126,   -52,
     128,   130,    12,   -53,    96,   108,   132,   107,   129
};

static const yytype_uint8 yycheck[] =
{
      13,    50,     0,    46,    15,    23,     8,     9,   113,     7,
      73,    99,    22,    23,     0,    17,    18,    19,    15,    35,
      25,    22,    23,    34,    35,    17,    36,    38,    25,    27,
      32,    42,    37,    44,   139,    37,    38,    50,    37,    15,
     128,    26,    27,    28,    29,    30,    31,   110,    37,    37,
      61,    62,    63,    64,   117,    29,    30,    31,     8,     9,
      73,    29,    30,    31,    75,    15,    15,    17,    18,    19,
      81,    82,    37,   136,    36,    93,    15,    88,   121,    17,
      78,    35,    32,     8,     9,    37,   135,    37,   137,     6,
      36,    38,    17,    18,    19,    16,    38,   110,    26,    27,
      28,    29,    30,    31,   117,    17,    36,    32,     4,     5,
      38,     7,    37,    31,    10,    11,    12,    13,    14,    38,
      15,    17,   135,   136,   137,    21,    15,    28,    29,    30,
      31,    26,    27,    28,    29,    30,    31,    26,    27,    28,
      29,    30,    31,    15,    27,    28,    29,    30,    31,     3,
       4,     5,    16,     7,    26,    27,    28,    29,    30,    31,
       4,     5,    38,     7,    38,    35,    18,    24,    36,    36,
      39,    24,     7,    36,    78,   101,   127,   100,   123
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,    41,    42,    43,    44,    45,
      53,     0,    42,    35,    17,    10,    11,    12,    13,    14,
      17,    21,    49,    50,    51,    53,    57,    37,     8,     9,
      15,    17,    18,    19,    32,    37,    51,    54,    37,    37,
      15,    15,    25,    37,    37,    69,    72,    72,    15,    17,
      35,    46,    47,    48,    53,     6,    54,    54,    15,    26,
      27,    28,    29,    30,    31,    54,    73,    54,    38,    52,
      54,    54,    36,    69,    15,    25,    49,    38,    16,    17,
      38,    68,    67,    54,    54,    54,    54,    62,    65,    15,
      38,    78,    61,    50,    54,    36,    47,    54,    54,    38,
      54,    16,    38,    72,    15,    60,    57,    62,    52,    75,
      63,    38,    59,    50,    58,    35,    60,    66,    22,    23,
      55,    56,    74,    50,    18,    24,    36,    69,    39,    73,
      24,    77,    55,    57,    76,    70,    64,    71,    49,    50,
      49,    60
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    44,    45,    45,    46,
      46,    47,    47,    48,    49,    49,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    51,
      51,    52,    52,    53,    53,    53,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    55,    55,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     5,     1,     1,     0,
       1,     1,     3,     2,     2,     5,     5,     3,     5,     4,
       2,     2,     3,    10,    15,    11,     2,     2,    10,     4,
       3,     2,     4,     1,     1,     1,     3,     3,     3,     1,
       1,     1,     2,     1,     1,     1,     2,     4,     4,     3,
       3,     3,     1,     5,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: Funcs  */
#line 60 "parser.ypp"
    {}
#line 1255 "parser.tab.cpp"
    break;

  case 3: /* Funcs: %empty  */
#line 62 "parser.ypp"
    {Compiler::instance().ParseFuncs(yylineno);}
#line 1261 "parser.tab.cpp"
    break;

  case 4: /* Funcs: FuncDecl Funcs  */
#line 64 "parser.ypp"
    {Compiler::instance().ParseFuncs(yylineno);}
#line 1267 "parser.tab.cpp"
    break;

  case 5: /* FuncDecl: FuncHead LBRACE Statements GenNextLabel RBRACE  */
#line 66 "parser.ypp"
    {Compiler::instance().ParseFuncDecl(yylineno, yyvsp[-2], yyvsp[-1]);}
#line 1273 "parser.tab.cpp"
    break;

  case 6: /* FuncHead: RetType ID LPAREN Formals RPAREN  */
#line 68 "parser.ypp"
    {Compiler::instance().ParseFuncHead(yylineno, yyvsp[-4], yyvsp[-3], yyvsp[-1]);}
#line 1279 "parser.tab.cpp"
    break;

  case 7: /* RetType: Type  */
#line 70 "parser.ypp"
    {yyval = Compiler::instance().ParseRetType(yylineno, yyvsp[0]);}
#line 1285 "parser.tab.cpp"
    break;

  case 8: /* RetType: VOID  */
#line 72 "parser.ypp"
    {yyval = Compiler::instance().ParseRetType(yylineno);}
#line 1291 "parser.tab.cpp"
    break;

  case 9: /* Formals: %empty  */
#line 74 "parser.ypp"
    {yyval = Compiler::instance().ParseFormals(yylineno);}
#line 1297 "parser.tab.cpp"
    break;

  case 10: /* Formals: FormalsList  */
#line 76 "parser.ypp"
    {yyval = Compiler::instance().ParseFormals(yylineno, yyvsp[0]);}
#line 1303 "parser.tab.cpp"
    break;

  case 11: /* FormalsList: FormalDecl  */
#line 78 "parser.ypp"
    {yyval = Compiler::instance().ParseFormalsList(yylineno, yyvsp[0]);}
#line 1309 "parser.tab.cpp"
    break;

  case 12: /* FormalsList: FormalDecl COMMA FormalsList  */
#line 80 "parser.ypp"
    {yyval = Compiler::instance().ParseFormalsList(yylineno, yyvsp[-2], yyvsp[0]);}
#line 1315 "parser.tab.cpp"
    break;

  case 13: /* FormalDecl: Type ID  */
#line 82 "parser.ypp"
    {yyval = Compiler::instance().ParseFormalDecl(yylineno, yyvsp[-1], yyvsp[0]);}
#line 1321 "parser.tab.cpp"
    break;

  case 14: /* Statements: Statement BranchNext  */
#line 84 "parser.ypp"
    {yyval = Compiler::instance().ParseStatements(yylineno, yyvsp[-1], yyvsp[0]);}
#line 1327 "parser.tab.cpp"
    break;

  case 15: /* Statements: Statements BranchNext GenNextLabel Statement BranchNext  */
#line 86 "parser.ypp"
    {yyval = Compiler::instance().ParseStatements(yylineno, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1333 "parser.tab.cpp"
    break;

  case 16: /* Statement: PushStatementScope LBRACE Statements RBRACE PopScope  */
#line 88 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementOfStatements(yylineno, yyvsp[-2]);}
#line 1339 "parser.tab.cpp"
    break;

  case 17: /* Statement: Type ID SC  */
#line 90 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementType(yylineno, yyvsp[-2], yyvsp[-1]);}
#line 1345 "parser.tab.cpp"
    break;

  case 18: /* Statement: Type ID ASSIGN Exp SC  */
#line 92 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementTypeAssign(yylineno, yyvsp[-4], yyvsp[-3], yyvsp[-1]);}
#line 1351 "parser.tab.cpp"
    break;

  case 19: /* Statement: ID ASSIGN Exp SC  */
#line 94 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementAssign(yylineno, yyvsp[-3], yyvsp[-1]);}
#line 1357 "parser.tab.cpp"
    break;

  case 20: /* Statement: Call SC  */
#line 96 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementCall(yylineno);}
#line 1363 "parser.tab.cpp"
    break;

  case 21: /* Statement: RETURN SC  */
#line 98 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementReturn(yylineno);}
#line 1369 "parser.tab.cpp"
    break;

  case 22: /* Statement: RETURN Exp SC  */
#line 100 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementReturnExp(yylineno, yyvsp[-1]);}
#line 1375 "parser.tab.cpp"
    break;

  case 23: /* Statement: IF LPAREN Exp CheckBool RPAREN PushStatementScope GenIfLabel Statement PopScope BranchIfNext  */
#line 102 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementIf(yylineno, yyvsp[-7], yyvsp[-3], yyvsp[-2], yyvsp[0]);}
#line 1381 "parser.tab.cpp"
    break;

  case 24: /* Statement: IF LPAREN Exp CheckBool RPAREN PushStatementScope GenIfLabel Statement PopScope BranchIfNext ELSE PushStatementScope GenElseLabel Statement PopScope  */
#line 104 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementIfElse(yylineno, yyvsp[-12], yyvsp[-8], yyvsp[-7], yyvsp[-5], yyvsp[-2], yyvsp[-1]);}
#line 1387 "parser.tab.cpp"
    break;

  case 25: /* Statement: WHILE LPAREN BranchWhileHead GenWhileHeadLabel Exp CheckBool RPAREN PushWhileScope GenWhileBodyLabel Statement BranchWhileHead  */
#line 106 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementWhile(yylineno, yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1393 "parser.tab.cpp"
    break;

  case 26: /* Statement: BREAK SC  */
#line 108 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementBreak(yylineno);}
#line 1399 "parser.tab.cpp"
    break;

  case 27: /* Statement: CONTINUE SC  */
#line 110 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementContinue(yylineno);}
#line 1405 "parser.tab.cpp"
    break;

  case 28: /* Statement: SWITCH LPAREN Exp CheckSwitchExp RPAREN BranchSwitchHead PushSwitchScope LBRACE CaseList RBRACE  */
#line 112 "parser.ypp"
    {yyval = Compiler::instance().ParseStatementSwitch(yylineno, yyvsp[-7], yyvsp[-4], yyvsp[-1]);}
#line 1411 "parser.tab.cpp"
    break;

  case 29: /* Call: ID LPAREN ExpList RPAREN  */
#line 114 "parser.ypp"
    {yyval = Compiler::instance().ParseCall(yylineno, yyvsp[-3], yyvsp[-1]);}
#line 1417 "parser.tab.cpp"
    break;

  case 30: /* Call: ID LPAREN RPAREN  */
#line 116 "parser.ypp"
    {yyval = Compiler::instance().ParseCall(yylineno, yyvsp[-2]);}
#line 1423 "parser.tab.cpp"
    break;

  case 31: /* ExpList: Exp ConvertBool  */
#line 118 "parser.ypp"
    {yyval = Compiler::instance().ParseExplist(yylineno, yyvsp[0]);}
#line 1429 "parser.tab.cpp"
    break;

  case 32: /* ExpList: Exp ConvertBool COMMA ExpList  */
#line 120 "parser.ypp"
    {yyval = Compiler::instance().ParseExplist(yylineno, yyvsp[-2], yyvsp[0]);}
#line 1435 "parser.tab.cpp"
    break;

  case 33: /* Type: INT  */
#line 122 "parser.ypp"
    {yyval = Compiler::instance().ParseInt(yylineno);}
#line 1441 "parser.tab.cpp"
    break;

  case 34: /* Type: BYTE  */
#line 124 "parser.ypp"
    {yyval = Compiler::instance().ParseByte(yylineno);}
#line 1447 "parser.tab.cpp"
    break;

  case 35: /* Type: BOOL  */
#line 126 "parser.ypp"
    {yyval = Compiler::instance().ParseBool(yylineno);}
#line 1453 "parser.tab.cpp"
    break;

  case 36: /* Exp: LPAREN Exp RPAREN  */
#line 128 "parser.ypp"
    {yyval = Compiler::instance().ParseParentheses(yylineno, yyvsp[-1]);}
#line 1459 "parser.tab.cpp"
    break;

  case 37: /* Exp: Exp PLUS_MINUS Exp  */
#line 130 "parser.ypp"
    {yyval = Compiler::instance().ParseBinop(yylineno, yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1465 "parser.tab.cpp"
    break;

  case 38: /* Exp: Exp MULT_DIV Exp  */
#line 132 "parser.ypp"
    {yyval = Compiler::instance().ParseBinop(yylineno, yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1471 "parser.tab.cpp"
    break;

  case 39: /* Exp: ID  */
#line 134 "parser.ypp"
    {yyval = Compiler::instance().ParseID(yylineno, yyvsp[0]);}
#line 1477 "parser.tab.cpp"
    break;

  case 40: /* Exp: Call  */
#line 136 "parser.ypp"
    {yyval = Compiler::instance().ParseCallExp(yylineno, yyvsp[0]);}
#line 1483 "parser.tab.cpp"
    break;

  case 41: /* Exp: NUM  */
#line 138 "parser.ypp"
    {yyval = Compiler::instance().ParseNum(yylineno, yyvsp[0]);}
#line 1489 "parser.tab.cpp"
    break;

  case 42: /* Exp: NUM B  */
#line 140 "parser.ypp"
    {yyval = Compiler::instance().ParseNumB(yylineno, yyvsp[-1]);}
#line 1495 "parser.tab.cpp"
    break;

  case 43: /* Exp: STRING  */
#line 142 "parser.ypp"
    {yyval = Compiler::instance().ParseString(yylineno, yyvsp[0]);}
#line 1501 "parser.tab.cpp"
    break;

  case 44: /* Exp: TRUE  */
#line 144 "parser.ypp"
    {yyval = Compiler::instance().ParseTrue(yylineno);}
#line 1507 "parser.tab.cpp"
    break;

  case 45: /* Exp: FALSE  */
#line 146 "parser.ypp"
    {yyval = Compiler::instance().ParseFalse(yylineno);}
#line 1513 "parser.tab.cpp"
    break;

  case 46: /* Exp: NOT Exp  */
#line 148 "parser.ypp"
    {yyval = Compiler::instance().ParseNot(yylineno, yyvsp[0]);}
#line 1519 "parser.tab.cpp"
    break;

  case 47: /* Exp: Exp AND GenAndLabel Exp  */
#line 150 "parser.ypp"
    {yyval = Compiler::instance().ParseAnd(yylineno, yyvsp[-3], yyvsp[-1], yyvsp[0]);}
#line 1525 "parser.tab.cpp"
    break;

  case 48: /* Exp: Exp OR GenOrLabel Exp  */
#line 152 "parser.ypp"
    {yyval = Compiler::instance().ParseOr(yylineno, yyvsp[-3], yyvsp[-1], yyvsp[0]);}
#line 1531 "parser.tab.cpp"
    break;

  case 49: /* Exp: Exp EQUAL_UNEQUAL Exp  */
#line 154 "parser.ypp"
    {yyval = Compiler::instance().ParseRelOp(yylineno, yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1537 "parser.tab.cpp"
    break;

  case 50: /* Exp: Exp INEQUALITY Exp  */
#line 156 "parser.ypp"
    {yyval = Compiler::instance().ParseRelOp(yylineno, yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1543 "parser.tab.cpp"
    break;

  case 51: /* CaseList: CaseDecl GenNextLabel CaseList  */
#line 158 "parser.ypp"
    {yyval = Compiler::instance().ParseCaseList(yylineno, yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1549 "parser.tab.cpp"
    break;

  case 52: /* CaseList: CaseDecl  */
#line 160 "parser.ypp"
    {yyval = Compiler::instance().ParseCaseList(yylineno, yyvsp[0]);}
#line 1555 "parser.tab.cpp"
    break;

  case 53: /* CaseList: DEFAULT COLON BranchDefaultHead GenDefaultLabel Statements  */
#line 162 "parser.ypp"
    {yyval = Compiler::instance().ParseCaseDefault(yylineno, yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1561 "parser.tab.cpp"
    break;

  case 54: /* CaseDecl: CASE NUM COLON BranchCaseHead GenCaseDeclLabel Statements  */
#line 164 "parser.ypp"
    {yyval = Compiler::instance().ParseCaseDecl(yylineno, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]);}
#line 1567 "parser.tab.cpp"
    break;

  case 55: /* PushStatementScope: %empty  */
#line 169 "parser.ypp"
    {Compiler::instance().ParsePushStatementScope(yylineno);}
#line 1573 "parser.tab.cpp"
    break;

  case 56: /* PushWhileScope: %empty  */
#line 171 "parser.ypp"
    {Compiler::instance().ParsePushWhileScope(yylineno, yyvsp[(-3) - (0)]);}
#line 1579 "parser.tab.cpp"
    break;

  case 57: /* PushSwitchScope: %empty  */
#line 173 "parser.ypp"
    {Compiler::instance().ParsePushSwitchScope(yylineno, yyvsp[(-3) - (0)]);}
#line 1585 "parser.tab.cpp"
    break;

  case 58: /* PopScope: %empty  */
#line 175 "parser.ypp"
    {Compiler::instance().ParsePopScope(yylineno);}
#line 1591 "parser.tab.cpp"
    break;

  case 59: /* CheckSwitchExp: %empty  */
#line 179 "parser.ypp"
    {Compiler::instance().ParseCheckSwitchExp(yylineno, yyvsp[0]);}
#line 1597 "parser.tab.cpp"
    break;

  case 60: /* CheckBool: %empty  */
#line 181 "parser.ypp"
    {Compiler::instance().ParseCheckBool(yylineno, yyvsp[0]);}
#line 1603 "parser.tab.cpp"
    break;

  case 61: /* GenIfLabel: %empty  */
#line 185 "parser.ypp"
    {yyval = Compiler::instance().ParseGenIfLabel(yylineno);}
#line 1609 "parser.tab.cpp"
    break;

  case 62: /* GenElseLabel: %empty  */
#line 187 "parser.ypp"
    {yyval = Compiler::instance().ParseGenElseLabel(yylineno);}
#line 1615 "parser.tab.cpp"
    break;

  case 63: /* GenWhileHeadLabel: %empty  */
#line 189 "parser.ypp"
    {yyval = Compiler::instance().ParseGenWhileHeadLabel(yylineno);}
#line 1621 "parser.tab.cpp"
    break;

  case 64: /* GenWhileBodyLabel: %empty  */
#line 191 "parser.ypp"
    {yyval = Compiler::instance().ParseGenWhileBodyLabel(yylineno);}
#line 1627 "parser.tab.cpp"
    break;

  case 65: /* GenAndLabel: %empty  */
#line 193 "parser.ypp"
    {yyval = Compiler::instance().ParseGenAndLabel(yylineno);}
#line 1633 "parser.tab.cpp"
    break;

  case 66: /* GenOrLabel: %empty  */
#line 195 "parser.ypp"
    {yyval = Compiler::instance().ParseGenOrLabel(yylineno);}
#line 1639 "parser.tab.cpp"
    break;

  case 67: /* GenNextLabel: %empty  */
#line 197 "parser.ypp"
    {yyval = Compiler::instance().ParseGenNextLabel(yylineno);}
#line 1645 "parser.tab.cpp"
    break;

  case 68: /* GenDefaultLabel: %empty  */
#line 199 "parser.ypp"
   {yyval = Compiler::instance().ParseGenDefaultLabel(yylineno);}
#line 1651 "parser.tab.cpp"
    break;

  case 69: /* GenCaseDeclLabel: %empty  */
#line 201 "parser.ypp"
   {yyval = Compiler::instance().ParseGenCaseDeclLabel(yylineno);}
#line 1657 "parser.tab.cpp"
    break;

  case 70: /* BranchNext: %empty  */
#line 205 "parser.ypp"
    {yyval = Compiler::instance().ParseBranchNext(yylineno);}
#line 1663 "parser.tab.cpp"
    break;

  case 71: /* BranchWhileHead: %empty  */
#line 207 "parser.ypp"
    {yyval = Compiler::instance().ParseBranchWhileHead(yylineno);}
#line 1669 "parser.tab.cpp"
    break;

  case 72: /* BranchIfNext: %empty  */
#line 209 "parser.ypp"
    {yyval = Compiler::instance().ParseBranchIfNext(yylineno);}
#line 1675 "parser.tab.cpp"
    break;

  case 73: /* BranchSwitchHead: %empty  */
#line 211 "parser.ypp"
    {yyval = Compiler::instance().ParseBranchSwitchHead(yylineno);}
#line 1681 "parser.tab.cpp"
    break;

  case 74: /* BranchCaseHead: %empty  */
#line 213 "parser.ypp"
    {yyval = Compiler::instance().ParseBranchCaseHead(yylineno);}
#line 1687 "parser.tab.cpp"
    break;

  case 75: /* BranchDefaultHead: %empty  */
#line 215 "parser.ypp"
    {yyval = Compiler::instance().ParseBranchDefaultHead(yylineno);}
#line 1693 "parser.tab.cpp"
    break;

  case 76: /* ConvertBool: %empty  */
#line 219 "parser.ypp"
    {yyval = Compiler::instance().ParseConvertBool(yylineno, yyvsp[0]);}
#line 1699 "parser.tab.cpp"
    break;


#line 1703 "parser.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 220 "parser.ypp"


void yyerror(const char * message)
{
	errorSyn(yylineno);
	exit(0);
}
