#ifndef __YMTcpSocket_H__
#define __YMTcpSocket_H__

#ifdef WIN32
#include <winsock.h>
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

typedef int SOCKET;
//#pragma region define win32const variable inlinux
#define INVALID_SOCKET			-1
#define SOCKET_ERROR			-1
//#pragmaendregion
#endif

class YMTcpSocket {
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
       

protected:
    SOCKET m_sock;
};
#endif //end __YMTcpSocket_H__
