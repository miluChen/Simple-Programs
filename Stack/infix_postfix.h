#ifndef __C_INFIX_POSTFIX__
#define __C_INFIX_POSTFIX__

#include "stacks.h"
#include "stack_chars.h"

#define BOTTOMMARKER '$'
#define BUFFSIZE 100
#define PUSH(s, c) \
	if (push_char(s, c) == ERROR) {\
		printf("Fatal error in pushing symbol on stack.\n");\
		exit(1);\
	}
#define POP(s, c) \
	if (pop_char(s, c) == ERROR) {\
		printf("Fatal error in popping symbol off stack.\n");\
		exit(1);\
	}
#define TOP(s, c)\
	if (top_char(s, c) == ERROR) {\
		printf("Fatal error in top operation.\n");\
		exit(1);\
	}


typedef enum { EOF_T, EOL_T, OPERATOR_T, VARIABLE_T, RIGHTPAREN_T } inputtype;
typedef struct {
	inputtype tokentype;
	char tokenvalue;
} tokendata;

void clearstack(stack *p_S);
// return the precedence of the operator at the top the stack
int stackprec(stack *p_S);
// return the precedence of the input operator
int inputprec(char c);
// read tokens untill the end of the line
void skiptoeol();
// lexical analyzer
tokendata *gettoken();

#endif
