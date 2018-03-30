#ifndef __C_LISP_INTERPRETER__
#define __C_LISP_INTERPRETER__

#include "lisp_struct.h"

void printerror(int errnum);
int read_expression(lisp_expression *p_expression);
int eval_expression(lisp_expression expression, lisp_expression *p_value);
status print_expression(lisp_expression expression);

#endif
