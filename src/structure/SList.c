
#include "stdafx.h"

#include "SList.h"

SListNode *slist_new_node(void *data) {
	SListNode *node = (SListNode *) malloc(sizeof(SListNode));
	node->next = 0;
	node->data = data;
    return node;
}

SList *slist_new(void)
{
	SList *list = (SList *) malloc(sizeof(SList));
	slist_init(list);
	return list;
}

void slist_init(SList *list)
{
	list->head = list->end = 0;
	list->len = list->previousindex = 0;
	list->previous = 0;
}

void slist_free(SList *list,TFreeFunc func)
{
	slist_empty(list,func);
	free(list);
}

void slist_empty(SList *list,TFreeFunc func)
{
	while (list->head) {
		SListNode *remove = list->head;
		if(func) func(list->head->data);
		list->head = list->head->next;
		free(remove);
	}
	slist_init(list);
}

int slist_go(SList *list,unsigned int index)
{
    if (index >= list->len) return 1;
	
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
    
	return 0;
}

void slist_insert(SList *list,void *data, unsigned int index)
{
	SListNode *newnode = slist_new_node(data);

    if (!list->head) {
		list->previous = list->head = list->end = newnode;
		list->previousindex = 0;
	} else if (index >= list->len) {
        list->previous = list->end = list->end->next = newnode;
		list->previousindex = list->len;
	} else if (index <= 0) {
		list->previous = list->head = newnode->next = list->head;
		list->previousindex = 0;
	} else {
		slist_go(list,index-1);

		newnode->next = list->previous->next;
		list->previous = list->previous->next = newnode;
		list->previousindex +=1;
	}
	list->len += 1;
}

void slist_replace(SList *list,void *data, unsigned int index)
{
	if(slist_go(list,index)) return;

	list->previous->data = data;
}

unsigned char ptr_compare(void *d1,void *d2) {
	return d1 < d2;
}

void slist_concat(SList *list,const SList *list2)
{
	SListNode *cur = list2->head;

	while (cur) {
		slist_append(list,cur->data);
		cur = cur->next;
	}
}

void *slist_foreach(const SList *list, TIterFunc func, void *user_data)
{
	SListNode *cur = list->head;

	while (cur) {
		void *value = func(cur->data,user_data);
		if(value) return value;
		cur = cur->next;
	}

	return 0;
}

void *slist_get(SList *list,unsigned int index)
{
	if(slist_go(list,index)) return 0;

	return list->previous ? list->previous->data : 0;
}

void *slist_next(SList *list)
{
	return slist_get(list,list->previousindex+1);
}

__inline void mergeinsert(SList *mlist,SList *sublist)
{
	if(mlist->previous) {
		mlist->previous = mlist->end = mlist->previous->next = sublist->head;
		mlist->previousindex += 1;
	} else {
		mlist->previous = mlist->end = mlist->head = sublist->head;
	}
	mlist->len += 1;

	sublist->head = sublist->head->next;
	sublist->len -= 1;
}

void slistmerge(SList *mlist, SList *left, SList *right,TCompareFunc func)
{
	while(left->len || right->len) {
		if (left->len && right->len)
			mergeinsert(mlist,func(left->head->data,right->head->data) ? right : left);
		else if (left->len)
			mergeinsert(mlist,left);
		else
			mergeinsert(mlist,right);
	}
}

void slist_sort(SList *list,TCompareFunc func)
{
	SList left,right;
	size_t i,middle;

	if(!list || list->len <= 1) return;

	slist_init(&left);
	slist_init(&right);
	i = 0;
	middle = list->len/2;

	slist_go(list,middle-1);

	//divide the list in two
	left.head = list->head;
	left.end = list->previous;
	left.len = middle;
	left.previous = left.head;

	right.head = list->previous->next;
	right.end = list->end;
	right.len = middle;
	right.previous = right.head;

	left.end->next = 0;
	slist_init(list);

	slist_sort(&left,func);
	slist_sort(&right,func);

	slistmerge(list,&left,&right,func);
}

void *slist_pop(SList *list, unsigned int index) {
	SListNode *cur = list->head, *origin = 0;
	void *data = 0;
    if (index >= list->len) return 0;
	
    while (cur && index) {
		origin = cur;
        cur = cur->next;
        index -= 1;
	}
	
	if (cur) {
		data = cur->data;
		slist_removePtrFrom(list,origin);
	}
	
	return data;
}

void slist_remove(SList *list, void *data) {
    SListNode *cur = list->head, *origin = 0;
    
    while (cur && cur->data != data) {
		origin = cur;
        cur = cur->next;
	}
	
	if (cur) slist_removePtrFrom(list,origin);
}

void slist_removePtr(SList *list, SListNode *ptr) {
    SListNode *cur = list->head, *origin = 0;
    
    while (cur && cur != ptr) {
		origin = cur;
        cur = cur->next;
	}
    
    if (cur) slist_removePtrFrom(list,origin);
}

void slist_removeIndex(SList *list, unsigned int index) {
    SListNode *cur = list->head, *origin = 0;
	void *data = 0;
    if (index >= list->len) return;
	
    while (cur && index) {
		origin = cur;
        cur = cur->next;
        index -= 1;
	}
    
    if (cur) slist_removePtrFrom(list,origin);
}

void slist_removePtrFrom(SList *list, SListNode *origin) {
    if (!origin) {
        if (list->end == list->head) list->end = 0;
        list->head = list->head->next;
        list->len -= 1;
		list->previousindex = 0;
		list->previous = list->head;
        return;
	}

	AppAssert(origin->next);
    
	if (list->end == origin->next) list->end = origin;
    origin->next = origin->next->next;
	list->len -= 1;
	list->previousindex = 0;
	list->previous = list->head;
}
