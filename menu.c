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
#include "SDL_ttf.h"

void displaytext(const char *text)
{
	TTF_Font *font = NULL;
	SDL_Color textcolor = {255, 192, 0}; /* happy orange */
	SDL_Surface *textsurface;
	SDL_Rect dest;
	int textwidth, textheight;

	if (TTF_Init() < 0)
		error("Font error: %s", TTF_GetError());

	font = TTF_OpenFont("font.ttf", 48);

	if (font == NULL)
		error("Font error: %s", TTF_GetError());

	if (TTF_SizeText(font, text, &textwidth, &textheight) < 0)
		error("Font error: %s", TTF_GetError());

	if (textwidth >= 640 || textheight >= 480)
		error("Font is too big (%dx%d)", textwidth, textheight);

	dest.x = 320 - textwidth/2;
	dest.y = 240 - textheight/2;
	dest.w = textwidth;
	dest.h = textheight;

	textsurface = TTF_RenderText_Blended(font, text, textcolor);

	if (!textsurface)
		error("Font error: %s", TTF_GetError());

	SDL_BlitSurface(textsurface, NULL, getscreen(), &dest);
	SDL_FreeSurface(textsurface);
	SDL_UpdateRect(getscreen(), 0, 0, 0, 0);

	TTF_CloseFont(font);
	font = NULL;
	TTF_Quit();
}

int showmenu(const int score)
{
	char *scorestring;
	SDL_Event event;

	if (asprintf(&scorestring, "%d", score) == -1)
		scorestring = "ERROR";

	displaytext(scorestring);

	while (1)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (event.type != SDL_KEYDOWN)
				continue;
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return 0;
			if (event.key.keysym.sym != SDLK_SPACE
				&& event.key.keysym.sym != SDLK_RETURN)
			{
				continue;
			}
			break;
		}
		else
			SDL_Delay(10);
	}

	clearscreen();
	return 1;
}
