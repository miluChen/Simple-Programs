#ifndef __C_STACKS__
#define __C_STACKS__

#include "../include/types.h"

#define STACKINCREMENT 100
#define current_stacksize(p_S) ((p_S)->top - (p_S)->base)

typedef struct {
	generic_ptr *base;
	generic_ptr *top;
	int stacksize;
} stack;

status init_stack(stack *p_S);
bool empty_stack(stack *p_S);
status push(stack *p_S, generic_ptr data);
status pop(stack *p_S, generic_ptr *p_data);
status top(stack *p_S, generic_ptr *p_data);

#endif
