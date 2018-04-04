#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "parser.h"
#include "macros.h"

static int parse_number(char *numberspec, double_list head, double_list current, double_list *p_node);

int parse_linespec(char *linespec, double_list head, double_list current, double_list *p_start, double_list *p_end) {
	// parse linespec
	// set p_start to the starting line and p_end to the ending line
	int rc;
	char *nextnumber;

	if (*linespec == '\0') {
		*p_start = current;
	}
	else {
		rc = parse_number(linespec, head, current, p_start);
		if (rc)
			return rc;
	}
	nextnumber = strchr(linespec, ',');

	if (nextnumber == NULL) {
		*p_end = *p_start;
	}
	else {
		rc = parse_number(nextnumber + 1, head, current, p_end);
		if (rc)
			return rc;
	}
	if (*p_start == NULL || *p_end == NULL)
		return E_LINES;
	return 0;
}

int parse_number(char *numberspec, double_list head, double_list current, double_list *p_node) {
	// parse a single numberspec
	char numberbuffer[BUFFSIZE], *p_num;
	int nodenumber;
	int direction;

	if (*numberspec == '.') {
		*p_node = current;
		numberspec++;
	}
	else if (*numberspec == '$') {
		// start with the last line
		*p_node = nth_double_node(head, -1);
		if (*p_node == NULL)
			return E_LINES;
		numberspec++;
	}
	else if (isdigit(*numberspec)) {
		// read the line number
		p_num = numberbuffer;
		while (isdigit(*numberspec))
			*p_num++ = *numberspec++;
		*p_num = '\0';
		nodenumber = atoi(numberbuffer);
		*p_node = nth_double_node(head, nodenumber);
		if (*p_node == NULL)
			return E_LINES;
	}
	else
		return E_LINES;

	// check whether there is plus or minus
	if (*numberspec == '+') {
		direction = 1;
		numberspec++;
	}
	else if (*numberspec == '-') {
		direction = -1;
		numberspec++;
	}
	else
		direction = 0;

	if (isdigit(*numberspec) && direction != 0) {
		p_num = numberbuffer;
		while (isdigit(*numberspec))
			*p_num++ = *numberspec++;
		*p_num = '\0';
		nodenumber = atoi(numberbuffer) * direction;
		*p_node = nth_relative_double_node(*p_node, nodenumber);
		if (p_node == NULL)
			return E_LINES;
		direction = 0;
	}

	if (direction == 0 && (*numberspec == '\0' || *numberspec == ','))
		return 0;
	else
		return E_LINES;
}
