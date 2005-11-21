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
#include "mt.h"
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
