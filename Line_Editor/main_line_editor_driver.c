#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "line_editor.h"

int main() {
	char filename[BUFFSIZE];
	char buffer[BUFFSIZE];
	double_list linelist, currentline;
	bool file_edited, exit_flag;
	int rc;

	init_double_list(&linelist);
	printf("Enter the name of the file to edit:");
	if (fgets(buffer, BUFFSIZE, stdin) == NULL) {
		printf("Reading filename error\n");
		exit(1);
	}
	if ((rc = readfile(filename, &linelist)) != 0) {
		printerror(rc);
		exit(1);
	}
	printf("%d lines read.\n", double_length(linelist));
	currentline = nth_double_node(linelist, -1);
	file_edited = FALSE;
	exit_flag = FALSE;
	while (exit_flag == FALSE) {
		printf("cmd: ");
		fgets(buffer, BUFFSIZE, stdin);
		/*
			the following commands are implemented:
			p -- print
			d -- delete
			i -- insert
			m -- move
			w -- write
			q -- quit
		*/
		switch (toupper(buffer[0])) {
			case '\0':
				break;
			case 'P':
				rc = printlines(&buffer[1], &linelist, &currentline);
				if (rc)
					printerror(rc);
				break;
			case 'D':
				file_edited = TRUE;
				rc = deletelines(&buffer[1], &linelist, &currentline);
				if (rc)
					printerror(rc);
				break;
			case 'I':
				file_edited = TRUE;
				rc = insertlines(&buffer[1], &linelist, &currentline);
				if (rc)
					printerror(rc);
				break;
			case 'M':
				file_edited = TRUE;
				rc = movelines(&buffer[1], &linelist, &currentline);
				if (rc)
					printerror(rc);
				break;
			case 'W':
				if (buffer[1] != '\0')
					strcpy(filename, &buffer[1]);
				rc = writefile(filename, &linelist);
				if (rc)
					printerror(rc);
				else
					printf("%d lines written in %s\n", double_length(linelist), filename);
				file_edited = FALSE;
				break;
			case 'Q':
				if (file_edited == TRUE) {
					printf("File modified. Enter W to save, Q to discard.\n");
					file_edited = FALSE;
				}
				else
					exit_flag = TRUE;
				break;
			default:
				printerror(E_BADCMD);
				break;
		}
	}
}
