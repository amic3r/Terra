#include "stdafx.h"

#include "talloc.h"

#include "terror.h"

TPtr allocDef(TUInt64 size)
{
	TPtr d = malloc((size_t) size);
	if(!d) TErrorReport(T_ERROR_OUT_OF_MEMORY);
	return d;
}

TPtr rAllocDef(TPtr ptr, TUInt64 size)
{
	TPtr d = realloc(ptr, (size_t) size);
	if(!d) TErrorReport(T_ERROR_OUT_OF_MEMORY);
	return d;
}

static TPtr (*talloc)(TUInt64) = allocDef;
static TPtr (*tralloc)(TPtr , TUInt64) = rAllocDef;
static void (*tfree)(TPtr ) = free;

TPtr TAlloc(TUInt64 size)
{
	return talloc(size);
}

TPtr TRAlloc(TPtr ptr, TUInt64 size)
{
	return tralloc(ptr,size);
}

void TFree(TPtr ptr)
{
	tfree(ptr);
}

void TAllocSet(TPtr (*_alloc)(TUInt64), TPtr (*_ralloc)(TPtr , TUInt64),void (*_free) (TPtr ))
{
	if(!_alloc) _alloc = allocDef;
	if(!_ralloc) _ralloc = rAllocDef;
	if(!_free) _free = free;

	talloc = _alloc;
	tralloc = _ralloc;
	tfree = _free;
}
