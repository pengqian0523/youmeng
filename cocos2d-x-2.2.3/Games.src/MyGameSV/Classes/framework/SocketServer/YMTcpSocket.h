#ifndef __YMTcpSocket_H__
#define __YMTcpSocket_H__

#include "YMEventDispatcher.h"
#include "YMSocketData.h"
#include "YMSocketEvent.h"
#include "YMSocketDataEvent.h"
#ifdef WIN32
#include <winsock.h>
#include "platform/third_party/win32/pthread/pthread.h"
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

typedef int SOCKET;
//#pragma region define win32const variable inlinux
#define INVALID_SOCKET			-1
#define SOCKET_ERROR			-1
//#pragmaendregion
#endif

class YMTcpSocket : public YMEventDispatcher{
public:
    YMTcpSocket(SOCKET sock = INVALID_SOCKET);
    ~YMTcpSocket();

    int Create(int af, int type, int protocol = IPPROTO_IP);
    int Connect(const char* ip, unsigned short port);

    bool Bind(unsigned short port);
    bool Listen(int backlog = 5);
    bool Accept(YMTcpSocket& s, char* fromip = NULL);
    int Send(const char* buf, int len, int flags = 0);
    int Recv(char* buf, int len, int flags = 0);
    int Close(); 

	int GetError();

    static bool Init();
    static bool Clean();	

	SOCKET getSocket() const;
	void setSocket(const SOCKET sock);
	//operator SOCKET();
	//YMTcpSocket& operator =(SOCKET s); 
    static bool DnsParse(const char* domain, char*ip);
	static void* Handler(void *arg);
	void schaleHandler();
	int SendMsg(int cmd, YMSocketData *data);
	int chars2Int(char chars[],int start);
	void int2Chars(char *chars, int val, int start = 0);
	string int2String(unsigned int val);
	void getKey(int cmd, int stamp, int needKeylen, char buffer[]);
	void encrypt(char buffer[], int size, int cmd, int stamp);
	void hasError();
	int createThread();
	static YMTcpSocket* getIns();
protected:
    SOCKET m_sock;
	unsigned int iStamp;
	pthread_t threadId;
	pthread_attr_t threadAttr;
	static YMTcpSocket m_ins;
	int m_Server[100];
	int m_maxConnect;
	int m_clientSock;
};
#endif //end __YMTcpSocket_H__
