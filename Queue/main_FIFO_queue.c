#include "queue_jobdata.h"
#include "stats.h"
#include "events.h"

#define NUMJOBS 100

int main() {
	queue cpuq;
	int entry;

	init_events();
	init_stats();
	init_queue(&cpuq);

	while (jobscompleted() < NUMJOBS) {
		switch (entry = next_entry()) {
			case SUBMITONE:
			case SUBMITTWO:
				submitjob(&cpuq, entry);
				break;
			case JOBCOMPLETE: // need to check JOBCOMPLETE before CPUTIMEOUT
				finishjob(&cpuq);
				break;
			case CPUTIMEOUT:
				requeuejob(&cpuq);
				break;
		}
	}
	print_stats();
}
