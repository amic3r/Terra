
#ifndef __included_terra_rand_h
#define __included_terra_rand_h

void TRandInitialize(size_t seed);
void TRandDestroy();

void TRandSetSeed(size_t seed);

unsigned char TRandBool(void);

int TRandInteger(int begin,int end);
size_t TRandUInteger(size_t begin,size_t end);
#define TRandRangeInteger(r) TRandInteger(0,r)

void TRandUniqueIntegersArray(int offset,size_t range,int *intarray,size_t size);

size_t TRandPickOne(size_t start, size_t end, size_t reject);

double TRandDouble(double begin,double end);

int TRandNormal(size_t mean, size_t range, size_t clip);

#endif
