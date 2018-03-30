#ifndef __C_LISP_STRUCT__
#define __C_LISP_STRUCT__

#include "../include/types.h"
#include "../Linked_List/lists.h"

#define E_EXIT -1
#define E_EOF 1
#define E_SPACE 2
#define E_SYNTAX 3
#define E_EVAL 4
#define MAXERROR 5

#define LISP_TYPE(x) (((lisp_expression)x)->type)
#define ATOM_VALUE(x) (((lisp_expression)x)->value.atom)
#define LIST_VALUE(x) (((lisp_expression)x)->value.lisplist)

#define T_VALUE "T"
#define NIL_VALUE "NIL"
#define QUOTE_VALUE "QUOTE"

typedef enum {LIST, ATOM} lisp_type;
typedef list lisp_list;
typedef char *lisp_atom;

typedef struct lisp_node {
	lisp_type type;
	union {
		lisp_list lisplist;
		lisp_atom atom;
	} value;
} lisp_node, *lisp_expression;

// low level subroutines
lisp_expression new_expression();
lisp_atom new_atom(char *s);
int equal_atom(lisp_atom atom1, lisp_atom atom2);

#endif
