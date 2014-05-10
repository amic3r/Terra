
#include "stdafx.h"

#include "tinteger.h"

#include "talloc.h"

int *TIntegerToPtr(int data)
{
	int *i = (int *) TAlloc(sizeof(int));
	if(!i) return 0;
	*i = data;
	return i;
}

size_t *TIntegerToPtrU(size_t data)
{
	size_t *i = (size_t *) TAlloc(sizeof(size_t));
	if(!i) return 0;
	*i = data;
	return i;
}

gint TIntegerCompareGlibU (gconstpointer a,gconstpointer b, gpointer c)
{
	const unsigned int *x = (const unsigned int *) a;
	const unsigned int *y = (const unsigned int *) b;

	if(*x < *y) return -1;
	else if (*x > *y) return 1;

	return 0;
}
