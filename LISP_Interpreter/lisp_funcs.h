#ifndef __C_LISP_FUNCTIONS__
#define __C_LISP_FUNCTIONS__

#include "lisp_struct.h"

#define MAXBUILTIN 5

int evaluate_function(char *name, list args, lisp_expression *p_value);
int c_car(list arglist, lisp_expression *p_return);
int c_cdr(list arglist, lisp_expression *p_return);
int c_cons(list arglist, lisp_expression *p_return);
int c_eval(list arglist, lisp_expression *p_return);
int c_exit(list arglist, lisp_expression *p_return);

#endif
