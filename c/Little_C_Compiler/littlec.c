/* A Little C Compiler. */

#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define NUM_FUNC 	100
#define NUM_GLOBAL_VARS 100
#define NUM_LOCAL_VARS	200
#define NUM_BLOCK	100
#define ID_LEN		31
#define FUNC_CALLS	31
#define NUM_PARAMS	31
#define PROG_SIZE	10000
#define LOOP_NEST	31

enum tok_types {DELIMITER, IDENTIFIER, NUMBER, KEYWORD, TEMP, STRING, BLOCK};

/* add additional C keyword tokens here. */
enum tokens {ARG, CHAR, INT, IF, ELSE, FOR, DO, WHILE, SWITCH, RETURN, EOL, FINISHED, END};

/* ADD Additional double operators here (such as ->). */
enum double_ops {LT=1, LE, GT, GE, EQ, NE};

/* These are the constants used to call sntx_err() when a syntax error occurs. Add more if you like.
NOTE: SYNTAX is a generic error message used when nothing else seems appropriate. */
enum error_msg {SYNTAX, UNBAL_PARENS, NO_EXP, EQUALS_EXPECTED, NOT_VAR, PARAM_ERR, SEMI_EXPECTED, UNBAL_BRACES, FUNC_UNDEF, TYPE_EXPECTED, NEST_FUNC, RET_NOCALL, PAREN_EXPECTED, WHILE_EXPECTED, QUOTE_EXPECTED, NOT_TEMP, TOO_MANY_LVARS, DIV_BY_ZERO};

char *prog;	/* current location in source code. */
char *p_buf;	/* points to start of program buffer. */
jmp_buf e_buf;	/* hold environment for longjmp(). */

/* An array of these structures will hold the info associated with global variables. */
struct var_type 
{
	char var_name[ID_LEN];
	int v_type;
	int value;
} global_vars[NUM_GLOBAL_VARS];

struct var_type local_var_stack[NUM_LOCAL_VARS];

struct func_type
{
	char func_name[ID_LEN];
	int ret_type;
	char *loc;	/* location of entry point in file. */
} func_table[NUM_FUNC];

int call_stack[NUM_FUNC];

struct commands		/* Keyword lookup table. */
{	

	char command[20];
	char tok;
} table[] = {	/* Commands must be entered lowercase. */
	"if", IF,  /* in this table. */
	"else", ELSE, 
	"for", FOR, 
	"do", DO, 
	"while", WHILE, 
	"char", CHAR, 
	"int", INT, 
	"return", RETURN, 
	"end", END, 
	"", END		/* Mark end of table. */
};

char token[80];
char token_type, tok;

int functions;		/* index to top of the function call stack. */
int func_index;		/* index into function table. */
int gvar_index;		/* index into global variable table. */
int lvartos;		/* index into local variable stack */
int ret_value;		/* function return value. */

void print(void), prescan(void);


