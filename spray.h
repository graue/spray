#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

#define TICK_INTERVAL 24
#define KEY_LEFT  (1<<0)
#define KEY_RIGHT (1<<1)
#define KEY_UP    (1<<2)
#define KEY_DOWN  (1<<3)

/* error stuff */
void error(const char *msg, ...);

/* game stuff */
int playgame(void);

/* input stuff */
int getinput(void);

/* menu stuff */
int showmenu(const int score);
void displaytext(const char *text);

/* music stuff */
void startmusic(void);

/* random stuff */
#define rnd(range) (random() % range)
int rnd3(int range);

/* time stuff */
void time_wait(void);
void timer_start(void);

/* video stuff */
void setupvideo(void);
Uint32 getcolor(int r, int g, int b);
void putpixel(int x, int y, Uint32 color);
void updatescreen(void);
SDL_Surface *getscreen(void);
void clearscreen(void);
void restorescreen(void);
