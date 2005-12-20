#include "spray.h"

static int keys = 0;

int getinput(void)
{
	SDL_Event event;
	int pause = 0;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_UP)
				keys |= KEY_UP;
			else if (event.key.keysym.sym == SDLK_DOWN)
				keys |= KEY_DOWN;
			else if (event.key.keysym.sym == SDLK_LEFT)
				keys |= KEY_LEFT;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				keys |= KEY_RIGHT;
			else if (event.key.keysym.sym == SDLK_F2)
				screen_save();
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				exit(0);
			else if (event.key.keysym.sym == SDLK_SPACE)
				pause = 1;
		}
		else if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_UP)
				keys &= ~KEY_UP;
			else if (event.key.keysym.sym == SDLK_DOWN)
				keys &= ~KEY_DOWN;
			else if (event.key.keysym.sym == SDLK_LEFT)
				keys &= ~KEY_LEFT;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				keys &= ~KEY_RIGHT;
		}
	}

	if (pause)
		displaytext("PAUSED");

	while (pause)
	{
		SDL_Delay(10);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				exit(0);
			if (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_ESCAPE)
			{
				exit(0);
			}
			if (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_SPACE)
			{
				pause = 0;
			}
		}

		if (!pause)
			restorescreen();
	}

	return keys;
}
