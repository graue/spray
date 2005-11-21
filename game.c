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
#include <math.h>

#define WIDTH    640
#define HEIGHT   480
#define SCATTER   49 /* 39 */ /* 13 */
#define SPRAY     12 /* 8 */ /* 2 */
#define SPECIAL  190 /* 160 */ /* 30 */

#define MAXSPEED   7
#define MOVEMUL    2
#define ACCEL      2

#define TRUEWIDTH  (WIDTH  - SCATTER)
#define TRUEHEIGHT (HEIGHT - SCATTER)

static int falsify(const int fake)
{
	return fake + SCATTER/2;
}

static void spray(const int truex, const int truey, const Uint32 color,
                  const int amount)
{
	int falsex, falsey;

	falsex = falsify(truex);
	falsey = falsify(truey);

	do
	{
		int sprayx, sprayy;

		sprayx = falsex - SCATTER/2 + rnd(SCATTER);
		sprayy = falsey - SCATTER/2 + rnd(SCATTER);
		putpixel(sprayx, sprayy, color);
	} while (rnd(amount));
}

static int randvel(void)
{
	return rnd(MAXSPEED*2+1) - MAXSPEED;
}

static void move(int *px, int *py, int *pxvel, int *pyvel, int movemul)
{
	int x, xvel;
	int y, yvel;
	int xbounce = 0;
	int ybounce = 0;

	x = *px, xvel = *pxvel/movemul;
	y = *py, yvel = *pyvel/movemul;

	while (xvel != 0)
	{
		if (xvel < 0)
		{
			xvel++;
			if (x == 0)
			{
				xbounce = 1;
				xvel = -xvel;
			}
			else
				x--;
		}
		else
		{
			xvel--;
			if (x == TRUEWIDTH-1)
			{
				xbounce = 1;
				xvel = -xvel;
			}
			else
				x++;
		}
	}

	while (yvel != 0)
	{
		if (yvel < 0)
		{
			yvel++;
			if (y == 0)
			{
				ybounce = 1;
				yvel = -yvel;
			}
			else
				y--;
		}
		else
		{
			yvel--;
			if (y == TRUEHEIGHT-1)
			{
				ybounce = 1;
				yvel = -yvel;
			}
			else
				y++;
		}
	}

	*px = x, *py = y;
	if (xbounce)
		*pxvel = -*pxvel;
	if (ybounce)
		*pyvel = -*pyvel;
}

static int clamp(int velocity)
{
	if (velocity < -MAXSPEED)
		return -MAXSPEED;
	if (velocity > MAXSPEED)
		return MAXSPEED;
	return velocity;
}

static int avoid(int x, int y, int targetx, int targety)
{
	int keys = 0;

	/* fuzzy logic? */
	targetx += rnd(SCATTER/2-1) - (SCATTER-1);
	targety += rnd(SCATTER/2-1) - (SCATTER-1);

	if (targetx > x && rnd(TRUEWIDTH) > targetx - x)
		keys |= KEY_LEFT;
	if (targetx < x && rnd(TRUEWIDTH) > x - targetx)
		keys |= KEY_RIGHT;
	if (targety > y && rnd(TRUEHEIGHT) > targety - y)
		keys |= KEY_UP;
	if (targety < y && rnd(TRUEHEIGHT) > y - targety)
		keys |= KEY_DOWN;

	return keys;
}

static int randomselectkeys(int x, int y)
{
	int keys = 0;
	int bits = rnd(16);

	if (rnd3(TRUEWIDTH) > x && (bits & 1))
		keys |= KEY_RIGHT;
	if (rnd3(TRUEWIDTH) < x && (bits & 2))
		keys |= KEY_LEFT;
	if (rnd3(TRUEHEIGHT) > y && (bits & 4))
		keys |= KEY_DOWN;
	if (rnd3(TRUEHEIGHT) < y && (bits & 8))
		keys |= KEY_UP;

	return keys;
}

static void diminish(int *pvel)
{
	if (rnd(50) == 0)
	{
		if (*pvel > 2)
			(*pvel)--;
		else if (*pvel < 2)
			(*pvel)++;
	}
}

static void adjustaccel(int *pxacc, int *pyacc, int keys)
{
	int xa, ya;

	xa = *pxacc;
	ya = *pyacc;

	if (keys & KEY_LEFT)
		xa = -ACCEL;
	else if (xa < 0)
		xa += rnd(2);

	if (keys & KEY_RIGHT)
		xa = ACCEL;
	else if (xa > 0)
		xa -= rnd(2);

	if (keys & KEY_UP)
		ya = -ACCEL;
	else if (ya < 0)
		ya += rnd(2);

	if (keys & KEY_DOWN)
		ya = ACCEL;
	else if (ya > 0)
		ya -= rnd(2);

	*pxacc = xa;
	*pyacc = ya;
}

