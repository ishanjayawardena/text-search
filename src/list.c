/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * list.c
 ******************************************************************************/
#include "list.h"

static unsigned int node_count;

LIST add_to_list(LIST *listp, LIST newnode)
{
	LIST p;
	if (newnode == NULL) {
		fprintf(stderr, "Cannot insert node to list.\n");
		return NULL;
	} else {
		p = *listp;
		*listp = newnode;
		newnode->next = p;
		++node_count;
	}
	return *listp;
	
}

LIST make_newnode(char *str)
{	
	LIST newnode = (LIST)malloc(sizeof(LIST_ENTRY));
	if (newnode == NULL) {
		fprintf(stderr, "Could not allocate memory for new node.\n");
	} else {
		newnode->path = (char *)malloc(strlen(str) + 1);
		strcpy(newnode->path, str);
		newnode->next = NULL;
	}
	return newnode;
}


	
void clear_list(LIST *listp) /* we use a pointer to LIST because we need to change its*/
{
	if (*listp != NULL) {
		clear_list(&((*listp)->next));
		free(*listp);
		*listp = NULL;
	}
}

void print_list(LIST node)
{
	if (node != NULL) {
		printf("node: %s:%d\n", node->path, (int)strlen(node->path));
		print_list((node->next));
	}
}

void make_list(LIST *listp)
{
	*listp = NULL;
	node_count = 0;
}

unsigned int get_listsize(LIST list)
{
	if (list != NULL)
		return node_count;
	else 
		return -1;
}

STRING * make_array(LIST list)
{
	unsigned int n = get_listsize(list);
	unsigned int i;
	STRING *sp;
	if (list == NULL || n <= 0) {
		printnexit("Cannot make an array. List is empty.");
	} else {
		sp = (STRING *)calloc(n, sizeof(STRING));
		if (sp == NULL) {
			printnexit("No enough memory for array.");
		} else {
			for (i = 0; i < n && list != NULL; ++i) {
				sp[i] = strcpy(malloc(strlen(list->path) + 1), list->path);
				list = list->next;
					
			}
		}
		
	}
	return sp;
}

