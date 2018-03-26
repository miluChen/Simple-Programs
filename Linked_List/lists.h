#ifndef __C_LISTS__
#define __C_LISTS__

#include "../include/types.h"

#define DATA(L) ((L)->datapointer)
#define NEXT(L) ((L)->next)

typedef struct node node, *list;
struct node {
	generic_ptr datapointer;
	list next;
};

/* primitive operations on Linked Lists */
status init_list(list *p_L);
bool empty_list(list L);
// insert an item at the front
status insert(list *p_L, generic_ptr data);
// append an item to the end
status append(list *p_L, generic_ptr data);
// delete the first node and return the DATA in p_data
status delete(list *p_L, generic_ptr *p_data);
// detele a node
status delete_node(list *p_L, list node);
// an application-defined function is called with the data stored in each node
status traverse(list L, status (*p_func_f) ());
// locate a node based on a key
status find_key(list L, generic_ptr key, int (*p_cmp_f)(), list *p_keynode);
// list iterator: return each item of list in turn
list list_iterator(list L, list lastreturn);
// destroy an entire list
status destroy(list *p_L, void (*p_func_f)());
// an interface for allocating nodes
status allocate_node(list *p_L, generic_ptr data);
// free the node
void free_node(list *p_L);
// sort the list
void sortList(list *p_L, int (*p_cmp_f)());

#endif
