#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "polynomial.h"

status term_insert(list *p_L, int coef, int deg);
status term_delete(list *p_L, int *p_coef, int *p_deg);
status write_term(term *p_term);
int cmp_degree(term *p_term1, term *p_term2);

status read_poly(polynomial *p_poly) {
	int coef;
	int degree;

	if (init_list(p_poly) == ERROR)
		return ERROR;
	do {
		printf("\tEnter coefficient, degree (0, 0 when done): ");
		scanf(" %d,%d", &coef, &degree);
		if (coef != 0)
			if (term_insert(p_poly, coef, degree) == ERROR)
				return ERROR;
	} while (coef != 0 || degree != 0);
	return OK;
}

void write_poly(polynomial poly) {
	traverse(poly, write_term);
}

status write_term(term *p_term) {
	printf(" + %dx^%d", p_term->coefficient, p_term->degree);
	return OK;
}

status add_poly(polynomial *p_poly1, polynomial *p_poly2) {
	/* perform poly1 += poly2; poly2 = 0 */
	list lastreturn = NULL;
	list match_node;
	term *p_term, *p_match_term;
	int coef, degree;

	while ((lastreturn = list_iterator(*p_poly1, lastreturn)) != NULL) {
		p_term = (term *)DATA(lastreturn);
		if (find_key(*p_poly2, (generic_ptr)p_term, cmp_degree, &match_node) == OK) {
			p_match_term = (term *)DATA(match_node);
			p_term->coefficient += p_match_term->coefficient;
			delete_node(p_poly2, match_node);
			free(p_match_term); // delete the data
		}
	}
	// add what's left in poly2 into poly1
	while (empty_list(*p_poly2) == FALSE) {
		if (term_delete(p_poly2, &coef, &degree) == ERROR)
			return ERROR;
		else if (term_insert(p_poly1, coef, degree) == ERROR)
			return ERROR;
	}
	return OK;
}

int cmp_degree(term *p_term1, term *p_term2) {
	return p_term1->degree - p_term2->degree;
}

void destroy_poly(polynomial *p_poly) {
	destroy(p_poly, free);
}

void sort_poly(polynomial *p_poly) {
	sortList(p_poly, cmp_degree);
}

status term_insert(list *p_L, int coef, int deg) {
	term *p_term = (term *)malloc(sizeof(term));

	if (p_term == NULL)
		return ERROR;
	p_term->coefficient = coef;
	p_term->degree = deg;
	if (insert(p_L, (generic_ptr)p_term) == ERROR) {
		free(p_term);
		return ERROR;
	}
	return OK;
}

status term_delete(list *p_L, int *p_coef, int *p_deg) {
	/* delete a node from p_L and return the data stored in the node in p_coef and p_deg */
	term *p_term;

	if (delete(p_L, (generic_ptr *)&p_term) == ERROR)
		return ERROR;
	*p_coef = p_term->coefficient;
	*p_deg = p_term->degree;
	free(p_term); // delete the data
	return OK;
}
