
//百度:kfifo

#include <stdio.h>
#include <windows.h>
#include <map>
#include <thread>
#include <time.h>
#include <sstream> 
#include <sys/timeb.h>
#include <list>

long GetMSec()
{
	struct timeb tv;
	ftime(&tv);
	return ((long)tv.time) * 1000 + (long)tv.millitm;
}


std::map<int, int> g_map;
clock_t gBegin, gEnd, gDiff;
const int g_count = 99999;

//无锁队列
#if 0

const static int Len = 10000;
int g_list[Len];
int g_in_index = 0, g_out_index = 0;

int GetFreeLen()
{
	int n_len = 0;
	if (g_in_index >= g_out_index) {
		n_len = Len - (g_in_index - g_out_index);
	}
	else {
		n_len = g_out_index - g_in_index;
	}
	return --n_len;
}

inline void AddIndex(int &index_)
{
	index_ = (++index_) % Len;
}

void Add2List(int v_)
{
	while (GetFreeLen() <= 0) {
		printf("no free len\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	g_list[g_in_index] = v_;
	AddIndex(g_in_index);
}

bool GetOne(int & out_)
{
	if (g_in_index == g_out_index) {
		return false;
	}
	out_ = g_list[g_out_index];
	AddIndex(g_out_index);
	return true;
}

void run()
{
	int n_;
	while (true) {
		if (GetOne(n_)) {
			g_map[n_] += 1;
			if (n_ == g_count) {
				if (g_map.size() == g_count) {
					return;
				}
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}


void addJob()
{
	for (int i = 0; i < g_count; ++i) {
		Add2List(i + 1);
	}
}

void test_main()
{
	gBegin = GetMSec();
	std::thread t_(run);

	std::thread t_2(addJob);
	t_.join();
	t_2.join();
	gEnd = GetMSec();
	gDiff = (gEnd - gBegin);
	printf("%s %d s %d ms \n", "无锁队列##", gDiff / 1000, gDiff % 1000);
}

#endif

//无锁队列_2
#if 0
#include <thread>
#include <stdio.h>
#include <assert.h>

template <class T>
class LockFreeQueue
{
	const static int m_nLen = 10000;
	int m_nInIndex;
	int m_nPopIndex;

	T m_list[m_nLen];
public:
	LockFreeQueue() {
		m_nInIndex = 0;
		m_nPopIndex = 0;
	}

	int GetFreeLen()
	{
		int n_len = 0;
		if (m_nInIndex >= m_nPopIndex) {
			n_len = m_nLen - (m_nInIndex - m_nPopIndex);
		}
		else {
			n_len = m_nPopIndex - m_nInIndex;
		}
		return --n_len;
	}


	void AddIndex(int &index_)
	{
		index_ = (++index_) % m_nLen;
	}


	void Add2List(T v_)
	{
		while (GetFreeLen() <= 0) {
			printf("no free len\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		m_list[m_nInIndex] = v_;
		AddIndex(m_nInIndex);
	}


	bool GetOne(T & out_)
	{
		if (m_nInIndex == m_nPopIndex) {
			return false;
		}
		out_ = m_list[m_nPopIndex];
		AddIndex(m_nPopIndex);
		return true;
	}

};


LockFreeQueue<int> g_list;

//测试 队列中数据是否正确
void run()
{
	int n_ = -1;
	int n_old_data = 0;
	while (true) {
		if (g_list.GetOne(n_)) {
			if (n_old_data + 1 == n_) {
				n_old_data = n_;
			}
			else {
				assert(0);//数据异常
			}

			if (n_ == g_count) {
				return;
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void addJob()
{
	for (int i = 0; i < g_count; ++i) {
		g_list.Add2List(i + 1);
	}
}

void test_main()
{
	gBegin = GetMSec();
	std::thread t_(run);

	std::thread t_2(addJob);
	t_.join();
	t_2.join();
	gEnd = GetMSec();
	gDiff = (gEnd - gBegin);
	printf("%s %d s %d ms \n", "无锁队列##", gDiff / 1000, gDiff % 1000);
}

#endif

//带锁的队列
#if 1
#include <mutex>

std::mutex g_mtx;
std::list<int> g_list;
void Add2List(int v_)
{
	g_mtx.lock();
	g_list.insert(g_list.end(), v_);
	g_mtx.unlock();
}

bool GetOne(int & out_)
{
	if (g_list.empty()) {
		return false;
	}
	out_ = g_list.front();
	g_mtx.lock();
	g_list.pop_front();
	g_mtx.unlock();
	return true;
}

void run()
{
	while (true) {
		int n_;
		if (GetOne(n_)) {
			g_map[n_] += 1;
			if (n_ == g_count && g_map.size() == g_count) {
				return;
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void addJob()
{
	for (int i = 0; i < g_count; ++i) {
		Add2List(i + 1);
	}
}

void test_main()
{
	gBegin = GetMSec();
	std::thread t_(run);
	std::thread t_2(addJob);
	t_.join();
	t_2.join();
	gEnd = GetMSec();
	gDiff = (gEnd - gBegin);
	printf("%s %d s %d ms \n", "list队列##", gDiff / 1000, gDiff % 1000);
}

#endif
/*
无锁队列## 5 s 824 ms
list队列## 9 s 359 ms
*/

int main()
{
	test_main();

	system("pause");
	return 0;
}