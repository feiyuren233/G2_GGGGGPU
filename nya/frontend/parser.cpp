/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

/***********************************************************************
 *   Interface to the parser module for CSC467 course project.
 * 
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source language program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>
#include "common.h"
#include "ast.h"
// #include "semantic.h"
#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */

#line 46 "parser.y" /* yacc.c:339  */

#define YYDEBUG 1

#line 98 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF_SYM = 258,
    ELSE_SYM = 259,
    WHILE_SYM = 260,
    TRUE_SYM = 261,
    FALSE_SYM = 262,
    CONST_SYM = 263,
    BOOL_T = 264,
    BVEC2_T = 265,
    BVEC3_T = 266,
    BVEC4_T = 267,
    INT_T = 268,
    IVEC2_T = 269,
    IVEC3_T = 270,
    IVEC4_T = 271,
    FLOAT_T = 272,
    VEC2_T = 273,
    VEC3_T = 274,
    VEC4_T = 275,
    FUNC = 276,
    NOT = 277,
    AND = 278,
    OR = 279,
    PLUS = 280,
    MINUS = 281,
    TIMES = 282,
    SLASH = 283,
    EXP = 284,
    EQL = 285,
    NEQ = 286,
    LSS = 287,
    LEQ = 288,
    GTR = 289,
    GEQ = 290,
    LPAREN = 291,
    RPAREN = 292,
    LBRACE = 293,
    RBRACE = 294,
    LBRACKET = 295,
    RBRACKET = 296,
    ASSGNMT = 297,
    SEMICOLON = 298,
    COMMA = 299,
    INT_C = 300,
    FLOAT_C = 301,
    ID = 302,
    UNARY_PREC = 303,
    V_F_C_PREC = 304
  };
#endif
/* Tokens.  */
#define IF_SYM 258
#define ELSE_SYM 259
#define WHILE_SYM 260
#define TRUE_SYM 261
#define FALSE_SYM 262
#define CONST_SYM 263
#define BOOL_T 264
#define BVEC2_T 265
#define BVEC3_T 266
#define BVEC4_T 267
#define INT_T 268
#define IVEC2_T 269
#define IVEC3_T 270
#define IVEC4_T 271
#define FLOAT_T 272
#define VEC2_T 273
#define VEC3_T 274
#define VEC4_T 275
#define FUNC 276
#define NOT 277
#define AND 278
#define OR 279
#define PLUS 280
#define MINUS 281
#define TIMES 282
#define SLASH 283
#define EXP 284
#define EQL 285
#define NEQ 286
#define LSS 287
#define LEQ 288
#define GTR 289
#define GEQ 290
#define LPAREN 291
#define RPAREN 292
#define LBRACE 293
#define RBRACE 294
#define LBRACKET 295
#define RBRACKET 296
#define ASSGNMT 297
#define SEMICOLON 298
#define COMMA 299
#define INT_C 300
#define FLOAT_C 301
#define ID 302
#define UNARY_PREC 303
#define V_F_C_PREC 304

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 53 "parser.y" /* yacc.c:355  */

  /* Lexer */
  int as_int;
  float as_float;
  
  char as_id[MAX_IDENTIFIER + 1];
  char as_func[MAX_IDENTIFIER + 1];

  /* Parser */
  int op_type;
  int data_type;
  node *ast_node;

#line 250 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 281 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  63
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  114

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   159,   159,   162,   165,   166,   169,   170,   173,   174,
     175,   178,   179,   180,   181,   182,   185,   186,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   227,   228,   231,   232,   235,   238,   241,   244,
     245,   248,   249,   251
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF_SYM", "ELSE_SYM", "WHILE_SYM",
  "TRUE_SYM", "FALSE_SYM", "CONST_SYM", "BOOL_T", "BVEC2_T", "BVEC3_T",
  "BVEC4_T", "INT_T", "IVEC2_T", "IVEC3_T", "IVEC4_T", "FLOAT_T", "VEC2_T",
  "VEC3_T", "VEC4_T", "FUNC", "NOT", "AND", "OR", "PLUS", "MINUS", "TIMES",
  "SLASH", "EXP", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "ASSGNMT",
  "SEMICOLON", "COMMA", "INT_C", "FLOAT_C", "ID", "UNARY_PREC",
  "V_F_C_PREC", "$accept", "program", "scope", "declarations",
  "statements", "declaration", "statement", "else_statement", "type",
  "expression", "variable", "unary_op", "constructor", "function",
  "function_name", "arguments_opt", "arguments", "epsilon", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

