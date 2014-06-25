
#include "stdafx.h"

#include "trand.h"

static GRand *tPseudoTandomGen;
static size_t tRandSeed = INT_MAX;

void TRandInitialize(size_t seed)
{
	tRandSeed = seed;
	tPseudoTandomGen = g_rand_new_with_seed(seed);
}

void TRandDestroy()
{
	g_rand_free(tPseudoTandomGen);
}

void TRandSetSeed(size_t seed)
{
	tRandSeed = seed;
	g_rand_set_seed(tPseudoTandomGen,seed);
}

unsigned char TRandBool(void)
{
	return g_random_boolean();
}

int TRandInteger(int begin,int end)
{
	return g_rand_int_range(tPseudoTandomGen,begin,end);
}

void TRandUniqueIntegersArray(int offset,size_t range,int *intarray,size_t size)
{
	if(!intarray) return;

	if(range >= size)
	{
		size_t *integers = (size_t *) malloc(sizeof(size_t)*range);
		size_t i = 0;
		for(; i < range; ++i) integers[i] = i;

		for (i = 0; i < size; ++i) {
			size_t r = TRandInteger(0,range);
			size_t value = integers[r];
			integers[r] = integers[--range];

			intarray[i] = value + offset;
		}

		free(integers);
	}
}

size_t TRandPickOne(size_t start, size_t end, size_t reject)
{
	if (reject == start)		start++;
	else if (reject == end-1)	end--;
	else if (TRandInteger(0,2)) start = reject+1;
	else end = reject;
	
	return TRandInteger(start,end);
}

double TRandDouble(double begin,double end)
{
	return g_rand_double_range(tPseudoTandomGen,begin,end);
}

int TRandNormal(size_t mean, size_t range, size_t clip)
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
