
#pragma once

#include <time.h>

namespace basefun {
#define ONE_DAY_SECOND (60*60*24)

	int GetSecondNowToSomeTime(int second)
	{
		int n_diff = time(NULL) % ONE_DAY_SECOND;
		if (n_diff < second) {
			return second - n_diff;
		}

		return ONE_DAY_SECOND + second - n_diff;
	}

	int GetSecondSomeTimeToSomeTime(int some_time, int second)
	{
		int n_diff = some_time % ONE_DAY_SECOND;
		if (n_diff < second) {
			return second - n_diff;
		}

		return ONE_DAY_SECOND + second - n_diff;
	}

union CharAndShort {
	//判断 大小端
	bool IsLittleEndian()
	{
		m_n1 = (1<<8) + 1;
		return m_c1;
	}
	char m_c1;
	short m_n1;
};
	
}

//=====
//============
//============================
#include <sstream> 
#include <stdio.h>
#include <time.h>

namespace log2file {

	static const char * g_path = "../../login_mode.txt";
	static char g_buf[32] = { 0 };
	static inline const char * GetTimeStr()
	{
		time_t n_time = time(NULL);
		tm* tm_time;
		tm_time = localtime(&n_time);

		memset(g_buf, 0, sizeof(g_buf));
#if defined(WIN32) || defined(WIN64)
		sprintf_s(g_buf, sizeof(g_buf), "[%4d-%02d-%02d %02d:%02d:%02d] ", tm_time->tm_year + 1900,
			tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
#else
		snprintf(g_buf, sizeof(g_buf), "[%4d-%02d-%02d %02d:%02d:%02d] ", tm_time->tm_year + 1900,
			tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
#endif
		return g_buf;
	}

	static inline void Add2file(const char * str_fun ,std::string str_content)
	{
		FILE  * handle = fopen(g_path , "a");
		if (handle) {
			std::string str_ = GetTimeStr();
			str_ += str_fun;
			str_ += "\t";
			str_ += str_content + "\n";
			fputs(str_.c_str(), handle);
			fclose(handle);
		}
	}
	//__FUNCTION__
#define _Add2file(x) Add2file(__FUNCTION__ , (x) )


}


