
#include "stdafx.h"

static GRand *pseudorandomgen;
static unsigned int randseed = INT_MAX;

void rand_initialize(unsigned int seed)
{
	randseed = seed;
	pseudorandomgen = g_rand_new_with_seed(seed);
}

void rand_destroy()
{
	g_rand_free(pseudorandomgen);
}

void rand_set_seed(unsigned int seed)
{
	randseed = seed;
	g_rand_set_seed(pseudorandomgen,seed);
}

unsigned char rand_bool(void)
{
	return g_random_boolean();
}

int rand_integer(int begin,int end)
{
	return g_rand_int_range(pseudorandomgen,begin,end);
}

void unique_integers_array(int offset,unsigned int range,int *intarray,unsigned int size)
{
	if(!intarray) return;

	if(range >= size)
	{
		unsigned int *integers = (unsigned int *) malloc(sizeof(unsigned int)*range);
		unsigned int i = 0;
		for(; i < range; ++i)
			integers[i] = i;

		for (i = 0; i < size; ++i) {
			unsigned int r = rand_integer(0,range);
			unsigned int value = integers[r];
			integers[r] = integers[--range];

			intarray[i] = value + offset;
		}

		free(integers);
	}
}

unsigned int pick_one(unsigned int size, int reject)
{
	if(reject < 0)
		return rand_integer(0,size);
	else if (reject == 0)
		return rand_integer(0,size-1)+1;
	else if (reject == size-1)
		return rand_integer(0,size-1);
	else if (rand_integer(0,2))
		return rand_integer(reject+1,size);
	
	return rand_integer(0,reject);
}

double rand_double(double begin,double end)
{
	return g_rand_double_range(pseudorandomgen,begin,end);
}

int rand_normal(unsigned int mean, unsigned int range, unsigned int clip)
{
	//TODO : fix this
	// Central limit theorm - Maybe upgrade to Box-Muller later
	int counter;
	float seed = FLT_MAX;

	return mean;
	
	if(clip > range)		return 0;		// Don't be an asshole.
	while(seed > mean + range - clip || seed < mean - range + clip) {
		seed = 0.0f;

		for (counter = 0; counter < 12; ++counter)
			seed += ((float)rand() / (float)RAND_MAX);

		seed = (seed - 6.0f) * (range / 3.0f) + mean;

		seed = CLAMP(seed,(float)(mean - range),(float)(mean + range));
	}

	return (int)seed;
}
