
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

	const int endian = 1;
	#define is_bigendian() ( (*(char*) &endian) == 0 )
	#define is_littlendbian() ( (*(char*) &endian) == 1 )
	
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

#if defined(WIN32) || defined(WIN64)
#else

static char * _ltoa(int value, char * pStr, int radix) {
	char tmp[32 + 1];
	char *tp = tmp;
	char i;
	unsigned int v;
	int sign;
	char *sp;

	if (radix > 36 || radix <= 1) {
		return NULL;
	}

	sign = (radix == 10 && value < 0);
	if (sign)
		v = -value;
	else
		v = (unsigned int)value;

	while (v || tp == tmp) {
		i = char(v % radix);
		v = v / radix;
		if (i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	if (pStr == NULL)
		pStr = (char *)malloc((tp - tmp) + sign + 1);
	sp = pStr;

	if (sign)
		*sp++ = '-';
	while (tp > tmp)
		*sp++ = *--tp;
	*sp = '\0';
	return pStr;
}

static char * _i64toa(long long value, char *pStr, int radix) {
	char tmp[64 + 1];
	char *tp = tmp;
	char i;
	unsigned long long v;
	int sign;
	char *sp;

	if (radix > 36 || radix <= 1) {
		return NULL;
	}

	sign = (radix == 10 && value < 0);
	if (sign)
		v = -value;
	else
		v = (unsigned long long)value;
	while (v || tp == tmp) {
		i = char(v % radix);
		v = v / radix;
		if (i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	if (pStr == NULL)
		pStr = (char *)malloc((tp - tmp) + sign + 1);
	sp = pStr;

	if (sign)
		*sp++ = '-';
	while (tp > tmp)
		*sp++ = *--tp;
	*sp = '\0';
	return pStr;
}

static char * _ultoa(unsigned int value, char *pStr, int radix) {
	char tmp[32 + 1];
	char *tp = tmp;
	char i;
	unsigned int v = value;
	char *sp;

	if (radix > 36 || radix <= 1) {
		return NULL;
	}

	while (v || tp == tmp) {
		i = char(v % radix);
		v = v / radix;
		if (i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}
	if (pStr == NULL)
		pStr = (char *)malloc((tp - tmp) + 1);
	sp = pStr;

	while (tp > tmp)
		*sp++ = *--tp;
	*sp = '\0';
	return pStr;
}

static char * _ui64toa(unsigned long long value, char *pStr, int radix) {
	char tmp[64 + 1];
	char *tp = tmp;
	char i;
	unsigned long long v = value;
	char *sp;

	if (radix > 36 || radix <= 1) {
		return NULL;
	}

	while (v || tp == tmp) {
		i = char(v % radix);
		v = v / radix;
		if (i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}
	if (pStr == NULL)
		pStr = (char *)malloc((tp - tmp) + 1);
	sp = pStr;

	while (tp > tmp)
		*sp++ = *--tp;
	*sp = '\0';
	return pStr;
}

#endif




#define Code_StringAppend(T) \
T &operator << (const double v_) {							\
	const int nRatio = 1000;	/*精度*/					\
	long long n1 = llabs( (long long)(v_ * nRatio));		\
	long long  nInteger = n1 / nRatio;	/*整数部分*/		\
	int nDecimal = (long long)(n1) % nRatio; /*小数部分*/	\
	char buff[56] = { 0 };									\
	int nIndex = 0;											\
	memset(buff, 0, sizeof(buff));							\
	if (v_ < 0) {											\
		buff[nIndex++] = '-';								\
	}														\
	_i64toa(nInteger, buff + nIndex, 10);					\
	strcat(buff, ".");										\
	_ltoa(nDecimal, buff + strlen(buff), 10);				\
	Append(buff,strlen(buff));								\
	return *this;											\
}															\
T &operator << (const int v) {								\
	char buff[32];											\
	_ltoa(v, buff, 10);										\
	Append(buff,strlen(buff));								\
	return *this;											\
}															\
T &operator << (const unsigned int v) {						\
	char buff[32];											\
	_ultoa(v, buff, 10);									\
	Append(buff,strlen(buff));								\
	return *this;											\
}															\
T &operator << (const long long v) {						\
	char buff[32];											\
	_i64toa(v, buff, 10);									\
	Append(buff,strlen(buff));								\
	return *this;											\
}															\
T &operator << (const unsigned long long v) {				\
	char buff[32];											\
	_ui64toa(v, buff, 10);									\
	Append(buff,strlen(buff));								\
	return *this;											\
}															\
T &operator << (const std::string & str_) {					\
	Append(str_.c_str() , str_.size() );					\
	return *this;											\
}															\
T &operator << (const long unsigned int v) {				\
	char buff[32];											\
	_ui64toa(v, buff, 10);									\
	Append(buff,strlen(buff));								\
	return *this;											\
}															\
T &operator << (const char * pStr) {						\
	Append(pStr,strlen(pStr));								\
	return *this;											\
}


template<int SIZE>
class StringAppend
{
public:
	StringAppend() {
		m_szBuf[0] = '\0';
		m_nPos = 0;
	}
	//
	Code_StringAppend(StringAppend)
	//
	const char * Get() const {
		return m_szBuf;
	}

	void Append(const char * pStr , size_t nLen) {
		if (m_nPos + nLen >= SIZE) {
			assert(false && "out size");
		}
		memcpy(m_szBuf + m_nPos, pStr, nLen);
		m_nPos += nLen;
		m_szBuf[m_nPos] = '\0';
	}

private:
	char m_szBuf[SIZE];
	int  m_nPos;
};
//
class StringAppendExt
{
public:
	StringAppendExt(size_t nLen = 128)
		:pBuff(NULL), m_nMaxLen(nLen), m_nPos(0) {
		//
		pBuff = (char *)malloc(m_nMaxLen);
		assert(pBuff && "malloc error");
		pBuff[0] = '\0';
		//
	}

	//
	Code_StringAppend(StringAppendExt)
		//
	const char * Get() const {
		return pBuff;
	}

	void Append(const char * pStr, size_t nLen) {
		if (m_nPos + nLen >= m_nMaxLen) {
			Resize(CalSize(m_nPos + nLen));
		}
		memcpy(pBuff + m_nPos, pStr, nLen);
		m_nPos += nLen;
		pBuff[m_nPos] = '\0';
	}

	size_t CalSize(size_t nLen) {
		const int nStep = 1024 * 10;
		nLen += nStep - (nLen % nStep);
		nLen *= 2;
		return nLen;
	}

	void Resize(size_t nLen) {
		if (m_nMaxLen < nLen) {
			char * ptr_ = (char *)realloc(pBuff, nLen);
			assert(ptr_ && "realloc size");
			//
			pBuff = ptr_;
			m_nMaxLen = nLen;
		}
	}

	~StringAppendExt() {
		if (pBuff) {
			free(pBuff); pBuff = NULL;
		}
	}

private:
	char * pBuff;
	size_t m_nMaxLen;
	size_t m_nPos;
};

