#include "spray.h"
#include "SDL_mixer.h"

static Mix_Music *music = NULL;

void startmusic(void)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		error("Audio error: %s", SDL_GetError());

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		error("Mixer error: %s", Mix_GetError());

	atexit(Mix_CloseAudio);

	music = Mix_LoadMUS("wak2.ogg");

	if (music == NULL)
		error("Mixer error: %s", Mix_GetError());

	Mix_PlayMusic(music, -1);
}
