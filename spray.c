#include "spray.h"
#include <stdarg.h>

void error(const char *msg, ...)
{
	va_list argptr;
	char *str;

	va_start(argptr, msg);
	if (vasprintf(&str, msg, argptr) == -1)
		fprintf(stderr, "Error too long to print out: %s\n", msg);
	else
	{
		fprintf(stderr, "%s\n", str);
		free(str);
	}
	va_end(argptr);

	exit(1);
}

static void random_seed(void)
{
	srandomdev();
}

int rnd3(int range)
{
	int r1, r2, r3;
	r1 = rnd(range);
	r2 = rnd(range);
	r3 = rnd(range);
	return (r1+r2+r3) / 3;
}

int main(int argc, char *argv[])
{
	int score;

	random_seed();
	setupvideo();
	startmusic();
	do
	{
		score = playgame();
	} while (showmenu(score));
	argc = argc, argv = argv;
	return 0;
}
