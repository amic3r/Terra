
#include "stdafx.h"

#include "integer.h"

gint unsigned_integer_compare_glib (gconstpointer a,gconstpointer b, gpointer c)
{
	const unsigned int *x = (const unsigned int *) a;
	const unsigned int *y = (const unsigned int *) b;

	if(*x < *y) return -1;
	else if (*x > *y) return 1;

	return 0;
}