#define YYPACT_NINF -62

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-62)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -28,   -62,    18,   -62,   222,   -62,   -62,   247,   -62,   -62,
     -62,   -62,   -62,   -62,   -62,   -62,   -62,   -62,   -62,   -62,
       6,   -62,   -22,   -62,   -21,    -9,    -7,   -62,   -62,   -10,
     -62,   -62,     4,   -30,     9,    89,    89,     3,    89,    89,
     -62,    89,   -62,   -62,   -62,   -62,   -62,    89,   -62,   -62,
      16,    53,   -62,    89,   -62,   -62,    20,   177,    13,   114,
     135,   156,   192,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    12,   -62,    89,
      12,   -62,   -62,   -62,   -62,   -62,   220,   -16,   243,    37,
      -5,    -5,    28,    28,    28,   254,   254,   254,   254,   254,
     254,    69,    54,    48,   -62,   -62,   -62,    89,    12,   -62,
     -62,   -62,   220,   -62
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    63,     0,     2,    63,     5,     1,     0,    22,    23,
      24,    25,    18,    19,    20,    21,    26,    27,    28,    29,
       0,     4,     0,     7,     0,     0,     0,     3,    15,    52,
      14,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       8,     0,    34,    35,    58,    54,    55,     0,    32,    33,
       0,     0,    36,     0,    30,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    63,
       0,    53,    11,     9,    10,    51,    62,     0,    38,    39,
      46,    47,    48,    49,    50,    40,    41,    42,    43,    44,
      45,    63,     0,    59,    60,    13,    56,     0,     0,    12,
      17,    57,    61,    16
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -62,   -62,    93,   -62,   -62,   -62,   -61,   -62,     0,   -33,
     -19,   -62,   -62,   -62,   -62,   -62,    15,    -4
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,    30,     4,    20,    21,    31,   109,    50,    86,
      52,    53,    54,    55,    56,   102,    87,     5
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      23,    32,    51,    57,    22,    59,    60,    24,    61,    25,
       1,    26,    39,    40,    62,    25,   101,    26,     6,   105,
      78,   106,    68,    69,    70,    33,    34,    35,   107,    36,
      37,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,     1,    27,    38,   113,    58,    28,
       1,    41,    63,    29,    81,    28,    79,    70,    32,    29,
      64,    32,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,   108,   112,   104,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    32,
      77,   111,   107,     3,   103,    42,    43,   110,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      44,    45,     0,     0,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    29,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,     0,    82,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,     0,     0,     0,     0,     0,     0,     0,    83,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,     0,     0,     0,     0,     0,     0,     0,    84,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    80,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    85,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    66,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1
};

