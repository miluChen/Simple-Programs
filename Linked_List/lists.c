#include <stddef.h>
#include <stdlib.h>
#include "lists.h"

list mergeSort(list head, int (*p_cmp_f)());

status init_list(list *p_L) {
	*p_L = NULL;
	return OK;
}

bool empty_list(list L) {
	return (L == NULL) ? TRUE : FALSE;
}

status insert(list *p_L, generic_ptr data) {
	list L;

	if (allocate_node(&L, data) == ERROR)
		return ERROR;
	NEXT(L) = *p_L;
	*p_L = L;
	return OK;
}

status append(list *p_L, generic_ptr data) {
	list L, tmplist;

	if (allocate_node(&L, data) == ERROR)
		return ERROR;
	if (empty_list(*p_L) == TRUE)
		*p_L = L;
	else {
		for (tmplist = *p_L; NEXT(tmplist) != NULL; tmplist = NEXT(tmplist))
			;
		NEXT(tmplist) = L;
	}
	return OK;
}

status delete(list *p_L, generic_ptr *p_data) {
	if (empty_list(*p_L))
		return ERROR;

	*p_data = DATA(*p_L);
	return delete_node(p_L, *p_L);
}

status delete_node(list *p_L, list node) {
	if (empty_list(*p_L) == TRUE)
		return ERROR;
	if (*p_L == node)
		*p_L = NEXT(*p_L);
	else {
		list L;
		for (L = *p_L; L != NULL && NEXT(L) != node; L = NEXT(L))
			;
		if (L == NULL)
			return ERROR;
		NEXT(L) = NEXT(node);
	}
	free_node(&node);
	return OK;
}

status allocate_node(list *p_L, generic_ptr data) {
	list L = (list)malloc(sizeof(node));
	if (L == NULL)
		return ERROR;
	*p_L = L;
	DATA(L) = data;
	NEXT(L) = NULL;
	return OK;
}

void free_node(list *p_L) {
	free(*p_L);
	*p_L = NULL;
}

status traverse(list L, status (*p_func_f)()) {
	if (empty_list(L))
		return OK;
	if ((*p_func_f)(DATA(L)) == ERROR)
		return ERROR;
	else
		return traverse(NEXT(L), p_func_f);
}

status find_key(list L, generic_ptr key, int (*p_cmp_f)(), list *p_keynode) {
	list curr = NULL;
	while ((curr = list_iterator(L, curr)) != NULL) {
		if ((*p_cmp_f)(key, DATA(curr)) == 0) {
			*p_keynode = curr;
			return OK;
		}
	}
	return ERROR;
}

list list_iterator(list L, list lastreturn) {
	return (lastreturn == NULL) ? L : NEXT(lastreturn);
}

status destroy(list *p_L, void (*p_func_f)()) {
	if (empty_list(*p_L) == FALSE) {
		destroy(&NEXT(*p_L), p_func_f);
		if (p_func_f != NULL)
			(*p_func_f)(DATA(*p_L));
		free_node(p_L);
	}
}

list mergeSort(list head, int (*p_cmp_f)()) {
	if (head == NULL || NEXT(head) == NULL)
		return head;
	list slow = head, fast = NEXT(head);
	while (fast && NEXT(fast)) {
		slow = NEXT(slow);
		fast = NEXT(NEXT(fast));
	}
	list node2 = NEXT(slow);
	NEXT(slow) = NULL;
	list node1 = mergeSort(head, p_cmp_f);
	node2 = mergeSort(node2, p_cmp_f);
	// the merge step
	node dummy = {NULL, NULL};
	list tail = &dummy;
	while (node1 && node2) {
		if (p_cmp_f(DATA(node1), DATA(node2)) < 0) {
			NEXT(tail) = node1;
			node1 = NEXT(node1);
		}
		else {
			NEXT(tail) = node2;
			node2 = NEXT(node2);
		}
		tail = NEXT(tail);
	}
	if (node1)
		NEXT(tail) = node1;
	if (node2)
		NEXT(tail) = node2;
	return dummy.next;
}

void sortList(list *p_L, int (*p_cmp_f)()) {
	// merge sort
	*p_L = mergeSort(*p_L, p_cmp_f);
}
