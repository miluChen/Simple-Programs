#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "lisp_funcs.h"
#include "lisp_interpreter.h"

int evaluate_function(char *name, list args, lisp_expression *p_value) {
	// evaluate the built-in funciton, name
	static struct {
		char *func_name;
		int (*f)();
	} builtin[MAXBUILTIN] = {
		{"CAR", c_car},
		{"CDR", c_cdr},
		{"CONS", c_cons},
		{"EVAL", c_eval},
		{"EXIT", c_exit}
	};
	int i;

	for (i=0; i<MAXBUILTIN; i++) {
		if (strcmp(name, builtin[i].func_name) == 0)
			return builtin[i].f(args, p_value);
	}
	return E_EVAL;
}

// arglist must contain a single node that is a list. The first node in that list is returned
int c_car(list arglist, lisp_expression *p_return) {
	lisp_expression car_list;

	if (circ_length(arglist) != 1)
		return E_EVAL;
	car_list = (lisp_expression)DATA(nth_node(arglist, 1));
	if (LISP_TYPE(car_list) != LIST)
		return E_EVAL;
	*p_return = (lisp_expression)DATA(nth_node(LIST_VALUE(car_list), 1));
	return 0;
}

// arglist must contain a single node that must be a list. 
// Create a new expression that contains all but the first element of that list.
// this function modifies its argument
int c_cdr(list arglist, lisp_expression *p_return) {
	lisp_expression expression;
	lisp_expression cdr_list;
	list car_node;

	if (circ_length(arglist) != 1)
		return E_EVAL;
	cdr_list = (lisp_expression)DATA(nth_node(arglist, 1));
	if (LISP_TYPE(cdr_list) != LIST)
		return E_EVAL;
	if ((expression = new_expression()) == NULL)
		return E_EVAL;
	// get rid of the first argument
	car_node = nth_node(LIST_VALUE(cdr_list), 1);
	free(DATA(car_node));
	if (circ_delete_node(&LIST_VALUE(cdr_list), car_node) == ERROR) {
		return E_EVAL;
	}
	if (empty_circ_list(LIST_VALUE(cdr_list)) == TRUE) {
		// expression should be the NIL atom
		LISP_TYPE(expression) = ATOM;
		ATOM_VALUE(expression) = new_atom(NIL_VALUE);
		if (ATOM_VALUE(expression) == NULL)
			return E_SPACE;
	}
	else {
		LISP_TYPE(expression) = LIST;
		LIST_VALUE(expression) = LIST_VALUE(cdr_list);
	}
	*p_return = expression;
	return 0;
}

// arglist must contain two items, the second of which must be a list
// the firt item is inserted at the front of the other list
// this function modifies its argument
int c_cons(list arglist, lisp_expression *p_return) {
	lisp_expression arg1, arg2;

	if (circ_length(arglist) != 2)
		return E_EVAL;

	arg1 = (lisp_expression)DATA(nth_node(arglist, 1));
	arg2 = (lisp_expression)DATA(nth_node(arglist, 2));
	if (LISP_TYPE(arg2) != LIST)
		return E_EVAL;
	if (circ_insert(&LIST_VALUE(arg2), (generic_ptr)arg1) == ERROR)
		return E_EVAL;
	*p_return = arg2;
	return 0;
}

int c_eval(list arglist, lisp_expression *p_return) {
	if (circ_length(arglist) != 1)
		return E_EVAL;
	return eval_expression((lisp_expression)DATA(arglist), p_return);
}

int c_exit(list arglist, lisp_expression *p_return) {
	return E_EXIT;
}
