
#ifndef __included_terra_heap_h
#define __included_terra_heap_h

#include "tdefine.h"

typedef struct _THeap THeap;

#define THEAP_MAX_PRIORITY 0
#define THEAP_MIN_PRIORITY 1

THeap *THeapNew(int type);
void THeapFree(THeap *h, TFreeFunc func);

void THeapEmpty(THeap *h, TFreeFunc func);

void THeapPush(THeap *h, int priority, void *data);
void *THeapPop(THeap *h);

size_t THeapNumElements(THeap *h);

#ifdef _DEBUG
void THeapPrint(THeap *h, TIterFunc func);
#endif

#endif
