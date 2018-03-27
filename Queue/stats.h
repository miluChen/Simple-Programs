#ifndef __C_STATS__
#define __C_STATS__

void init_stats();
void accumulate_stats(int qtime);
void print_stats();
int jobscompleted();

#endif
