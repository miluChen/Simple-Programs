#include <stdio.h>
#include <stddef.h>
#include "lisp_interpreter.h"
#include "lexical_analyzer.h"
#include "lisp_funcs.h"

/*
	read s-expression:
	if the token is a left parenthesis
		if the next token is a right parenthesis
			create a null list
		otherwise
			until the next token is right parenthesis do
				recursively read the inner expression
				append the inner expression as a node on the current expression list
	otherwise if the token is an apostrophe
		expand the apostrophe into (QUOTE...) by creating a list with the 
		first argument "QUOTE" and appending the next expression to that list.
	otherwise if the token is a string
		the expression is an atom 
*/
int read_expression(lisp_expression *p_expression) {
	/* read a lisp expression; if no errors, return 0 */
	list expr_list;
	lisp_expression expression, inner_expression;
	tokendata *p_token, *p_next_token;
	lisp_atom value;
	status rc;
	int readerror;

	p_token = gettoken();
	if (p_token->tokentype == EOF_T)
		return E_EOF;

	if ((expression = new_expression()) == NULL)
		return E_SPACE;

	switch (p_token->tokentype) {
		case RIGHTPAREN_T:	// can't be a lone ')'
			return E_SYNTAX;
		case STRING_T: // an atome
			if ((value = new_atom(p_token->tokenvalue)) == NULL)
				return E_SPACE;
			LISP_TYPE(expression) = ATOM;
			ATOM_VALUE(expression) = value;
			break;
		case LEFTPAREN_T: // the start of a list
			p_next_token = lookahead();
			if (p_next_token->tokentype == EOF_T)
				return E_EOF;
			if (p_next_token->tokentype == RIGHTPAREN_T) {
				// it's a null list
				if ((value = new_atom(NIL_VALUE)) == NULL)
					return E_SPACE;
				LISP_TYPE(expression) = ATOM;
				ATOM_VALUE(expression) = value;
			}
			else {
				// read the inner expressions
				init_circ_list(&expr_list);
				do {
					readerror = read_expression(&inner_expression);
					if (readerror != 0)
						return readerror;
					rc = circ_append(&expr_list, (generic_ptr)inner_expression);
					if (rc == ERROR)
						return E_SPACE;
					p_next_token = lookahead();
					if (p_next_token->tokentype == EOF_T)
						return E_EOF;
				} while (p_next_token->tokentype != RIGHTPAREN_T);
				LISP_TYPE(expression) = LIST;
				LIST_VALUE(expression) = expr_list;
			}
			p_token = gettoken();	// remove the right parenthesis
			break;
		case QUOTE_T:	// an apostrophe: create the QUOTE token and make the next expression the second element in the list
			p_next_token = lookahead();
			if (p_next_token->tokentype == EOF_T)
				return E_EOF;
			if (p_next_token->tokentype == RIGHTPAREN_T)
				return E_SYNTAX;
			else {
				init_circ_list(&expr_list);
				if ((inner_expression = new_expression()) == NULL)
					return E_SPACE;
				if ((value = new_atom(QUOTE_VALUE)) == NULL)
					return E_SPACE;
				LISP_TYPE(inner_expression) = ATOM;
				ATOM_VALUE(inner_expression) = value;
				rc = circ_append(&expr_list, (generic_ptr)inner_expression);
				if (rc == ERROR)
					return E_SPACE;
				readerror = read_expression(&inner_expression);
				if (readerror != 0)
					return readerror;
				rc = circ_append(&expr_list, (generic_ptr)inner_expression);
				if (rc == ERROR)
					return E_SPACE;

				LISP_TYPE(expression) = LIST;
				LIST_VALUE(expression) = expr_list;
			}
			break;
	}

	*p_expression = expression;
	return 0;
}

status print_expression(lisp_expression expression) {
	if (LISP_TYPE(expression) == ATOM)
		printf("%s ", ATOM_VALUE(expression));
	else {
		printf("( ");
		circ_traverse(LIST_VALUE(expression), print_expression);
		printf(")");
	}
	return OK;
}

/*
	evaluate the S-Expression, assumptions:
	The value an atom is the atom name.
	The value of a list is obtained by assuming the first element of the list is a function name
	The first element must be an atom
	The function is applied to the rest of the arguments in the list after the arguments are evaluated.
	Exception: "QUOTE" returns its arguments unevaluated.
*/
int eval_expression(lisp_expression expression, lisp_expression *p_value) {
	list L, arg;
	lisp_atom function_name;
	list sublist;
	lisp_expression subexpr;
	lisp_expression return_value;
	int rc;

	if (LISP_TYPE(expression) == ATOM) {
		*p_value = expression;
		return 0;
	}

	L = LIST_VALUE(expression);
	// get the first list member, which should be the function name
	arg = circ_list_iterator(L, NULL);
	if (LISP_TYPE(DATA(arg)) != ATOM)
		return E_EVAL;
	function_name = ATOM_VALUE(DATA(arg));

	if (equal_atom(function_name, QUOTE_VALUE)) {
		if (circ_length(L) != 2)
			return E_EVAL;
		*p_value = (lisp_expression)DATA(circ_list_iterator(L, arg));
		return 0;
	}
	// evalute the inner expression if there are any
	init_circ_list(&sublist);
	if (circ_length(L) > 1) {
		while ((arg = circ_list_iterator(L, arg)) != NULL) {
			rc = eval_expression((lisp_expression)DATA(arg), &subexpr);
			if (rc)
				return rc;
			if (circ_append(&sublist, (generic_ptr)subexpr) == ERROR)
				return E_EVAL;
		}
	}
	rc = evaluate_function(function_name, sublist, &return_value);
	if (rc)
		return rc;
	*p_value = return_value;
	return 0;
}

void printerror(int errnum) {
	static char *errmsg[] = {
		"unexpected end of file",
		"out of memory space",
		"syntax error",
		"error evaluating expression"
	};

	if (errnum < 0 || errnum >= MAXERROR) {
		printf("System error. Invalid error number: %d\n", errnum);
		return;
	}
	printf("%s\n", errmsg[errnum-1]);
}
