#ifndef __C_QUEUES__
#define __C_QUEUES__

#include "../Linked_List/lists.h"
#include "../include/types.h"

#define FRONT(Q) ((Q)->front)
#define REAR(Q) ((Q)->rear)

typedef struct {
	node *front;
	node *rear;
} queue;

status init_queue(queue *p_Q);
bool empty_queue(queue *p_Q);
status qadd(queue *p_Q, generic_ptr data);
status qremove(queue *p_Q, generic_ptr *p_data);

#endif
