#include <string.h>
#include <stdlib.h>
#include "lisp_struct.h"

// low level subroutines
lisp_expression new_expression() {
	return (lisp_expression)malloc(sizeof(lisp_node));
}

lisp_atom new_atom(char *s) {
	char *dest = malloc(sizeof(char) * (strlen(s) + 1));

	if (dest)
		strcpy(dest, s);
	return dest;
}

int equal_atom(lisp_atom atom1, lisp_atom atom2) {
	return strcmp(atom1, atom2) == 0;
}
