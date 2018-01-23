


//常用头文件
#if 1

#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>

#include <iostream>
#include <list>
#include <map>
#include <memory>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined(WIN32) || defined(WIN64)
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <set>
#include <sstream>


#include <vector>
#include <utility>
#include "linux_and_win.h"
#include "thread.h"

#endif

//测试常用函数
#if 1

void waitTo(time_t end_time) {
	while (time(NULL) < end_time) {
		sleep_msec(1);
	}
}

void write2File(int nAppID , std::string str_) {
	char name_[128];
	sprintf(name_ , "report_dir/%d.txt" , nAppID);
	FILE * f_ = fopen(name_, "w");
	if (f_ == NULL) {
		return;
	}
	fputs(str_.c_str(), f_ );
	fclose(f_); f_ = NULL;
}


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




int main(int argc, char* argv[])
{

	return 0;
}















