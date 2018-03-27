#ifndef __C_EVENTS__
#define __C_EVENTS__

#include "queue_jobdata.h"

#define MAXEVENTS 4
#define SUBMITONE 0
#define SUBMITTWO 1
#define JOBCOMPLETE 2
#define CPUTIMEOUT 3

#define MEANSUBMIT 8
#define MEANRUN 15
#define CPULIMIT 20
#define UNUSED RAND_MAX

void init_events();
int next_entry();
int advance_clock(int incr);
status submitjob(queue *p_Q, int event);
status startjob(queue *p_Q);
status finishjob(queue *p_Q);
status requeuejob(queue *p_Q);
// return a random number in an exponential distribution about meantime
int expdistr(int meantime);

#endif
