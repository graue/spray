#include "spray.h"
#include "mt19937ar.h"
#include <time.h>

#if defined(__OpenBSD__) && !defined(PORTABLE)
#define USE_RANDOM
#endif

void random_seed(void)
{
#ifdef USE_RANDOM
	srandomdev();
#else
	init_genrand((unsigned long) time(NULL));
#endif
}

int rnd3(int range)
{
	int r1, r2, r3;
	r1 = rnd(range);
	r2 = rnd(range);
	r3 = rnd(range);
	return (r1+r2+r3) / 3;
}

int rnd(int range)
{
#ifdef USE_RANDOM
	return random() % range;
#else
	return genrand_int31() % range;
#endif
}
