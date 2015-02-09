
#ifndef __included_terra_alloc_h
#define __included_terra_alloc_h

void *TAlloc(size_t size);
void *TRAlloc(void *ptr, size_t size);
void TFree(void *ptr);

void TAllocSet(void *(*_alloc)(size_t), void *(*_ralloc)(void *, size_t),void (*_free) (void *));

#endif
