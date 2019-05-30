#include "list.h"

List initList(V id, char* name)
{
	List list;
	list = (List) malloc(sizeof(struct list));
	list->id = id;
	list->name = strdup(name);
	list->next = NULL;
	list->prev = NULL;
	return list;
}

List addLast(List list, V id,  char *name)
{
	List new = initList(id, name);
	if (list == NULL)
		return new;
	List temp = list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
	return list;
}

List deleteItem(List l, V id) 
{
	if(l == NULL) {
		return NULL;
	} else {
		List tmp = l, prev;
		if(tmp->id == id)
		{
			l = l->next;
			free(tmp->name);
			free(tmp);
			l->prev = NULL;
			return l;
		} else {
			prev = tmp;
			tmp = tmp->next;
		}
		while(tmp != NULL)
		 {
			if(tmp->id == id)
			{
				prev->next = tmp->next;
				if (tmp->next != NULL)
					tmp->next->prev = prev;
				free(tmp->name);
				free(tmp);
				return l;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		return l;
	}
}

List freeList(List list)
{
	List tmp;
	if (list == NULL)
		return list;
	while (list != NULL) {
		tmp = list;
		list = list->next;
		free(tmp->name);
		free(tmp);
	}
	return NULL;
}

void printList(List list)
{
	while(list != NULL)
	{
		printf("%sid = %d\n", list->name, list->id);
		list = list->next;
	}
	printf("\n");
}
