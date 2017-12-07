#include <stdio.h>
#include <time.h>

#include "ae.h"
#include "anet.h"
#include "zmalloc.h"
#include "adlist.h"
#include <sys/resource.h>

#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <sys/utsname.h>
#include <locale.h>
#include <sys/socket.h>


//结构体
#if 1
#define CONFIG_MIN_RESERVED_FDS 32
struct redisServer {
	/* Networking */
    int port;                   /* TCP listening port */
    int tcp_backlog;            /* TCP listen() backlog */
    char *bindaddr; 			/* Addresses we should bind to */
	char neterr[ANET_ERR_LEN];   /* Error buffer for anet.c */
	int ipfd;
	int tcpkeepalive;               /* Set SO_KEEPALIVE if non-zero. */
	aeEventLoop *el;
	long long stat_numconnections;
	
	list *clients;
	
    /* Limits */
    unsigned int maxclients;            /* Max number of simultaneous clients */
};
struct redisServer server; /* Server global state */

//初始化
void init(){
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
	server.port = 2017;
	server.tcp_backlog = 128;
	server.bindaddr = zstrdup("192.168.0.49");
	server.tcpkeepalive = 60;
	server.stat_numconnections = 0;
	server.clients = listCreate();
}

#endif

#if 1	//宏定义
#define C_OK                    0
#define C_ERR                   -1

#define NET_IP_STR_LEN 46

#define LL_DEBUG 0
#define LL_VERBOSE 1
#define LL_NOTICE 2
#define LL_WARNING 3
#define LL_RAW (1<<10) /* Modifier to log without timestamp */

#define LOG_MAX_LEN    1024
#define CONFIG_MIN_RESERVED_FDS 32

#define CLIENT_MASTER (1<<1)
#define CONFIG_FDSET_INCR (CONFIG_MIN_RESERVED_FDS+96)

#endif

//常用函数
#if 1

void serverLog(int level, const char *fmt, ...) {
    va_list ap;
    char msg[LOG_MAX_LEN];

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
	printf("##%s \n" , msg);
}

void adjustOpenFilesLimit(void) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
    rlim_t maxfiles = server.maxclients+CONFIG_MIN_RESERVED_FDS;
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE,&limit) == -1) {
        serverLog(LL_WARNING,"Unable to obtain the current NOFILE limit (%s), assuming 1024 and setting the max clients configuration accordingly.",
            strerror(errno));
        server.maxclients = 1024-CONFIG_MIN_RESERVED_FDS;
    } else {
        rlim_t oldlimit = limit.rlim_cur;

        /* Set the max number of files if the current limit is not enough
         * for our needs. */
        if (oldlimit < maxfiles) {
            rlim_t bestlimit;
            int setrlimit_error = 0;

            /* Try to set the file limit to match 'maxfiles' or at least
             * to the higher value supported less than maxfiles. */
            bestlimit = maxfiles;
            while(bestlimit > oldlimit) {
                rlim_t decr_step = 16;

                limit.rlim_cur = bestlimit;
                limit.rlim_max = bestlimit;
                if (setrlimit(RLIMIT_NOFILE,&limit) != -1) break;
                setrlimit_error = errno;

                /* We failed to set file limit to 'bestlimit'. Try with a
                 * smaller limit decrementing by a few FDs per iteration. */
                if (bestlimit < decr_step) break;
                bestlimit -= decr_step;
            }

            /* Assume that the limit we get initially is still valid if
             * our last try was even lower. */
            if (bestlimit < oldlimit) bestlimit = oldlimit;

            if (bestlimit < maxfiles) {
                rlim_t old_maxclients = server.maxclients;
                server.maxclients = bestlimit-CONFIG_MIN_RESERVED_FDS;
                /* maxclients is unsigned so may overflow: in order
                 * to check if maxclients is now logically less than 1
                 * we test indirectly via bestlimit. */
                if (bestlimit <= CONFIG_MIN_RESERVED_FDS) {
                    serverLog(LL_WARNING,"Your current 'ulimit -n' "
                        "of %llu is not enough for the server to start. "
                        "Please increase your open file limit to at least "
                        "%llu. Exiting.",
                        (unsigned long long) oldlimit,
                        (unsigned long long) maxfiles);
                    exit(1);
                }
                serverLog(LL_WARNING,"You requested maxclients of %d "
                    "requiring at least %llu max file descriptors.",
                    old_maxclients,
                    (unsigned long long) maxfiles);
                serverLog(LL_WARNING,"Server can't set maximum open files "
                    "to %llu because of OS error: %s.",
                    (unsigned long long) maxfiles, strerror(setrlimit_error));
                serverLog(LL_WARNING,"Current maximum open files is %llu. "
                    "maxclients has been reduced to %d to compensate for "
                    "low ulimit. "
                    "If you need higher maxclients increase 'ulimit -n'.",
                    (unsigned long long) bestlimit, server.maxclients);
            } else {
                serverLog(LL_NOTICE,"Increased maximum number of open files "
                    "to %llu (it was originally set to %llu).",
                    (unsigned long long) maxfiles,
                    (unsigned long long) oldlimit);
            }
        }
    }
}

int listenToPort(int port, int *fds ) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
    /* Bind IPv4 address. */
    *fds = anetTcpServer(server.neterr,port,server.bindaddr, server.tcp_backlog);
    if (*fds == ANET_ERR) {
        serverLog(LL_WARNING,
            "Creating Server TCP listening socket %s:%d: %s",
            server.bindaddr ? server.bindaddr : "*",
            port, server.neterr);
        return C_ERR;
    }
    anetNonBlock(NULL,*fds);
    return C_OK;
}

#endif

#if 1	//createClient

typedef struct client {
	int fd;                 /* Client socket. */
} client;

