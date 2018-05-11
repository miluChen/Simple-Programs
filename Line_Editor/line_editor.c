#include <stdio.h>
#include "line_editor.h"

static status writeline(char *s);

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
int readfile(char *filename, double_list *p_L) {
	char buffer[BUFFSIZE];
	FILE *fd;

	if ((fd = fopen(filename, "r")) == NULL)
		return E_IO;
	while (fgets(buffer, BUFFSIZE, fd) != NULL) {
		if (string_double_append(p_L, buffer) == ERROR)
			return E_SPACE;
	}
	fclose(fd);
	return 0;	
}

static FILE *outputfd;
int writefile(char *filename, double_list *p_L) {
	status rc;

	if ((outputfd = fopen(filename, "w")) == NULL)
		return E_IO;
	rc = double_traverse(*p_L, writeline);
	fclose(outputfd);
	return (rc == ERROR) ? E_IO : 0;
}

status writeline(char *s) {
	if (fputs(s, outputfd) == EOF)
		return ERROR;
	return OK;
}

// insert new lines into file.
// the function prompts the user for the lines to be added and inserts them before the line indicated
// the end of user input is indicated by a period (".") on a line by itself.
// TODO: Problem: it is not possible to insert lines at the very end of the file
int insertlines(char *linespec, double_list *p_head, double_list *p_current) {
	double_list newdata, startnode, endnode;
	status rc;
	int cmp, parseerror;
	char buffer[BUFFSIZE];

	if (empty_double_list(*p_head) == TRUE) {
		if (strlen(linespec) != 0)
			return E_LINES;	// empty file should not have linespec
		startnode = endnode = NULL;
	}
	else {
		// the given linespec better be a single line number
		parseerror = parse_linespec(linespec, *p_head, *p_current, &startnode, &endnode);
		if (parseerror)
			return parseerror;
		if (startnode != endnode)
			return E_LINES;
	}
	// collect the new lines in newdata. Then "paste" the list before startnode
	init_double_list(&newdata);
	do {
		printf("insert>");
		fgets(buffer, BUFFSIZE, stdin);
		cmp = strcmp(buffer, ".\n");
		if (cmp != 0) {
			rc = string_double_append(&newdata, buffer);
			if (rc == ERROR)
				return E_SPACE;
		}
	} while (cmp != 0);
	if (empty_double_list(newdata) == TRUE)
		return 0;

	if (startnode == NULL) {
		// empty list
		*p_head = newdata;
		*p_current = nth_double_node(newdata, -1);
	}
	else if (PREV(startnode) == NULL) {
		// insert before the first line
		double_list lastnode = nth_double_node(newdata, -1);
		paste_list(&lastnode, p_head);
		*p_head = newdata;
		*p_current = startnode;
	}
	else {
		// insert in the middle of the list
		paste_list(&PREV(startnode), &newdata);
		*p_current = startnode;
	}
	return 0;
}

int deletelines(char *linespec, double_list *p_head, double_list *p_current) {
	// delete some lines from p_head
	// update p_current to be after last line deleted
	// if the last line is deleted, make p_current be before first line
	double_list startnode, endnode, tmplist;
	double_list new_current;
	int startnumber, endnumber;
	int rc;

	rc = parse_linespec(linespec, *p_head, *p_current, &startnode, &endnode);
	if (rc)
		return rc;

	startnumber = double_node_number(startnode);
	endnumber = double_node_number(endnode);
	if (startnumber > endnumber) {
		tmplist = startnode;
		startnode = endnode;
		endnode = tmplist;
	}
	new_current = nth_relative_double_node(endnode, 1);
	if (new_current == NULL)
		new_current = nth_relative_double_node(startnode, -1);

	cut_list(p_head, &startnode, &endnode);

	*p_current = new_current;
	destroy_double_list(&startnode, free);
	return 0;
}

// print a range of lines
int printlines(char *linespec, double_list *p_head, double_list *p_current) {
	// print out lines. Direction indicates whether going forward or backward
	double_list startnode, endnode;
	int startnumber, endnumber, count, direction;
	int rc;

	rc = parse_linespec(linespec, *p_head, *p_current, &startnode, &endnode);
	if (rc)
		return rc;

	startnumber = double_node_number(startnode);
	endnumber = double_node_number(endnode);
	direction = (startnumber < endnumber ) ? 1 : -1;
	count = (endnumber - startnumber) * direction + 1;
	while (count-- > 0) {
		printf("%d %s", startnumber, DATA(startnode));
		startnumber += direction;
		startnode = nth_relative_double_node(startnode, direction);
	}
	*p_current = endnode;
	return 0;
}

// TODO: Problem: it is not possible to move lines to the very end of the file
int movelines(char *linespec, double_list *p_head, double_list *p_current) {
	// move lines to after p_current. make sure the lines moved do not include p_current
	double_list startnode, endnode;
	double_list tmpnode;
	int startnumber, endnumber;
	int rc, currentnumber;
	int tmp;

	rc = parse_linespec(linespec, *p_head, *p_current, &startnode, &endnode);
	if (rc)
		return rc;

	startnumber = double_node_number(startnode);
	endnumber = double_node_number(endnode);
	currentnumber = double_node_number(*p_current);
	// make sure start < end
	if (startnumber > endnumber) {
		tmp = startnumber;
		startnumber = endnumber;
		endnumber = tmp;
		tmpnode = startnode;
		startnode = endnode;
		endnode = tmpnode;
	}
	// do not include the current line in the ones being moved
	if (currentnumber >= startnumber && currentnumber <= endnumber)
		return E_LINES;

	cut_list(p_head, &startnode, &endnode);
	paste_list(&PREV(*p_current), &startnode);
	return 0;
}
