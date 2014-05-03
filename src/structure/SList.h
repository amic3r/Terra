
#ifndef __included_terra_slist_h
#define __included_terra_slist_h

/** 
  * Single-Linked List
  */

typedef struct _SListNode {
	struct _SListNode *next;
	void *data;
} SListNode;


typedef struct _SList {
	SListNode *head;
	SListNode *end;
	unsigned int len;

	SListNode *previous;
	unsigned int previousindex;
} SList;

SList *slist_new(void);

void slist_init(SList *list);

void slist_free(SList *list,TFreeFunc func);

void slist_empty(SList *list,TFreeFunc func);

void slist_insert(SList *list,void *data, unsigned int index);

__inline void slist_prepend(SList *list,void *data)
{
	slist_insert(list,data,0);
}

__inline void slist_append(SList *list,void *data)
{
	slist_insert(list,data,list->len);
}

#define slist_push(l,d) slist_prepend(l,d)

void slist_replace(SList *list,void *data, unsigned int index);

void slist_concat(SList *list,const SList *list2);

void *slist_foreach(const SList *list,TIterFunc func,void *user_data);
void *slist_get(SList *list,unsigned int index);
__inline void *slist_first(SList *list) { return slist_get(list, 0); }
void *slist_next(SList *list);

void slist_sort(SList *list,TCompareFunc func);

void *slist_pop(SList *list, unsigned int index);

void slist_remove(SList *list, void *data);
void slist_removePtr(SList *list, SListNode *ptr);
void slist_removeIndex(SList *list, unsigned int index);
void slist_removePtrFrom(SList *list, SListNode *origin);

#endif