void freeClient(client *c) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
	listNode *ln;
	if (c->fd != -1) {
		/* Remove from the list of active clients. */
		ln = listSearchKey(server.clients,c);
		listDelNode(server.clients,ln);
	
		/* Unregister async I/O handlers and close the socket. */
		aeDeleteFileEvent(server.el,c->fd,AE_READABLE);
		aeDeleteFileEvent(server.el,c->fd,AE_WRITABLE);
		close(c->fd);
		c->fd = -1;
	}
	zfree(c);
}

#define PROTO_IOBUF_LEN         (1024*16)
void readQueryFromClient(aeEventLoop *el, int fd, void *privdata, int mask) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
    client *c = (client*) privdata;
    int nread, readlen;
    size_t qblen;

	char buff[PROTO_IOBUF_LEN];
    nread = read(fd, buff, PROTO_IOBUF_LEN);
    if (nread == -1) {
        if (errno == EAGAIN) {
            return;
        } else {
            serverLog(LL_VERBOSE, "Reading from client: %s",strerror(errno));
            freeClient(c);
            return;
        }
    } else if (nread == 0) {
        serverLog(LL_VERBOSE, "Client closed connection");
        freeClient(c);
        return;
    } else{
        //zhou
		buff[nread] = '\0';
		printf("##%s \n", buff);
    }
}

client *createClient(int fd) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
    client *c = zmalloc(sizeof(client));

    /* passing -1 as fd it is possible to create a non connected client.
     * This is useful since all the commands needs to be executed
     * in the context of a client. When commands are executed in other
     * contexts (for instance a Lua script) we need a non connected client. */
    if (fd != -1) {
        anetNonBlock(NULL,fd);
        anetEnableTcpNoDelay(NULL,fd);
        if (server.tcpkeepalive)
            anetKeepAlive(NULL,fd,server.tcpkeepalive);
		
		//hbb//事件驱动_6//将要_执行readQueryFromClient事件
        if (aeCreateFileEvent(server.el,fd,AE_READABLE, readQueryFromClient, c) == AE_ERR){
            close(fd);
            zfree(c);
            return NULL;
        }
    }

    c->fd = fd;
    
    if (fd != -1) listAddNodeTail(server.clients,c);
    return c;
}
#endif

#if 1	//acceptTcpHandler

#define MAX_ACCEPTS_PER_CALL 1000
static void acceptCommonHandler(int fd, int flags, char *ip) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
    client *c;
    if ((c = createClient(fd)) == NULL) {
        serverLog(LL_WARNING,
            "Error registering fd event for the new client: %s (fd=%d)",
            strerror(errno),fd);
        close(fd); /* May be already closed, just ignore errors */
        return;
    }

    server.stat_numconnections++;
}
void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
    int cport, cfd, max = MAX_ACCEPTS_PER_CALL;
    char cip[NET_IP_STR_LEN];

    while(max--) {
        cfd = anetTcpAccept(server.neterr, fd, cip, sizeof(cip), &cport);
        if (cfd == ANET_ERR) {
            if (errno != EWOULDBLOCK)
                serverLog(LL_WARNING,
                    "Accepting client connection: %s", server.neterr);
            return;
        }
        serverLog(LL_VERBOSE,"Accepted %s:%d", cip, cport);
        acceptCommonHandler(cfd,0,cip);
    }
}
#endif

//测试开服_函数
#if 1

int main()
{
	init();
	printf("%s:%d \n", __FUNCTION__ , __LINE__);
	setlocale(LC_COLLATE,"");
	
	adjustOpenFilesLimit();	//调整server.maxclients
	server.el = aeCreateEventLoop(server.maxclients+CONFIG_FDSET_INCR);//hbb//事件驱动_1
	if (server.el == NULL) {
		serverLog(LL_WARNING, "Failed creating the event loop. Error message: '%s'", strerror(errno));
		exit(1);
    }
	
	//开启端口
	if (listenToPort(server.port,&server.ipfd) == C_ERR){
		exit(1);
	}
	else{
		char ip_[46];
		int nPort;
		anetSockName(server.ipfd, ip_, 46, &nPort);
		printf("##%s:%d \n" , ip_ , nPort);
	}
	
	if (aeCreateFileEvent(server.el, server.ipfd, AE_READABLE, acceptTcpHandler,NULL) == AE_ERR)
	{
		printf( "Unrecoverable error creating server.ipfd file event.");
	}
	
	aeMain(server.el);//hbb//事件驱动_3
	
	aeDeleteEventLoop(server.el);
	
	return 0;
}
#endif

//测试连接函数
#if 0

inline int GetCurMs()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
}

int main()
{
	int max_ = 1024;
	char buf_[max_];
	char buff_log[max_];
	int n_s = anetTcpConnect(buff_log , "192.168.3.170" , 2017 );
	
	clock_t nCurT = GetCurMs();
	printf("diff time: %d ms\n", GetCurMs() - nCurT);
	
	{
		char ip[46];
		int n_port;
		int n_ret = anetSockName( n_s, ip, 46, &n_port);
		printf("%s:%d\n" , ip, n_port);
	}
	
	printf("%d\n", n_s);
	
	while(1){
		sleep(1);
		char msg[max_];
		short n1 = htons(4);
		memcpy(msg, (char *)(&n1) , 2);
		memcpy(msg + 2 , "hbb" , 4);
		anetWrite(n_s , msg,6);
		int nTotal = anetRead(n_s, buf_, 2);
		short nLen2 = ntohs( ((short*)buf_)[0] );
		nTotal = anetRead(n_s, buf_, nLen2);
		buf_[nTotal] = 0;
		if(nTotal > max_){
			printf("##%d\n" , __LINE__);
			return 1;
		}
	}


	return 0;
}

#endif



