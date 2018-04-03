#include <stdio.h>
#include "line_editor.h"

void printerror(int errnum) {
	static char *errmsg[] = {
		"io error",
		"out of memory space",
		"invalid line specification",
		"invalid command",
		"error deleting lines"
	};

	if (errnum < 0 || errnum >= MAXERROR) {
		printf("System Error. Invalid error number: %d\n", errnum);
		return;
	}
	printf("%s\n", errmsg[errnum-1]);
}

// read and write files
