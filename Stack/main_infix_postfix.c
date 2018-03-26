/* Infix-to-Postfix Mathmatical notation translation */
/*
	for each char , c, in the input line:
		if c is a variable, output c
		if c is an operator then
			(let t be on the top of the stack)
			while the stack is not empty and stack priority(t) >= input priority(c)
				pop the stack and output t
			push c onto the stack
		if c is a right parenthesis
			while the stack is not empty and the stack top is not "("
				pop the stack and output t
			pop the "(" off the stack
	while the stack is not empty
		pop the stack and output the operator
*/

#include <stdio.h>
#include <stdlib.h>
#include "infix_postfix.h"

int main() {
	/* Peform an infix-to-postfix language translation
	   Algorithm:
			if precedence(input) > precedenc(stack), push(input)
			otherwise while (prec(stack) >= prec(input)) pop and print
	   Infix expressions must be complete on 1 line.
	*/
	stack S;
	tokendata *p_token;
	char stacksymbol;
	bool eofreached = FALSE;

	init_stack(&S);
	PUSH(&S, BOTTOMMARKER);
	do {
		p_token = gettoken();
		switch (p_token->tokentype) {
			case EOF_T:
				eofreached = TRUE;
				break;
			case VARIABLE_T:
				printf("%c ", p_token->tokenvalue);
				break;
			case OPERATOR_T:
				while (stackprec(&S) >= inputprec(p_token->tokenvalue)) {
					POP(&S, &stacksymbol);
					printf("%c ", stacksymbol);
				}
				PUSH(&S, p_token->tokenvalue);
				break;
			case RIGHTPAREN_T:
				do {
					POP(&S, &stacksymbol);
					if (stacksymbol == BOTTOMMARKER) {
						printf("Error in expression.\n");
						skiptoeol();
						clearstack(&S);
						break;
					}
					if (stacksymbol != '(')
						printf("%c ", stacksymbol);
				} while (stacksymbol != '(');
				break;
			case EOL_T:
				TOP(&S, &stacksymbol);
				while (stacksymbol != BOTTOMMARKER) {
					POP(&S, &stacksymbol);
					if (stacksymbol == '(') {
						printf("Error in expression.\n");
						clearstack(&S);
					}
					else
						printf("%c ", stacksymbol);
					TOP(&S, &stacksymbol);
				}
				putchar('\n');
				break;
		}
	} while (eofreached == FALSE);

	return 0;
}
