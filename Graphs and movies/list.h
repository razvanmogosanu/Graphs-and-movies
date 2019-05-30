#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>	
#include <string.h>

typedef int V;

typedef struct list {
	V id;
	char *name;
	struct list *prev, *next;
}*List;

List initList(V id, char* name);
List addFirst(List list, V data);
List addLast(List list, V id, char *name);
List deleteItem(List list, V data);
List freeList(List list);
void printList(List list);

#endif