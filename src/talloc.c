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

static void *(*alloc)(size_t) = allocDef;
static void *(*ralloc)(void *, size_t) = rAllocDef;
static void (*dalloc)(void *) = free;

void *TAlloc(size_t size)
{
	return alloc(size);
}

void *TRAlloc(void *ptr, size_t size)
{
	return ralloc(ptr,size);
}

void TDeAlloc(void *ptr)
{
	dalloc(ptr);
}

void TAllocSet(void *(*_alloc)(size_t), void *(*_ralloc)(void *, size_t),void (*_dalloc) (void *))
{
	if(!_alloc) _alloc = allocDef;
	if(!_ralloc) _ralloc = rAllocDef;
	if(!_dalloc) _dalloc = free;

	alloc = _alloc;
	ralloc = _ralloc;
	dalloc = _dalloc;
}
