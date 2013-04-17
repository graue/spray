#include "spray.h"

static int keys = 0;
static SDL_Joystick *joy = NULL;
static int numaxes;
static int numbuttons;

/* initialize joystick if available */
static void initinput(void)
{
	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
		return;

	joy = SDL_JoystickOpen(0);
	if (joy == NULL)
		return;
	numaxes = SDL_JoystickNumAxes(joy);
	numbuttons = SDL_JoystickNumButtons(joy);
	if (numaxes < 2 || numbuttons < 1)
	{
		SDL_JoystickClose(joy);
		joy = NULL;
	}
}

int getinput(void)
{
	SDL_Event event;
	int pause = 0;
	static int inited = 0;
	static int lastbuttonstate = 0;
	int buttonstate = 0;

	if (!inited)
	{
		initinput();
		inited = 1;
	}

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

	if (joy)
	{
		SDL_JoystickUpdate();
		if (SDL_JoystickGetAxis(joy, 0) < 0)
		{
			keys &= ~KEY_RIGHT;
			keys |= KEY_LEFT;
		}
		else if (SDL_JoystickGetAxis(joy, 0) > 0)
		{
			keys &= ~KEY_LEFT;
			keys |= KEY_RIGHT;
		}
		if (SDL_JoystickGetAxis(joy, 1) < 0)
		{
			keys &= ~KEY_DOWN;
			keys |= KEY_UP;
		}
		else if (SDL_JoystickGetAxis(joy, 1) > 0)
		{
			keys &= ~KEY_UP;
			keys |= KEY_DOWN;
		}

		lastbuttonstate = buttonstate;
		buttonstate = SDL_JoystickGetButton(joy, 0);
		if (buttonstate && !lastbuttonstate)
			pause = 1;
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

		lastbuttonstate = buttonstate;
		buttonstate = SDL_JoystickGetButton(joy, 0);
		if (buttonstate && !lastbuttonstate)
			pause = 0;

		if (!pause)
			restorescreen();
	}

	return keys;
}
