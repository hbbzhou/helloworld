#include "thread.h"

CThreadEx::CThreadEx() : m_dwThreaID(0),
	m_Status(CThreadEx::READY)
{
#if defined(WIN32)
	m_hThread = NULL;
#endif
}

CThreadEx::~CThreadEx() 
{
}

void CThreadEx::start() 
{ 
	if (m_Status != CThreadEx::READY)
	{
		return ;
	}

	// thread create function
#if (defined(WIN32) || defined(WIN64))
	m_hThread = (HANDLE)::_beginthreadex(NULL, 0, MyThreadProcessEx, this, 0, &m_dwThreaID);
#else
	pthread_create((pthread_t*)&m_dwThreaID, NULL, MyThreadProcessEx, this);
#endif
}

void CThreadEx::stop()
{
}

void CThreadEx::exit(void * retval)
{
#if (defined(WIN32) || defined(WIN64))
	::CloseHandle(m_hThread);
#else
    pthread_exit(retval);
#endif
}

#if (defined(WIN32) || defined(WIN64))

uint32 WINAPI MyThreadProcessEx(void* derivedThread)
{
	CThreadEx *poThread = (CThreadEx *)derivedThread;
	if (NULL == poThread)
	{
		return 0;
	}

	// set thread's status to "RUNNING"
	poThread->setStatus(CThreadEx::RUNNING);

	// derived::run() called
	poThread->run();

	// set thread's status to "EXIT"
	poThread->setStatus(CThreadEx::EXIT);
	poThread->exit(NULL);

	return 0;	// avoid compiler's warning
}

#else

void* MyThreadProcessEx(void * derivedThread)
{
	CThreadEx * poThread = (CThreadEx *)derivedThread;
	if (NULL == poThread)
	{
		return NULL;
	}

	// set thread's status to "RUNNING"
	poThread->setStatus(CThreadEx::RUNNING);

	// here --polymorphism used.
	poThread->run();

	// set thread's status to "EXIT"
	poThread->setStatus(CThreadEx::EXIT);

	// INT ret = 0;
	//thread->exit(&ret);

	return NULL;	// avoid compiler's warning
}

#endif
