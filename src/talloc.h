
#ifndef __included_terra_alloc_h
#define __included_terra_alloc_h

#include <stdlib.h>
#include "tdefine.h"
#include "debugging/tlog.h"

static inline void *TAlloc(size_t size)
{
	void *d = malloc(size);
	if(!d) TLogReport(T_LOG_WARNING,"TAlloc","Out of memory.");
	return d;
}

static inline int TRAlloc(void **ptr, size_t size)
{
	void *d = realloc(*ptr,size);
	if(!d) TLogReport(T_LOG_WARNING,"TRAlloc","Out of memory.");
	else *ptr = d;
	return d != 0;
}

#endif
