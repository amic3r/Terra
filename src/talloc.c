#include "stdafx.h"

#include "talloc.h"

#include "debugging/tlog.h"

void *TAlloc(size_t size)
{
	void *d = malloc(size);
	if(!d) TLogReport(T_LOG_WARNING,"TAlloc","Out of memory.");
	return d;
}

int TRAlloc(void **ptr, size_t size)
{
	void *d = realloc(*ptr,size);
	if(!d) TLogReport(T_LOG_WARNING,"TRAlloc","Out of memory.");
	else *ptr = d;
	return d != 0;
}
