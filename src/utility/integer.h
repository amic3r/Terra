
#ifndef __included_terra_integer_h
#define __included_terra_integer_h

_inline unsigned int upper_power_of_two(unsigned int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}


_inline unsigned char is_power_of_two(unsigned int v)
{
	return v && !(v & (v - 1));
}

_inline int *integer_to_ptr(int data)
{
	int *i = (int *) malloc(sizeof(int));
	AppAssert(i);
	*i = data;
	return i;
}

_inline size_t *uinteger_to_ptr(size_t data)
{
	size_t *i = (size_t *) malloc(sizeof(size_t));
	AppAssert(i);
	*i = data;
	return i;
}

gint unsigned_integer_compare_glib (gconstpointer a,gconstpointer b, gpointer c);

#endif
