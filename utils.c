#include "spray.h"
#include "mt.h"
#include <time.h>

/*
 * Random number routines.
 */

#if defined(__OpenBSD__) && !defined(PORTABLE)
#define USE_RANDOM
#endif

void random_seed(void)
{
#ifdef USE_RANDOM
	srandomdev();
#else
	mt_init((unsigned long)time(NULL));
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
	return mt_rand() % range;
#endif
}

/*
 * Timer routines.
 */

static Uint32 next_time = 0;

void timer_start(void)
{
	next_time = SDL_GetTicks() + TICK_INTERVAL;
}

static Uint32 time_left(void)
{
	Uint32 now;
	now = SDL_GetTicks();
	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

void time_wait(void)
{
	Uint32 left;
	left = time_left();
	if (left > 10)
	{
		SDL_Delay(left);
		left = time_left();
	}
	if (left > 0)
		while (time_left()) { }
	timer_start();
}