static void adjustvelocity(int *pxvel, int *pyvel, int xacc, int yacc)
{
	int i;
	int xv, yv;

	xv = *pxvel;
	yv = *pyvel;

	if (xacc < 0)
	{
		for (i = 0; i > xacc; i--)
		{
			if (rnd(2))
			{
				if (clamp(xv - 1) == xv)
					diminish(&yv);
			else
				xv--;
			}
		}
	}
	else
	{
		for (i = 0; i < xacc; i++)
		{
			if (rnd(2))
			{
				if (clamp(xv + 1) == xv)
					diminish(&yv);
				else
					xv++;
			}
		}
	}

	if (yacc < 0)
	{
		for (i = 0; i > yacc; i--)
		{
			if (rnd(2))
			{
				if (clamp(yv - 1) == yv)
					diminish(&xv);
				else
					yv--;
			}
		}
	}
	else
	{
		for (i = 0; i < yacc; i++)
		{
			if (rnd(2))
			{
				if (clamp(yv + 1) == yv)
					diminish(&xv);
				else
					yv++;
			}
		}
	}

	*pxvel = xv;
	*pyvel = yv;
}

static int box_collide(const int  a_left, const int    a_top,
                       const int a_right, const int a_bottom,
                       const int  b_left, const int    b_top,
                       const int b_right, const int b_bottom)
{
	if (a_bottom < b_top || b_bottom < a_top)
		return 0;

	if (a_right < b_left || b_right < a_left)
		return 0;

	return 1;
}

int playgame(void)
{
	int playerx, playery;
	int playerxvel, playeryvel;
	int playerxacc, playeryacc;
	int enemyx, enemyy;
	int enemyxvel, enemyyvel;
	int enemyxacc, enemyyacc;
	int enemymovestate = 0;
	int bluestage = 0;
	int bluerandom = 15;
	int red = 255;
	int dred = -1;
	int score = 0;
	Uint32 playercolor, enemycolor, collidecolor;

	playerx = rnd(TRUEWIDTH);
	playery = rnd(TRUEHEIGHT);

	enemyx = (playerx +  TRUEWIDTH/2) % TRUEWIDTH;
	enemyy = (playery + TRUEHEIGHT/2) % TRUEHEIGHT;

	playercolor  = getcolor(255, 255, 255);
	enemycolor   = getcolor(255,   0,   0);
	collidecolor = getcolor(208, 255, 224);

	playerxvel = randvel();
	playeryvel = randvel();
	enemyxvel  = randvel();
	enemyyvel  = randvel();

	playerxacc = playeryacc = 0;
	enemyxacc  = enemyyacc  = 0;

	timer_start();

	while (1)
	{
		int keys;

		spray(playerx, playery, playercolor, SPRAY);
		spray( enemyx,  enemyy, enemycolor,  SPRAY);

		if (box_collide(
			playerx, playery,
			playerx + SCATTER-1, playery + SCATTER-1,
			enemyx, enemyy,
			enemyx + SCATTER-1, enemyy + SCATTER-1))
		{
			score += 50 + rnd(50);
			spray((playerx + enemyx) / 2,
				(playery + enemyy) / 2, collidecolor,
				SPECIAL);
			if (bluerandom < 40 && rnd(2*bluestage) == 0)
				bluerandom++;
		}

		move(&playerx, &playery, &playerxvel, &playeryvel, MOVEMUL);
		move( &enemyx,  &enemyy,  &enemyxvel,  &enemyyvel, MOVEMUL/2);

		updatescreen();

		keys = getinput();
		adjustaccel(&playerxacc, &playeryacc, keys);
		adjustvelocity(&playerxvel, &playeryvel,
			playerxacc, playeryacc);

		if (enemymovestate == 0)
			keys = randomselectkeys(enemyx, enemyy);
		else
			keys = avoid(enemyx, enemyy, playerx, playery);
		adjustaccel(&enemyxacc, &enemyyacc, keys);
		adjustvelocity(&enemyxvel, &enemyyvel,
			enemyxacc, enemyyacc);

		enemyxvel = clamp(enemyxvel);
		enemyyvel = clamp(enemyyvel);

		if (enemymovestate == 0)
		{
			if (rnd(5) == 0)
				enemymovestate = 1;
		}
		else if (rnd(5) == 0)
			enemymovestate = 0;

		if (rnd(bluerandom) == 0)
		{
			bluestage++;
			if (bluestage >= 256)
			{
				playercolor = getcolor(
					(bluestage-256)&255,
					(bluestage-256)&255,
					255 - ((bluestage-256)&255));
			}
			else
			{
				playercolor = getcolor(
					255 - (bluestage&255),
					255 - (bluestage&255),
					255);
			}

			if (bluestage == 513)
				break;

			if (rnd(7) < 2 && bluerandom > 2)
				bluerandom--;
		}

		if (rnd(8) == 0)
		{
			if (dred > 0)
			{
				red++;
				if (red == 255)
					dred = -1;
			}
			else
			{
				red--;
				if (red == 96)
					dred = 1;
			}
			enemycolor = getcolor(red, 255-(red/2), 255-(red/2));
		}

		time_wait();
	}

	return score;
}
