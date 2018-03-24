#include <stdio.h>
#include <stdlib.h>
#include "lists.h"
#include "polynomial.h"

int main() {
	/* read two polynomial, add them and print the result */
	polynomial poly1, poly2;

	printf("Enter the first polynomial:\n");
	if (read_poly(&poly1) == ERROR) {
		printf("error in reading polynomial\n");
		exit(1);
	}
	printf("Enter the second polynomial:\n");
	if (read_poly(&poly2) == ERROR) {
		printf("error in reading polynomial\n");
		exit(1);
	}
	printf (" ");
	write_poly(poly1);
	printf("\n+ ");
	write_poly(poly2);
	if (add_poly(&poly1, &poly2) == ERROR) {
		printf("error adding polynomials\n");
		exit(1);
	}
	printf("\n= ");
	sort_poly(&poly1);
	write_poly(poly1);
	printf("\n");
	destroy_poly(&poly1);
	destroy_poly(&poly2);

	return 0;
}
