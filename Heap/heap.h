#ifndef __HEAP__
#define __HEAP__

#include "../include/types.h"

#define HEAPINCREMENT 128

typedef struct {
	generic_ptr *base;
	int nextelement;
	int heapsize;
} heap;

status init_heap(heap *p_H);
bool empty_heap(heap *p_H);
status heap_insert(heap *p_H, generic_ptr *data, int (*p_cmp_f)());
status heap_delete(heap *p_H, int element, generic_ptr *p_data, int (*p_cmp_f)());

#endif
