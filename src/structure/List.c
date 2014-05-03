
#include "stdafx.h"

#include "list.h"

ListNode *list_new_node(void *data) {
	ListNode *node = (ListNode *) malloc(sizeof(ListNode));
	node->next = node->previous = 0;
	node->data = data;
    return node;
}

List *list_new(void)
{
	List *list = (List *) malloc(sizeof(List));
	list_init(list);
	return list;
}

void list_init(List *list)
{
	list->previous = list->head = list->end = 0;
	list->previousindex = list->len = 0;
}

void list_free(List *list,void (*free_func)(void *))
{
	if(list) list_empty(list,free_func);
	free(list);
}

void list_empty(List *list,void (*free_func)(void *))
{
	while (list->head) {
		ListNode *remove = list->head;
		if(free_func) free_func(list->head->data);
		list->head = list->head->next;
		free(remove);
	}
	list_init(list);
}

void list_insert(List *list,void *data, unsigned int index)
{
	ListNode *newnode = list_new_node(data);

    if (!list->head) {
        list->previous = list->head = list->end = newnode;
		list->previousindex = 0;
	} else if (index >= list->len) {
        list->end->next = newnode;
		newnode->previous = list->end;
		list->previous = list->end = newnode;
		list->previousindex = list->len;
	} else if (index <= 0) {
		newnode->next = list->head;
		list->head->previous = newnode;
		list->previous = list->head = newnode;
		list->previousindex = 0;
	} else {
		ListNode *location = list->head;
		list->previousindex = index;
        
        while (index) {
            location = location->next;
            index -= 1;
		}
        
        newnode->next = location;
		newnode->previous = location->previous;
		list->previous =  location->previous = newnode->previous->next = newnode;
	}
	list->len += 1;
}

void *list_foreach(List *list, void *(*func)(void *,void *),void *user_data)
{
	ListNode *cur = list->head;

	while (cur) {
		void *value = func(cur->data,user_data);
		if(value) return value;
		cur = cur->next;
	}

	return 0;
}

void *list_get(List *list,unsigned int index) {
    if (index >= list->len) return 0;

	if(index >= list->previousindex) {
		index-=list->previousindex;
		list->previousindex += index;
	} else {
		list->previousindex = index;
		list->previous = list->head;
	}
	
    while (list->previous && index) {
        list->previous = list->previous->next;
        index -= 1;
	}
    
	return list->previous ? list->previous->data : 0;
}

int list_find(List *list,void *data)
{
	ListNode *cur = list->head;
	int index = 0;
	
	while (cur && cur->data != data) {cur = cur->next; index += 1;}
    
	return cur ? index : -1;
}

void list_sort(List *list)
{
	//TODO
}

void *list_pop(List *list, unsigned int index) {
	ListNode *cur = list->head;
	void *data = 0;
    if (index >= list->len) return 0;
	
    while (cur && index) {
        cur = cur->next;
        index -= 1;
	}
	
	if (cur) {
		data = cur->data;
		list_removePtr(list,cur);
	}
	
	return data;
}

void list_remove(List *list, void *data) {
    ListNode *cur = list->head;
    
    while (cur && cur->data != data) cur = cur->next;
	
	list_removePtr(list,cur);
}

void list_removeIndex(List *list, unsigned int index) {
    ListNode *cur = list->head;
    if (index >= list->len) return;
	
    while (cur && index) {
        cur = cur->next;
        index -= 1;
	}
    
    list_removePtr(list,cur);
}

void list_removePtr(List *list, ListNode *ptr) {
	if (!list || !list->head) return;
    
	if (ptr) {
		if(ptr->next) ptr->next->previous = ptr->previous;
		else list->end = ptr->previous;

		if(ptr->previous) ptr->previous->next = ptr->next;
		else list->head = ptr->next;

		list->len -= 1;
		list->previousindex = 0;
		list->previous = list->head;
	}
}
