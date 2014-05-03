
#include "stdafx.h"

#include "heap.h"

#include "Array.h"

#define DCMP(a,b) (a->priority >= b->priority)
#define CMP(a,b) (a->priority <= b->priority)

typedef struct {
	int priority;
	void *data;
} HeapNode;

HeapNode *heapnode_new(int priority, void *data)
{
	HeapNode *node = (HeapNode *) malloc(sizeof(HeapNode));
	node->priority = priority;
	node->data = data;
	return node;
}

void heapnode_free(HeapNode *node, void (*data_free)(void *))
{
	if(node) {
		if(data_free) data_free(node->data);
		free(node);
	}
}

struct _Heap {
	Array data;
	size_t count;
	int type;
};

Heap *heap_new(int type)
{
	Heap *h = (Heap *) malloc(sizeof(Heap));

	array_init(&h->data,0);
	h->count = 0;
	h->type = type;

	return h;
}

void heap_free(Heap *h, void (*data_free)(void *))
{
	if(h) {
		heap_empty(h,data_free);
		free(h);
	}
}

void heap_empty(Heap *h, void (*data_free)(void *))
{
	array_empty(&h->data,data_free);
	h->count = 0;
}

void heap_push(Heap *h, int priority, void *data)
{
	HeapNode *node = heapnode_new(priority,data), *p;
	unsigned int index,parent;

	for(index = h->count++; index; index = parent)
	{
		parent = (index - 1) >> 1;
		p = (HeapNode *) array_get(&h->data,parent);
		if (h->type ? CMP(p,node) : DCMP(p,node)) break;
		array_insert(&h->data,p,index);
	}
	array_insert(&h->data,node,index);
}

void *heap_pop(Heap *h)
{
	unsigned int index = 0, swap, other;
	
	HeapNode *temp = (HeapNode *) array_get(&h->data,0), *s, *o;
	void *data = temp->data;

	heapnode_free(temp,0);
 
	// Reorder the elements
	while(1) {
		// Find the child to swap with
		swap = (index << 1) + 1;
		if (swap >= h->count) break; // If there are no children, the heap is reordered
		s = (HeapNode *) array_get(&h->data,swap);

		other = swap + 1;
		if (other < h->count) {
			o = (HeapNode *) array_get(&h->data,other);
			if (h->type ? CMP(o,s) : DCMP(o,s)) {
				swap = other;
				s = o;
			}
		}
 
		array_insert(&h->data,s,index);
		index = swap;
	}
	array_remove(&h->data,index);

	h->count--;

	return data;
}

size_t heap_num_elements(Heap *h)
{
	return h->count;
}

void heap_print(Heap *h, void (*data_print) (void *data))
{
	size_t i = 0, limit = h->data.lastindex;
	HeapNode *data;

	printf("Heap content: ");

	for(; i < limit; ++i) {
		data = (HeapNode *) h->data.data[i];
		data_print(data->data);
		printf(" ");
	}

	printf("\n");
}
