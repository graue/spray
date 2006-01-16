#include "spray.h"

void error(const char *msg, ...)
{
	va_list argptr;
	char str[1600] = "";

	va_start(argptr, msg);
	vsnprintf(str, sizeof str, msg, argptr);
	fprintf(stderr, "%s\n", str);
	va_end(argptr);

	exit(1);
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
