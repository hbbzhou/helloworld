#ifndef _THREAD_EX_H_
#define _THREAD_EX_H_

#include "type.h"


#if (defined(WIN32) || defined(WIN64))
    #include "Windows.h"
    #include "process.h"
#else
    #include <pthread.h>
#endif


#if defined(WIN32) || defined(WIN64)
#define sleep_msec(x) Sleep( (x) )
#else
#define sleep_msec(x) usleep((x) * 1000)
#endif

class CThreadEx
{
public:
	enum ThreadStatus {
		READY,		// ready
		RUNNING,	// running
		EXITING,	// exiting
		EXIT,		// exit 
	};

public:
	CThreadEx ();
	virtual ~CThreadEx();

public :
	virtual void stop();
	virtual void run()=0;

	void start() ;
	void exit(void *retval = NULL) ;

public:
	// get thread identifier
	uint32 getTID()const { return m_dwThreaID; }
	
	// get/set thread's status
	ThreadStatus getStatus ()const { return m_Status; }
	void setStatus(ThreadStatus status ) { m_Status = status; }

private:
	// thread identifier variable
	uint32 m_dwThreaID;

	// thread status
	ThreadStatus m_Status;

#if (defined(WIN32) || defined(WIN64))
	HANDLE m_hThread;
#endif
};

// thread function
#if (defined(WIN32) || defined(WIN64))
    uint32 WINAPI MyThreadProcessEx(void* derivedThread);
#else
	void* MyThreadProcessEx (void* derivedThread);
#endif

#endif
