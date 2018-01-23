#ifndef _LINUX_AND_WIN_H_
#define _LINUX_AND_WIN_H_

#if defined(WIN32) || defined(WIN64)
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif


#if defined(WIN32) || defined(WIN64)
#include <sys/timeb.h>
#else
#define _atoi64(val) strtoll(val, NULL, 10)
#endif

#endif