#ifndef MT_H
#define MT_H

#ifndef _WIN32
#include <sys/types.h>
#else
typedef int int32_t;
typedef unsigned int u_int32_t;
#endif

void mt_init(u_int32_t seed);
int32_t mt_rand(void);

#endif
