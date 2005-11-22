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
