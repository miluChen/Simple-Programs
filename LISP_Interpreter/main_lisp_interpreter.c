#include <stdio.h>
#include "lisp_interpreter.h"

int main() {
	/* read-eval-print loop for a "subset of lisp" interpreter */
	lisp_expression expression, value;
	int rc;

	do {
		printf("lisp: ");
		rc = read_expression(&expression);
		if (rc) {
			printerror(rc);
			continue;
		}
		rc = eval_expression(expression, &value);
		if (rc != E_EXIT) {
			if (rc) {
				printerror(rc);
				continue;
			}
			print_expression(value);
			printf("\n");
		}
	} while (rc != E_EXIT);

	printf("Normal termination.\n");
	return 0;
}
