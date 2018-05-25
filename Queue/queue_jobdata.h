#ifndef __C_QUEUE_JOBDATA__
#define __C_QUEUE_JOBDATA__

// #include "queues.h"
#include "priority_queue.h"

typedef struct {
	int basetime;	// system clock time when the job was put into the queue
	int elapsedtime;// time elapsed after job was submitted
	int runtime;	// time needed to run to finish the job
} job;

status qadd_job(queue *p_Q, int base, int elapse, int run);
status qremove_job(queue *p_Q, int *p_base, int *p_elapse, int *p_run);

#endif
