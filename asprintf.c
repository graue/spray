#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int my_vasprintf(char **ret, const char *format, va_list ap)
{
	int charsneeded;
	char *str;

	charsneeded = vsnprintf(NULL, 0, format, ap) + 1;
	str = malloc(charsneeded);

	if (str == NULL)
	{
		/*
		 * Setting *ret to NULL is what OpenBSD does, but it
		 * should not be relied upon.
		 */
		*ret = NULL;
		return -1;
	}

	*ret = str;
	return vsprintf(str, format, ap);
}

int my_asprintf(char **ret, const char *format, ...)
{
	int result;
	va_list argptr;

	va_start(argptr, format);
	result = my_vasprintf(ret, format, argptr);
	va_end(argptr);

	return result;
}
