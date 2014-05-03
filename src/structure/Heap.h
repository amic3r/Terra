
#ifndef __included_terra_heap_h
#define __included_terra_heap_h

typedef struct _Heap Heap;

#define MAX_PRIORITY 0
#define MIN_PRIORITY 1

Heap *heap_new(int type);
void heap_free(Heap *h, void (*data_free)(void *));

void heap_empty(Heap *h, void (*data_free)(void *));

void heap_push(Heap *h, int priority, void *data);
void *heap_pop(Heap *h);

size_t heap_num_elements(Heap *h);

#ifdef _DEBUG
void heap_print(Heap *h, void (*data_print) (void *data));
#endif

#endif