static const yytype_int8 yycheck[] =
{
       4,    20,    35,    36,     4,    38,    39,     7,    41,     3,
      38,     5,    42,    43,    47,     3,    77,     5,     0,    80,
      53,    37,    27,    28,    29,    47,    47,    36,    44,    36,
      40,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    38,    39,    42,   108,    45,    43,
      38,    42,    36,    47,    41,    43,    36,    29,    77,    47,
      23,    80,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,     4,   107,    79,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,   108,
      37,    37,    44,     0,    79,     6,     7,   101,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    37,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    38,    51,    52,    53,    67,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      54,    55,    58,    67,    58,     3,     5,    39,    43,    47,
      52,    56,    60,    47,    47,    36,    36,    40,    42,    42,
      43,    42,     6,     7,    21,    22,    26,    36,    45,    46,
      58,    59,    60,    61,    62,    63,    64,    59,    45,    59,
      59,    59,    59,    36,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    37,    59,    36,
      37,    41,    43,    43,    43,    37,    59,    66,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    56,    65,    66,    67,    56,    37,    44,     4,    57,
      67,    37,    59,    56
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    52,    53,    53,    54,    54,    55,    55,
      55,    56,    56,    56,    56,    56,    57,    57,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    60,    60,    61,    61,    62,    63,    64,    65,
      65,    66,    66,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     4,     2,     1,     2,     1,     3,     5,
       6,     4,     6,     5,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     4,     1,     1,     4,     4,     1,     1,
       1,     3,     1,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 159 "parser.y" /* yacc.c:1646  */
    { yTRACE("program -> scope");                                         ast = (yyvsp[0].ast_node);                                                                             }
#line 1568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 162 "parser.y" /* yacc.c:1646  */
    { yTRACE("scope -> { declarations statements }");                     (yyval.ast_node) = ast_allocate(SCOPE_NODE, (yyvsp[-2].ast_node), (yyvsp[-1].ast_node), &(yyloc));                                           }
#line 1574 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 165 "parser.y" /* yacc.c:1646  */
    { yTRACE("declarations -> declarations declaration");                 (yyval.ast_node) = ast_allocate(DECLARATIONS_NODE, (yyvsp[-1].ast_node), (yyvsp[0].ast_node), &(yyloc));                                    }
#line 1580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 166 "parser.y" /* yacc.c:1646  */
    { yTRACE("declarations -> epsilon");                                  (yyval.ast_node) = ast_allocate(DECLARATIONS_NODE, nullptr, nullptr, &(yyloc));                          }
#line 1586 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 169 "parser.y" /* yacc.c:1646  */
    { yTRACE("statements -> statements statement");                       (yyval.ast_node) = ast_allocate(STATEMENTS_NODE, (yyvsp[-1].ast_node), (yyvsp[0].ast_node), &(yyloc));                                      }
#line 1592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 170 "parser.y" /* yacc.c:1646  */
    { yTRACE("statements -> epsilon");                                    (yyval.ast_node) = ast_allocate(STATEMENTS_NODE, nullptr, nullptr, &(yyloc));                            }
#line 1598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 173 "parser.y" /* yacc.c:1646  */
    { yTRACE("declaration -> type ID ;");                                 (yyval.ast_node) = ast_allocate(DECLARATION_NODE, (yyvsp[-1].as_id), 0, (yyvsp[-2].data_type), nullptr, &(yyloc));                         }
#line 1604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 174 "parser.y" /* yacc.c:1646  */
    { yTRACE("declaration -> type ID = expression ;");                    (yyval.ast_node) = ast_allocate(DECLARATION_NODE, (yyvsp[-3].as_id), 0, (yyvsp[-4].data_type), (yyvsp[-1].ast_node), &(yyloc));                              }
#line 1610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 175 "parser.y" /* yacc.c:1646  */
    { yTRACE("declaration -> const type ID = expression ;");              (yyval.ast_node) = ast_allocate(DECLARATION_NODE, (yyvsp[-3].as_id), 1, (yyvsp[-4].data_type), (yyvsp[-1].ast_node), &(yyloc));                              }
#line 1616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 178 "parser.y" /* yacc.c:1646  */
    { yTRACE("statement -> variable = expression ;");                     (yyval.ast_node) = ast_allocate(ASSIGNMENT_NODE, (yyvsp[-3].ast_node), (yyvsp[-1].ast_node), &(yyloc));                                      }
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 179 "parser.y" /* yacc.c:1646  */
    { yTRACE("statement -> if ( expression ) statement else_statement");  (yyval.ast_node) = ast_allocate(IF_STATEMENT_NODE, (yyvsp[-3].ast_node), (yyvsp[-1].ast_node), (yyvsp[0].ast_node), &(yyloc));                                }
#line 1628 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 180 "parser.y" /* yacc.c:1646  */
    { yTRACE("statement -> while ( expression ) statement");              (yyval.ast_node) = ast_allocate(WHILE_STATEMENT_NODE, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                                 }
#line 1634 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 181 "parser.y" /* yacc.c:1646  */
    { yTRACE("statement -> scope");                                       (yyval.ast_node) = ast_allocate(NESTED_SCOPE_NODE, (yyvsp[0].ast_node), &(yyloc));                                        }
#line 1640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 182 "parser.y" /* yacc.c:1646  */
    { yTRACE("statement -> ;");                                           (yyval.ast_node) = ast_allocate(STALL_STATEMENT_NODE, &(yyloc));                                         }
#line 1646 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 185 "parser.y" /* yacc.c:1646  */
    { yTRACE("else_statement -> else statement");                         (yyval.ast_node) = (yyvsp[0].ast_node);                                                                              }
#line 1652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 186 "parser.y" /* yacc.c:1646  */
    { yTRACE("else_statement -> epsilon");                                (yyval.ast_node) = nullptr;                                                                         }
#line 1658 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 189 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> int");                                              (yyval.data_type) = INT_T;                                                                           }
#line 1664 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 190 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> ivec2");                                            (yyval.data_type) = IVEC2_T;                                                                         }
#line 1670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 191 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> ivec3");                                            (yyval.data_type) = IVEC3_T;                                                                         }
#line 1676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 192 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> ivec4");                                            (yyval.data_type) = IVEC4_T;                                                                         }
#line 1682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 193 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> bool");                                             (yyval.data_type) = BOOL_T;                                                                          }
#line 1688 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 194 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> bvec2");                                            (yyval.data_type) = BVEC2_T;                                                                         }
#line 1694 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 195 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> bvec3");                                            (yyval.data_type) = BVEC3_T;                                                                         }
#line 1700 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 196 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> bvec4");                                            (yyval.data_type) = BVEC4_T;                                                                         }
#line 1706 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 197 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> float");                                            (yyval.data_type) = FLOAT_T;                                                                         }
#line 1712 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 198 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> vec2");                                             (yyval.data_type) = VEC2_T;                                                                          }
#line 1718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 199 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> vec3");                                             (yyval.data_type) = VEC3_T;                                                                          }
#line 1724 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 200 "parser.y" /* yacc.c:1646  */
    { yTRACE("type -> vec4");                                             (yyval.data_type) = VEC4_T;                                                                          }
#line 1730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 203 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> constructor");                                (yyval.ast_node) = (yyvsp[0].ast_node);                                                                              }
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 204 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> function");                                   (yyval.ast_node) = (yyvsp[0].ast_node);                                                                              }
#line 1742 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 205 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> integer_literal");                            (yyval.ast_node) = ast_allocate(INT_C_NODE, (yyvsp[0].as_int), &(yyloc));                                               }
#line 1748 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 206 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> float_literal");                              (yyval.ast_node) = ast_allocate(FLOAT_C_NODE, (yyvsp[0].as_float), &(yyloc));                                             }
#line 1754 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 207 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> true");                                       (yyval.ast_node) = ast_allocate(BOOL_C_NODE, 1, &(yyloc));                                               }
#line 1760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 208 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> false");                                      (yyval.ast_node) = ast_allocate(BOOL_C_NODE, 0, &(yyloc));                                               }
#line 1766 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 209 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> variable");                                   (yyval.ast_node) = (yyvsp[0].ast_node);                                                                              }
#line 1772 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 210 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> unary_op expression");                        (yyval.ast_node) = ast_allocate(UNARY_EXPRESION_NODE, (yyvsp[-1].op_type), (yyvsp[0].ast_node), &(yyloc));                                 }
#line 1778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 211 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression && expression");                   (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, AND, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1784 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 212 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression || expression");                   (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, OR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                           }
#line 1790 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 213 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression == expression");                   (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, EQL, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1796 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 214 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression != expression");                   (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, NEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 215 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression < expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, LSS, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1808 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 216 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression <= expression");                   (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, LEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 217 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression > expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, GTR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 218 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression >= expression");                   (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, GEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1826 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 219 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression + expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, PLUS, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                         }
#line 1832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 220 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression - expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, MINUS, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                        }
#line 1838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 221 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression * expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, TIMES, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                        }
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 222 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression / expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, SLASH, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                        }
#line 1850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 223 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> expression ^ expression");                    (yyval.ast_node) = ast_allocate(BINARY_EXPRESSION_NODE, EXP, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                          }
#line 1856 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 224 "parser.y" /* yacc.c:1646  */
    { yTRACE("expression -> (expression)");                               (yyval.ast_node) = (yyvsp[-1].ast_node);                                                                              }
#line 1862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 227 "parser.y" /* yacc.c:1646  */
    { yTRACE("variable -> identifier");                                   (yyval.ast_node) = ast_allocate(ID_NODE, (yyvsp[0].as_id), &(yyloc));                                                  }
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 228 "parser.y" /* yacc.c:1646  */
    { yTRACE("variable -> identifier[integer_literal]");                  (yyval.ast_node) = ast_allocate(INDEXING_NODE, (yyvsp[-3].as_id), ast_allocate(INT_C_NODE, (yyvsp[-1].as_int), &(yylsp[-1])), &(yylsp[-3]) ,&(yyloc));    }
#line 1874 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 231 "parser.y" /* yacc.c:1646  */
    { yTRACE("unary_op -> !");                                            (yyval.op_type) = NOT;                                                                             }
#line 1880 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 232 "parser.y" /* yacc.c:1646  */
    { yTRACE("unary_op -> -");                                            (yyval.op_type) = MINUS;                                                                           }
#line 1886 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 235 "parser.y" /* yacc.c:1646  */
    { yTRACE("constructor -> type ( arguments )");                        (yyval.ast_node) = ast_allocate(CONSTRUCTOR_NODE, (yyvsp[-3].data_type), (yyvsp[-1].ast_node), &(yyloc));                                     }
#line 1892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 238 "parser.y" /* yacc.c:1646  */
    { yTRACE("function -> function_name ( arguments_opt )");              (yyval.ast_node) = ast_allocate(FUNCTION_NODE, (yyvsp[-3].as_func), (yyvsp[-1].ast_node), &(yyloc));                                        }
#line 1898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 241 "parser.y" /* yacc.c:1646  */
    { yTRACE("function_name -> lit | dp3 | rsq");                         strcpy((yyval.as_func), (yyvsp[0].as_func));                                                                       }
#line 1904 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 244 "parser.y" /* yacc.c:1646  */
    { yTRACE("arguments_opt -> arguments");                               (yyval.ast_node) = (yyvsp[0].ast_node);                                                                              }
#line 1910 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 245 "parser.y" /* yacc.c:1646  */
    { yTRACE("arguments_opt -> epsilon");                                 (yyval.ast_node) = ast_allocate(EXPRESSIONS_NODE, nullptr, nullptr, &(yyloc));                           }
#line 1916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 248 "parser.y" /* yacc.c:1646  */
    { yTRACE("arguments -> arguments , expression");                      (yyval.ast_node) = ast_allocate(EXPRESSIONS_NODE, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), &(yyloc));                                     }
#line 1922 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 249 "parser.y" /* yacc.c:1646  */
    { yTRACE("arguments -> expression");                                  (yyval.ast_node) = ast_allocate(EXPRESSIONS_NODE, nullptr, (yyvsp[0].ast_node), &(yyloc));                                }
#line 1928 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1932 "y.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 255 "parser.y" /* yacc.c:1906  */


/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(const char* s) {
  if (errorOccurred)
    return;    /* Error has already been reported by scanner */
  else
    errorOccurred = 1;
        
  fprintf(errorFile, "\nPARSER ERROR, LINE %d",yyline);
  if (strcmp(s, "parse error")) {
    if (strncmp(s, "parse error, ", 13))
      fprintf(errorFile, ": %s\n", s);
    else
      fprintf(errorFile, ": %s\n", s+13);
  } else
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
}

