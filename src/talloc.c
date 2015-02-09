#include "stdafx.h"

#include "talloc.h"

#include "terror.h"

void *allocDef(size_t size)
{
	void *d = malloc(size);
	if(!d) TErrorReport(T_ERROR_OUT_OF_MEMORY);
	return d;
}

void *rAllocDef(void *ptr, size_t size)
{
	void *d = realloc(ptr,size);
	if(!d) TErrorReport(T_ERROR_OUT_OF_MEMORY);
	return d;
}

static void *(*talloc)(size_t) = allocDef;
static void *(*tralloc)(void *, size_t) = rAllocDef;
static void (*tfree)(void *) = free;

void *TAlloc(size_t size)
{
	return talloc(size);
}

void *TRAlloc(void *ptr, size_t size)
{
	return tralloc(ptr,size);
}

void TFree(void *ptr)
{
	tfree(ptr);
}

void TAllocSet(void *(*_alloc)(size_t), void *(*_ralloc)(void *, size_t),void (*_free) (void *))
{
	if(!_alloc) _alloc = allocDef;
	if(!_ralloc) _ralloc = rAllocDef;
	if(!_free) _free = free;

	talloc = _alloc;
	tralloc = _ralloc;
	tfree = _free;
}
