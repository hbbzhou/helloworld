
#pragma once

#include <time.h>
#include <sys/timeb.h>

namespace basefun {
#define ONE_DAY_SECOND (60*60*24)

	//获取时间差
	//例如:假设现在为 3:00 点钟;
	//如果:hour_and_minute = 60*60 那么 结果就是 60*60*22
	//如果:hour_and_minute = 4*60*60 那么 结果就是 60*60
	uint32 GetSecondDiff(uint32 hour_and_minute) const
	{
		uint32 n_diff = GetCurSecond() % SECOND2DAY;
		if (n_diff < hour_and_minute) {
			return hour_and_minute - n_diff;
		}

		return SECOND2DAY + hour_and_minute - n_diff;
	}

	uint32 GetSecondDiff(uint32 begin_time, uint32 hour_and_minute)const
	{
		uint32 n_diff = begin_time % SECOND2DAY;
		if (n_diff < hour_and_minute) {
			return hour_and_minute - n_diff;
		}

		return SECOND2DAY + hour_and_minute - n_diff;
	}

	long GetMSec()//获取毫秒
	{
		struct timeb tv;
		ftime(&tv);
		return ((long)tv.time) * 1000 + (long)tv.millitm;
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


