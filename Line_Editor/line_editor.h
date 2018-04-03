#ifndef __C_LINE_EDITOR__
#define __C_LINE_EDITOR__

#include "../include/types.h"
#include "../Linked_List/lists.h"

#define E_IO 1
#define E_SPACE 2
#define E_LINES 3
#define E_BADCMD 4
#define E_DELETE 5
#define E_MOVE 6
#define MAXERROR 7

#define BUFFSIZE 100

void printerror(int errnum);
int readfile(char *filename, double_list *p_head);
int writefile(char *filename, double_list *p_head);
// the following functions need to update the p_head and p_current as necessary
int printlines(char *linespec, double_list *p_head, double_list *p_current);
int deletelines(char *linespec, double_list *p_head, double_list *p_current);
int insertlines(char *linespec, double_list *p_head, double_list *p_current);
int movelines(char *linespec, double_list *p_head, double_list *p_current);

#endif
