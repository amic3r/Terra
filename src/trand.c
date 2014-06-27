
#include "stdafx.h"

#include "trand.h"

#include <float.h>

static size_t tRandSeed = INT_MAX;

void TRandInitialize(size_t seed)
{
	TRandSetSeed(seed);
}

void TRandDestroy()
{
}

void TRandSetSeed(size_t seed)
{
	srand(seed);
	tRandSeed = seed;
}

unsigned char TRandBool(void)
{
	return (TRandUInteger(0,UINT_MAX) & (1 << 15)) != 0;
}

int TRandInteger(int begin,int end)
{
	return (int)(((double)rand() / RAND_MAX) * (end - begin) + begin);
}

size_t TRandUInteger(size_t begin,size_t end)
{
	return (size_t)(((double)rand() / RAND_MAX) * (end - begin) + begin);
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
	return (((double)rand() / RAND_MAX) * (end - begin) + begin);
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

		seed = clamp(seed,(float)(mean - range),(float)(mean + range));
	}

	return (int)seed;
}
