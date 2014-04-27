
#ifndef __included_terra_rand
#define __included_terra_rand

void rand_initialize(unsigned int seed);
void rand_destroy();

void rand_set_seed(unsigned int seed);

unsigned char rand_bool(void);

int rand_integer(int begin,int end);

#define umbra_rand_range_integer(r) rand_integer(0,r)

void unique_integers_array(int offset,unsigned int range,int *intarray,unsigned int size);

unsigned int pick_one(unsigned int size, int reject);

double rand_double(double begin,double end);

int rand_normal(unsigned int mean, unsigned int range, unsigned int clip);

#endif
