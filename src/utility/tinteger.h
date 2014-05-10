
#ifndef __included_terra_integer_h
#define __included_terra_integer_h

static inline size_t TIntegerUpperPowerOfTwo(size_t v)
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


inline unsigned char TIntegerIsPowerOfTwo(unsigned int v)
{
	return v && !(v & (v - 1));
}

int *TIntegerToPtr(int data);
size_t *TIntegerToPtrU(size_t data);

gint TIntegerCompareGlibU (gconstpointer a,gconstpointer b, gpointer c);

#endif
