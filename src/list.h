/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * list.h
 ******************************************************************************/
#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

typedef struct list {
    char *path;
    struct list *next;
} LIST_ENTRY;
typedef LIST_ENTRY *LIST;

extern LIST add_to_list(LIST *, LIST);
extern LIST make_newnode(char *);
extern void clear_list(LIST *);
extern void print_list(LIST);
extern void make_list(LIST *);
extern unsigned int get_listsize(LIST);
extern STRING *make_array(LIST);

#endif /* LIST_H_ */
