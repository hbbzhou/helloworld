#include <stdio.h>
#include <time.h>
//boost 无锁
#if 1

#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>
#include "thread.h"
#include <vector>
using namespace std;

#if 0
#include <atomic>
//是否生产完毕标志
std::atomic<bool> done(false);
#else
//是否生产完毕标志
bool done = false;
#endif


//队列
boost::lockfree::spsc_queue<int, boost::lockfree::capacity<1000 * 10000> > spsc_queue;

//迭代次数
const int iterations = 1000*10000;

class Cproducer : public CThreadEx
{
	//生产函数
	virtual void run() {
		for (int i = 0; i != iterations; ++i) {
			//原子计数————多线程不存在计数不上的情况
			int value = ++producer_count;
			//cout << "*";     //观察生产类型： 纯生产还是同时有消费的情况

			//若没有进入队列，则重复推送
			while (!spsc_queue.push(value));
		}
	}

public:
	Cproducer() {
		producer_count = 0;
	}

	//生产数量
	int producer_count;
};

class Cconsumer : public CThreadEx
{
	//消费函数
	virtual void run() {
		int value;
		//当没有生产完毕，则边消费边生产
		while (!done) {
			//只要能弹出元素，就消费
			while (spsc_queue.pop(value)) {
				//cout << ".";     //观察消费类型： 纯消费还是边生产边消费
				++consumer_count;
			}
		}
		//如果生产完毕，则消费
		while (spsc_queue.pop(value))
			++consumer_count;
	}
public:
	Cconsumer() {
		consumer_count = 0;
	}
	//消费数量
	int consumer_count;
};


void test_main() {
	if (!spsc_queue.is_lock_free()) {
		cout << "boost::lockfree::queue is not lockfree"<< endl;
		return;
	}
		
	//线程群管理器
	std::vector<Cproducer *> vec_producer;
	std::vector<Cconsumer *> vec_consumer;

	//创建生产者线程
	{
		Cproducer * ptr_ = new Cproducer();
		ptr_->start();
		vec_producer.push_back(ptr_);
	}

	//创建消费者线程
	{
		Cconsumer * ptr_ = new Cconsumer();
		ptr_->start();
		vec_consumer.push_back(ptr_);
	}

	//等待生产者生产完毕
	for (size_t i = 0; i < vec_producer.size(); ++i) {
		while (vec_producer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}

	
	//可以消费标志
	done = true;     //主线程不等生产线程完毕就设置done标记
	cout << endl;
	cout << "done" << endl;    //输出以观察主线程和各子线程的执行顺序

	//等待消费者结束
	for (size_t i = 0; i < vec_consumer.size(); ++i) {
		while (vec_consumer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}
	//输出生产和消费数量
	cout << endl;

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_producer.size(); ++i) {
			all_count += vec_producer[i]->producer_count;
		}
		cout << "produced " << all_count << " objects." << endl;
	}
	
	{
		int all_count = 0;
		for (size_t i = 0; i < vec_consumer.size(); ++i) {
			all_count += vec_consumer[i]->consumer_count;
		}
		cout << "consumed " << all_count << " objects." << endl;
	}
	

	//释放资源
	//略
}

#else

//boost 无锁的 对比测试

#include <iostream>
#include "thread.h"
#include <vector>
#include <deque>
#include "mutex.h"
using namespace std;

#if 0
#include <atomic>
//是否生产完毕标志
std::atomic<bool> done(false);
#else
//是否生产完毕标志
bool done = false;
#endif


//队列
std::deque<int> queue;
CLocker gLock;

//迭代次数
const int iterations = 1000 * 10000;

class Cproducer : public CThreadEx
{
	//生产函数
	virtual void run() {
		for (int i = 0; i != iterations; ++i) {
			int value = ++producer_count;
			//cout << "*";     //观察生产类型： 纯生产还是同时有消费的情况

			//若没有进入队列，则重复推送
			gLock.Lock();
			queue.push_front(value);
			gLock.Unlock();
		}
	}

public:
	Cproducer() {
		producer_count = 0;
	}

	//生产数量
	int producer_count;
};

class Cconsumer : public CThreadEx
{
	//消费函数
	virtual void run() {
		int value;
		//当没有生产完毕，则边消费边生产
		while (!done) {
			//只要能弹出元素，就消费
			gLock.Lock();
			if (queue.empty()) {
				gLock.Unlock();
				continue;
			}
			value = queue.back();
			queue.pop_back();
			gLock.Unlock();

			//cout << ".";     //观察消费类型： 纯消费还是边生产边消费
			++consumer_count;
		}
		//如果生产完毕，则消费
		while (1) {
			gLock.Lock();
			if (queue.empty()) {
				gLock.Unlock();
				break;
			}
			value = queue.back();
			queue.pop_back();
			gLock.Unlock();

			++consumer_count;
		}
	}
public:
	Cconsumer() {
		consumer_count = 0;
	}
	//消费数量
	int consumer_count;
};


void test_main() {

	//线程群管理器
	std::vector<Cproducer *> vec_producer;
	std::vector<Cconsumer *> vec_consumer;

	//创建生产者线程
	{
		Cproducer * ptr_ = new Cproducer();
		ptr_->start();
		vec_producer.push_back(ptr_);
	}

	//创建消费者线程
	{
		Cconsumer * ptr_ = new Cconsumer();
		ptr_->start();
		vec_consumer.push_back(ptr_);
	}

	//等待生产者生产完毕
	for (size_t i = 0; i < vec_producer.size(); ++i) {
		while (vec_producer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}


	//可以消费标志
	done = true;     //主线程不等生产线程完毕就设置done标记
	cout << endl;
	cout << "lock done" << endl;    //输出以观察主线程和各子线程的执行顺序

	//等待消费者结束
	for (size_t i = 0; i < vec_consumer.size(); ++i) {
		while (vec_consumer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}
	//输出生产和消费数量
	cout << endl;

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_producer.size(); ++i) {
			all_count += vec_producer[i]->producer_count;
		}
		cout << "produced " << all_count << " objects." << endl;
	}

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_consumer.size(); ++i) {
			all_count += vec_consumer[i]->consumer_count;
		}
		cout << "consumed " << all_count << " objects." << endl;
	}


	//释放资源
	//略
}


#endif

inline int GetCurMs()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
}

int main()
{
	clock_t nCurT = GetCurMs();
	test_main();
	printf("diff time: %d ms\n", GetCurMs() - nCurT);
	printf("end\n");
	return 0;
}

