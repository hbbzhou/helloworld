#ifndef  __MUTEX_H__
#define  __MUTEX_H__

#if defined(WIN32) || defined(WIN64)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "windows.h" 

#define mutex_t           CRITICAL_SECTION
#define mutex_init(m)     InitializeCriticalSection(m)
#define mutex_acquire(m)  EnterCriticalSection(m)
#define mutex_release(m)  LeaveCriticalSection(m)
#define mutex_destroy(m)  DeleteCriticalSection(m)

#else

#include <pthread.h>
#define mutex_t           pthread_mutex_t
#define mutex_init(m)     pthread_mutex_init(m, NULL)
#define mutex_acquire(m)  pthread_mutex_lock(m)
#define mutex_release(m)  pthread_mutex_unlock(m)
#define mutex_destroy(m)  pthread_mutex_destroy(m)

#endif

// locker
class CLocker
{
public:
	CLocker()
	{
		mutex_init(&m_oCS);
	}
	virtual ~CLocker()
	{
		mutex_destroy(&m_oCS);
	}

	virtual void Lock()
	{
		mutex_acquire(&m_oCS);
	}

	virtual void Unlock()
	{
		mutex_release(&m_oCS);
	}

	virtual void Release()
	{
	}

protected:
	mutex_t m_oCS;
};


#endif
