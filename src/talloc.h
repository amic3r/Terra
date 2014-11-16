
#ifndef __included_terra_alloc_h
#define __included_terra_alloc_h

void *TAlloc(size_t size);
void *TRAlloc(void *ptr, size_t size);
void TDeAlloc(void *ptr);

void TAllocSet(void *(*alloc)(size_t), void *(*ralloc)(void *, size_t),void (*dalloc) (void *));

#endif
