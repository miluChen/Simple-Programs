#ifndef __C_LISTS__
#define __C_LISTS__

#include "../include/types.h"

#define DATA(L) ((L)->datapointer)
#define NEXT(L) ((L)->next)
#define PREV(L) ((L)->prev)

typedef struct node node, *list;
struct node {
	generic_ptr datapointer;
	list next;
};

typedef struct double_node double_node, *double_list;
struct double_node {
	generic_ptr datapointer;
	double_list prev;
	double_list next;
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

/* primitive operations for circular linked list: 
   the "list" variable used to access the data structure points to the last node */
status init_circ_list(list *p_L);
bool empty_circ_list(list L);
status circ_insert(list *p_L, generic_ptr data);
status circ_append(list *p_L, generic_ptr data);
status circ_delete(list *p_L, generic_ptr *p_data);
status circ_delete_node(list *p_L, list node);
list circ_list_iterator(list L, list lastreturn);
int circ_length(list L);
list nth_node(list L, int number);
status circ_traverse(list L, status (*p_func_f)());

/* primitive operations for doubly-linked list */
status allocate_double_node(double_list *p_L, generic_ptr data);
void free_double_node(double_list *p_L);
status init_double_list(double_list *p_L);
bool empty_double_list(double_list L);
status double_insert(double_list *p_L, generic_ptr data);
status double_delete(double_list *p_L, generic_ptr *p_data);
status double_delete_node(double_list *p_L, double_list node);
int double_length(double_list L);
double_list nth_double_node(double_list L, int number);
status double_traverse(double_list L, status (*p_func_f)());
int double_node_number(double_list L);
double_list nth_relative_double_node(double_list L, int direction);
// extract sections of a list
void cut_list(double_list *p_L, double_list *p_start, double_list *p_end);
// combine two lists
void paste_list(double_list *p_target, double_list *p_source);

#endif
