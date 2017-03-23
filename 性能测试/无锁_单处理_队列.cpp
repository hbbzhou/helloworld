
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