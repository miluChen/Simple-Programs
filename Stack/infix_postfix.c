#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "infix_postfix.h"

int stackprec(stack *p_S) {
	char topsymbol;

	TOP(p_S, &topsymbol);
	switch (topsymbol) {
		case '(':
			return 0;
		case '*':
		case '/':
			return 2;
		case '+':
		case '-':
			return 1;
		case '$':
			return -1;
		default:
			printf("Unknown symbol on stack: %c\n", topsymbol);
			return -1;
	}
	printf("Reached an unreachable section of code!\n");
	return -1;
}

int inputprec(char c) {
	switch (c) {
		case '(':
			return 3;
		case '*':
		case '/':
			return 2;
		case '+':
		case '-':
			return 1;
		default:
			printf("Unknown operator in input: %c\n", c);
			return -1;
	}
	printf("Reached an unreachable section of code!\n");
	return -1;
}

void skiptoeol() {
	tokendata *p_token;

	do {
		p_token = gettoken();
	} while (p_token->tokentype != EOL_T && p_token->tokentype != EOF_T);
	putchar('\n');
}

void clearstack(stack *p_S) {
	char c;

	while (empty_stack(p_S) == FALSE)
		POP(p_S, &c);
	PUSH(p_S, BOTTOMMARKER);
}

tokendata *gettoken() {
	static bool eof = FALSE;
	static char buffer[BUFFSIZE], *bufptr = NULL;
	static tokendata token;
	char *operators = "+-*/(";

	if (eof == TRUE) {
		token.tokentype = EOF_T;
		return &token;
	}

	if (bufptr == NULL) {
		/* no data currently in buffer, so read an entire line */
		printf("? ");
		if ((bufptr = fgets(buffer, BUFFSIZE, stdin)) == NULL || *bufptr == '\0') {
			eof = TRUE;
			token.tokentype = EOF_T;
			return &token;
		}
	}

	while (isspace(*bufptr) && (bufptr - buffer < BUFFSIZE))
		bufptr++;

	if (*bufptr == '\0')
		token.tokentype = EOL_T;
	else if (*bufptr == ')')
		token.tokentype = RIGHTPAREN_T;
	else if (strchr(operators, *bufptr) != NULL)
		token.tokentype = OPERATOR_T;
	else
		token.tokentype = VARIABLE_T;

	token.tokenvalue = *bufptr;

	if (token.tokentype == EOL_T)
		bufptr = NULL;
	else
		bufptr++;

	return &token;
}
