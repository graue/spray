#include "spray.h"

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
