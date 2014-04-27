
#ifndef __included_terra_list_h
#define __included_terra_list_h

/** 
  * Doubly-Linked List
  */

typedef struct _ListNode {
	struct _ListNode *next;
	struct _ListNode *previous;
	void *data;
} ListNode;


typedef struct _List {
	ListNode *head;
	ListNode *end;
	unsigned int len;

	ListNode *previous;
	unsigned int previousindex;
} List;

List *list_new(void);

void list_init(List *list);

void list_free(List *list,void (*free_func)(void *));

void list_empty(List *list,void (*free_func)(void *));

void list_insert(List *list,void *data, unsigned int index);

__inline void list_prepend(List *list,void *data)
{
	list_insert(list,data,0);
}

__inline void list_append(List *list,void *data)
{
	list_insert(list,data,list->len);
}

__inline void list_push(List *list,void *data)
{
	list_prepend(list,data);
}

void *list_foreach(List *list, void *(*func)(void *,void *),void *user_data);
void *list_get(List *list,unsigned int index);
int list_find(List *list,void *data);

void list_sort(List *list);

void *list_pop(List *list, unsigned int index);
void list_remove(List *list, void *data);
void list_removePtr(List *list, ListNode *ptr);
void list_removeIndex(List *list, unsigned int index);

#endif
