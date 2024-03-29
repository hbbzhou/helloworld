﻿
//用法
//class TestOne : public CTimer
//{
//public:
//	virtual void OnTimer(int id, long long uid)
//	{
//		printf("%s , %d \n", __FUNCTION__, __LINE__);
//		printf("执行 任务id %d \n", id);
//	}
//
//};
//
//void test_main()
//{
//
//	TestOne oTestB;
//	long long uid = oTestB.AddTimer(100, 11 + time(NULL));
//	oTestB.DelTimer(uid);//删除 指定 定时器
//
//	while (1) {
//		CTimerMgr::Instance().Run(10);
//		Sleep(100);
//	}
//}


#pragma once 
#include <stdio.h>
#include <map>
#include <set>
#include <time.h>


class CTimer;
class CTimerData
{
public:
	long long m_nUID;
	int m_nID;
	time_t m_nEndTime;
	CTimer * m_pTimer;
};

class CTimer
{
	std::map<long long, time_t> m_map;
public:
	virtual ~CTimer() {
		DelTimer();
	}

	virtual void OnTimer(int id, long long uid) = 0;

	void OnBaseTimer(int id, long long uid)
	{
		m_map.erase(uid);
		OnTimer(id, uid);
	}
	//新增 定时器
	long long AddTimer(int id, time_t time_);
	//删除 定时器
	void DelTimer(long long uid);
	void DelTimer();
};

class CTimerMgr
{
	CTimerMgr() {

	}
public:
	typedef std::set<CTimerData *> TimerSet;
	typedef std::map<time_t, std::set<CTimerData *> > TimerMap;

	static CTimerMgr& Instance()
	{
		static CTimerMgr g_oInstance;
		return g_oInstance;
	}

	CTimerData * GetFront() const
	{
		if (m_mapData.empty()) {
			return NULL;
		}

		const TimerSet & set_ = m_mapData.begin()->second;
		if (set_.empty()) {
			return NULL;
		}
		return *set_.begin();
	}

	void Pop()
	{
		if (m_mapData.empty()) {
			return;
		}

		TimerSet & set_ = m_mapData.begin()->second;
		if (set_.empty()) {
			m_mapData.erase(m_mapData.begin());
			return;
		}
		Destroy(*set_.begin());
		set_.erase(set_.begin());
	}

	bool DealOne()
	{
		if (m_mapData.empty()) {
			return false;
		}

		bool is_continue = true;

		do {
			CTimerData * pCTimerData = GetFront();
			if (pCTimerData == NULL) {
				break;
			}

			if (pCTimerData->m_nEndTime <= time(NULL)) {
				//调用 回调函数
				if (pCTimerData->m_pTimer) {
					pCTimerData->m_pTimer->OnBaseTimer(pCTimerData->m_nID, pCTimerData->m_nUID);
				}
			}
			else {
				return false;
			}

		} while (0);

		Pop();
		return is_continue;
	}

	bool Run(int count)
	{
		for (int i = 0; i < count; ++i) {
			if (DealOne() == false) {
				return false;
			}
		}
		return true;//任务 繁忙中
	}

	long long AddTimerJob(int id, time_t time_, CTimer * p_timer)
	{
		static long long uid = 1000;
		CTimerData * pCTimerData = new CTimerData();
		pCTimerData->m_nEndTime = time_;
		pCTimerData->m_nUID = ++uid;
		pCTimerData->m_nID = id;
		pCTimerData->m_pTimer = p_timer;

		m_mapData[pCTimerData->m_nEndTime].insert(pCTimerData);
		return pCTimerData->m_nUID;
	}

	void Close(long long uid, time_t n_time)
	{
		TimerMap::iterator it_ = m_mapData.find(n_time);
		if (it_ == m_mapData.end()) {
			printf("%s , %d \n", __FUNCTION__, __LINE__);
			return;
		}

		TimerSet::iterator it_set = it_->second.begin();
		for (; it_set != it_->second.end(); ++it_set) {
			CTimerData * pCTimerData = *it_set;
			if (pCTimerData) {
				if (pCTimerData->m_nUID == uid) {
					Destroy(pCTimerData);
					it_->second.erase(pCTimerData);
					break;
				}
			}
		}
	}

	void Destroy(CTimerData* pCTimerData)
	{
		printf("%s , %d \n", __FUNCTION__, __LINE__);
		if (pCTimerData) {
			printf("删除 任务 id %d \n", pCTimerData->m_nID);
			delete pCTimerData; pCTimerData = NULL;
		}
	}

	void Destroy()
	{
		for (TimerMap::const_iterator it_ = m_mapData.begin();
			it_ != m_mapData.end(); ++it_) {

			for (TimerSet::const_iterator it_set = it_->second.begin(); it_set != it_->second.end(); ++it_set) {
				Destroy(*it_set);
			}
		}
		m_mapData.clear();
	}

	TimerMap m_mapData;
};

//新增 定时器
long long CTimer::AddTimer(int id, time_t time_)
{
	long long uid = CTimerMgr::Instance().AddTimerJob(id, time_, this);
	m_map[uid] = time_;
	return uid;
}

//删除 定时器
void CTimer::DelTimer()
{
	for (std::map<long long, time_t>::const_iterator it_ = m_map.begin();
		it_ != m_map.end(); ++it_) {
		CTimerMgr::Instance().Close(it_->first, it_->second);
	}
	m_map.clear();
}


void CTimer::DelTimer(long long uid)
{
	std::map<long long, time_t>::const_iterator it_ = m_map.find(uid);
	if (it_ != m_map.end()) {
		CTimerMgr::Instance().Close(it_->first, it_->second);
		m_map.erase(it_);
	}
}



