//SocketLib.h
//version 2
//最后修改时间：2009-02-03 22:09
#include <assert.h>
//Code Block 2.1 头文件包含
#ifdef WIN32
#include "winsock2.h"
#include "Ws2tcpip.h"
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
//End Code Block 2.1

//Code Block 2.2 为交互兼容性进行重新定义和全局化处理
#ifdef WIN32
WSADATA g_wsadata;
#define CloseSocket closesocket
#define GetSocketError WSAGetLastError
#define StartSocketLib WSAStartup( MAKEWORD(2,2) , &g_wsadata);
#define CloseSocketLib WSACleanup();
#ifndef socklen_t
typedef int socklen_t;
#endif
#else
#define CloseSocket close
#define GetSocketError errno
#define StartSocketLib {}
#define CloseSocketLib {}
#endif
//End Code Block 2.2

namespace SocketLib{
	class System{
	public:
		System(){
			StartSocketLib;
		}
		~System(){
			CloseSocketLib;
		}
	}system;
};

class BaseSocket{
protected:
	int m_sock;
	bool m_closed;
	bool m_err;
public:
	BaseSocket(){
		m_closed=false;
		m_err=false;
		m_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}

	void close(){
		shutdown(m_sock,2);
		CloseSocket(m_sock);
		m_closed=true;
	}

	bool isClosed(){
		return m_closed;
	}

	operator bool (){
		return !m_err;
	}

	int getSock(){
		return m_sock;
	}
};

class Socket: public BaseSocket{
public:
	Socket(){}

	Socket(int sock){
		m_sock=sock;
	}

	Socket(char* ipaddr,unsigned short port){
		int err=connect(ipaddr,port);
		if(err!=0)assert(!"Connect Failed!");
	}

	int connect(char* ipaddr,unsigned short port){
		sockaddr_in socketaddress;
		socketaddress.sin_family=AF_INET;
		socketaddress.sin_port=htons(port);
		socketaddress.sin_addr.s_addr=inet_addr(ipaddr);
		memset(&(socketaddress.sin_zero),0,8);
		return ::connect(m_sock,(sockaddr*)&socketaddress,sizeof(sockaddr_in));
	}

	int send(const char* data,int size){
		return ::send(m_sock,data,size,0);
	}

	Socket& operator << (const char* str){
		send(str,strlen(str)+1);
		return *this;
	}

	char* recv(){
		static char buf[1024];
		int err=::recv(m_sock,buf,1024,0);
		if(err<=0)m_err=true;
		return buf;
	}

	Socket& operator >> (char* str){
		strcpy(str,recv());
		return *this;
	}
};


class ServerSocket: public BaseSocket{
	sockaddr_in m_socketaddress;
public:
	ServerSocket(){}

	ServerSocket(unsigned short port,int backlog=50,unsigned long ipaddr=INADDR_ANY){
		int err;
		err=bind(port,ipaddr);
		if(err!=0)assert(!"Bind Failed!");
		err=listen(backlog);
		if(err!=0)assert(!"Listen Failed!");
	}

	int bind(unsigned short port,unsigned long ipaddr=INADDR_ANY){
		m_socketaddress.sin_family=AF_INET;
		m_socketaddress.sin_port=htons(port);
		m_socketaddress.sin_addr.s_addr=htonl(ipaddr);
		memset(&(m_socketaddress.sin_zero),0,8);
		return ::bind(m_sock,(sockaddr*)&m_socketaddress,sizeof(sockaddr_in));
	}

	int listen(int backlog=50){
		return ::listen(m_sock,backlog);
	}

	Socket accept(){
		int dsock;
		int sa_size=sizeof(sockaddr_in);
		dsock=::accept(m_sock,(sockaddr*)&m_socketaddress,&sa_size);
		Socket s=Socket(dsock);
		return s;
	}
};