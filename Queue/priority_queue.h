#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_

#include "../Heap/heap.h"

typedef heap queue;
#define init_queue(p_Q) init_heap(p_Q)
#define empty_queue(p_Q) empty_heap(p_Q)
#define qadd(p_Q, data, p_cmp_f) heap_insert(p_Q, data, p_cmp_f)
#define qremove(p_Q, p_data, p_cmp_f) heap_delete(p_Q, 0, p_data, p_cmp_f)

#endif
