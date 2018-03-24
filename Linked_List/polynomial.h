#ifndef __C_POLYNOMIAL__
#define __C_POLYNOMIAL__

#include "lists.h"

typedef list polynomial;

typedef struct term {
	int coefficient;
	int degree;
} term;

status read_poly(polynomial *p_poly);
void write_poly(polynomial poly);
status add_poly(polynomial *p_poly1, polynomial *p_poly2);
void destroy_poly(polynomial *p_poly);
void sort_poly(polynomial *p_poly);

#endif
