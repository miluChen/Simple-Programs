#ifndef __C_LISP_LEXICAL_ANALYZER__
#define __C_LISP_LEXICAL_ANALYZER__

#define MAXTOKENLENGTH 32

typedef enum {QUOTE_T, RIGHTPAREN_T, LEFTPAREN_T, STRING_T, EOF_T} inputtype;

typedef struct tokendata {
	inputtype tokentype;
	char tokenvalue[MAXTOKENLENGTH];
} tokendata;

tokendata *gettoken();
tokendata *lookahead();

#endif
