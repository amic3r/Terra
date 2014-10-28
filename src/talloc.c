#include "stdafx.h"

#include "talloc.h"

#include "debugging/tlog.h"

void *allocDef(size_t size)
{
	void *d = malloc(size);
	if(!d) TLogReport(T_LOG_WARNING,"TAlloc","Out of memory.");
	return d;
}

int rAllocDef(void **ptr, size_t size)
{
	void *d = realloc(*ptr,size);
	if(!d) TLogReport(T_LOG_WARNING,"TRAlloc","Out of memory.");
	else *ptr = d;
	return d != 0;
}

static void *(*alloc)(size_t) = allocDef;
static int (*ralloc)(void **, size_t) = rAllocDef;
static void (*dalloc)(void *) = free;

void *TAlloc(size_t size)
{
	return alloc(size);
}

int TRAlloc(void **ptr, size_t size)
{
	return ralloc(ptr,size);
}

void TDeAlloc(void *ptr)
{
	dalloc(ptr);
}

void TAllocSet(void *(*_alloc)(size_t), int (*_ralloc)(void **, size_t),void (*_dalloc) (void *))
{
	if(!_alloc) _alloc = allocDef;
	if(!_ralloc) _ralloc = rAllocDef;
	if(!_dalloc) _dalloc = free;

	alloc = _alloc;
	ralloc = _ralloc;
	dalloc = _dalloc;
}
