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

	if (textwidth >= WIDTH || textheight >= HEIGHT)
		error("Font is too big (%dx%d)", textwidth, textheight);

	dest.x = WIDTH/2  - textwidth/2;
	dest.y = HEIGHT/2 - textheight/2;
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
	char scorestring[12] = "";
	SDL_Event event;

	snprintf(scorestring, sizeof scorestring, "%d", score);
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
