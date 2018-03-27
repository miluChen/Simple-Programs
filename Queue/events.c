#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "stats.h"

static int systemclock;
static int EventTable[MAXEVENTS];
static job currentjob;

void init_events() {
	long now;

	srand(time(&now));

	EventTable[SUBMITONE] = expdistr(MEANSUBMIT);
	EventTable[SUBMITTWO] = expdistr(MEANSUBMIT);
	EventTable[JOBCOMPLETE] = UNUSED;
	EventTable[CPUTIMEOUT] = UNUSED;
	systemclock = 0;
	currentjob.runtime = 0;
}

int next_entry() {
	int event = 0;
	int i;

	for (i = 1; i < MAXEVENTS; i++)
		if (EventTable[i] < EventTable[event])
			event = i;

	advance_clock(EventTable[event]);
	return event;
}

int advance_clock(int incr) {
	int i;

	for (i=0; i<MAXEVENTS; EventTable[i++] -= incr)
		;
	return systemclock += incr;
}

status submitjob(queue *p_Q, int event) {
	if (qadd_job(p_Q, systemclock, 0, expdistr(MEANRUN)) == ERROR)
		return ERROR;
	printf("%5d Submitting job\n", systemclock);
	// expdirstr(MEANSUBMIT) represents the amount of time 
	// that must elapse before the next submit event should occur
	EventTable[event] = expdistr(MEANSUBMIT);
	return startjob(p_Q); // there may be a job is running
}

status startjob(queue *p_Q) {
	int base, elapse, runtime;
	// check whether there is a job running
	if (currentjob.runtime != 0)
		return OK;
	if (empty_queue(p_Q) == TRUE)
		return OK;
	if (qremove_job(p_Q, &base, &elapse, &runtime) == ERROR)
		return ERROR;
	// update the job's elapsedtime
	currentjob.runtime = runtime;
	currentjob.elapsedtime = elapse + systemclock - base;
	EventTable[CPUTIMEOUT] = CPULIMIT;
	EventTable[JOBCOMPLETE] = runtime;
	printf("%5d Starting job\n", systemclock);
	return OK;
}

status finishjob(queue *p_Q) {
	EventTable[CPUTIMEOUT] = UNUSED;
	EventTable[JOBCOMPLETE] = UNUSED;
	accumulate_stats(currentjob.elapsedtime);
	currentjob.runtime = 0;
	printf("%5d Finishing job\n", systemclock);
	return startjob(p_Q);
}

status requeuejob(queue *p_Q) {
	EventTable[CPUTIMEOUT] = UNUSED;
	EventTable[JOBCOMPLETE] = UNUSED;
	if (qadd_job(p_Q, systemclock, currentjob.elapsedtime, currentjob.runtime - CPULIMIT) == ERROR)
		return ERROR;

	currentjob.runtime = 0;
	printf("%5d Requeueing job\n", systemclock);
	return startjob(p_Q);
}

// return a random number in an exponential distribution about meantime
// Cumulative distribution function: F(x,lambda) = 1-e^(-lambda * x) ==> x = -1/lambda * log(1-F(x, lambda))
int expdistr(int meantime) {
	return (int)(-meantime * log((float)rand() / RAND_MAX));
}
