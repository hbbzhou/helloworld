#ifndef _LINUX_AND_WIN_H_
#define _LINUX_AND_WIN_H_

#if defined(WIN32) || defined(WIN64)
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif


#if defined(WIN32) || defined(WIN64)

#define sleep_msec(x) Sleep( (x) )


#else

#define sleep_msec(x) usleep((x) * 1000)
#define _atoi64(val) strtoll(val, NULL, 10)
#define GetCurrentThreadId pthread_self

#endif


inline time_t GetCurMs()
{
#if defined(WIN32) || defined(WIN64)
	struct timeb tp;
	ftime(&tp);
	return tp.time * 1000 + tp.millitm;

#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}




#endif