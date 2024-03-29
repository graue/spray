#include "spray.h"

static SDL_Surface *screen;
static SDL_Surface *fakescreen;
static int minx =       0, miny =        0;
static int maxx = WIDTH-1, maxy = HEIGHT-1;

void setupvideo(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
		error("SDL error: %s", SDL_GetError());

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0,
		SDL_ANYFORMAT|SDL_FULLSCREEN);
	if (screen == NULL)
		error("SDL error: %s", SDL_GetError());

	SDL_ShowCursor(SDL_DISABLE);

	fakescreen = SDL_CreateRGBSurface(0, WIDTH, HEIGHT,
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask);

	if (fakescreen == NULL)
		error("SDL error: %s", SDL_GetError());

	SDL_WM_SetCaption("Spray", NULL);
}

void clearscreen(void)
{
	SDL_FillRect(fakescreen, NULL, getcolor(0, 0, 0));
	SDL_FillRect(screen,     NULL, getcolor(0, 0, 0));
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

Uint32 getcolor(int r, int g, int b)
{
	return SDL_MapRGB(screen->format, r, g, b);
}

void putpixel(int x, int y, Uint32 color)
{
	SDL_Rect pixel;

	pixel.x = x;
	pixel.y = y;
	pixel.w = 1;
	pixel.h = 1;

	SDL_FillRect(fakescreen, &pixel, color);

	if (x < minx)
		minx = x;
	else if (x > maxx)
		maxx = x;

	if (y < miny)
		miny = y;
	else if (y > maxy)
		maxy = y;
}

void updatescreen(void)
{
	SDL_Rect area;

	if (minx > maxx || miny > maxy)
		return;

	area.x = minx;
	area.y = miny;
	area.w = maxx - minx + 1;
	area.h = maxy - miny + 1;

	SDL_BlitSurface(fakescreen, &area, screen, &area);
	SDL_UpdateRect(screen, minx, miny, maxx - minx + 1, maxy - miny + 1);

	minx = WIDTH-1, miny = HEIGHT-1;
	maxx =       0, maxy =        0;
}

void restorescreen(void)
{
	minx =       0, miny =        0;
	maxx = WIDTH-1, maxy = HEIGHT-1;
	updatescreen();
}

SDL_Surface *getscreen(void)
{
	return screen;
}

void screen_save(void)
{
	char filename[] = "xxxxxxxx.bmp";
	int i;

	for (i = 0; i < 8; i++)
		filename[i] = rnd(26) + 'a';

	SDL_SaveBMP(screen, filename);
}
