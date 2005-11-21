/*
 * Copyright (c) 2005 Scott M. Feeney <graue@oceanbase.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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
