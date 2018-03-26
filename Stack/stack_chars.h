#ifndef __C_STACK_CHAR__
#define __C_STACK_CHAR__

#include "stacks.h"
#include "../include/types.h"

status push_char(stack *p_S, char c);
status pop_char(stack *p_S, char *p_c);
status top_char(stack *p_S, char *p_c);

#endif
