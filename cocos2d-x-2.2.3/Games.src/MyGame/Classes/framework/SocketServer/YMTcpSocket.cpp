#include "YMTcpSocket.h"
#include <stdio.h>

#ifdef WIN32
#pragma comment(lib,"wsock32")
#endif


YMTcpSocket::YMTcpSocket(SOCKET sock):
m_sock(sock)
{
    
}

YMTcpSocket::~YMTcpSocket() 
{

}

bool YMTcpSocket::Init() 
{
#ifdef WIN32
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 2);
    int ret = WSAStartup(version, &wsaData);
    if ( ret ) 
	{
		printf("%s","init winsock error!");
        return false;
    }

#endif
    return true;
}

bool YMTcpSocket::Clean() 
{
#ifdef WIN32
	return WSACleanup() == 0 ? true : false;
#endif
    return true;
}

//YMTcpSocket& YMTcpSocket::operator = (SOCKET s) {
//	m_sock = s;
//	return (*this);
//}
void YMTcpSocket::setSocket(const SOCKET sock)
{
	m_sock = sock;
}

SOCKET YMTcpSocket::getSocket() const
{
	return m_sock;
}

//YMTcpSocket::operator SOCKET () {
//	return m_sock;
//}

int YMTcpSocket::Create(int af, int type, int protocol) 
{
    m_sock = socket(af, type, protocol);
	if( m_sock == INVALID_SOCKET )
	{
		YMTcpSocket::Clean();
		Close();
	}

	return m_sock;
}

int YMTcpSocket::Connect(const char* ip, unsigned short port) 
{
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr(ip);
    svraddr.sin_port = htons(port);

    int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));

	return ret;
} 

bool YMTcpSocket::Bind(unsigned short port) {
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = INADDR_ANY;
    svraddr.sin_port = htons(port);

    int opt = 1;

    if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 )
        return false;

    int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));

    if ( ret == SOCKET_ERROR ) {
        return false;
    }

    return true;
}

bool YMTcpSocket::Listen(int backlog) {
    int ret = listen(m_sock, backlog);

    if ( ret == SOCKET_ERROR ) {
        return false;
    }

    return true;
}

bool YMTcpSocket::Accept(YMTcpSocket& s, char*fromip) {
    struct sockaddr_in cliaddr;
    socklen_t addrlen = sizeof(cliaddr);
    SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);

    if ( sock == SOCKET_ERROR ) {
        return false;
    }

    s = sock;

    if ( fromip != NULL) {
        sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));
    }

    return true;
}

int YMTcpSocket::Send(const char* buf, int len, int flags) {
    int bytes;
    int count = 0;
	
    while ( count < len ) {
        bytes = send(m_sock, buf + count, len - count, flags);
        if ( bytes == -1 || bytes == 0 ) {
            return -1;
        }
        count += bytes;
    }

    return count;
}

int YMTcpSocket::Recv(char* buf, int len, int flags) {
    return recv(m_sock, buf, len, flags);
}

int YMTcpSocket::Close() 
{
#ifdef WIN32
    return (closesocket(m_sock));
#else
    return (close(m_sock));
#endif
}

int YMTcpSocket::GetError() 
{
#ifdef WIN32
    return (WSAGetLastError());
#else
    return (errno);
#endif
}

bool YMTcpSocket::DnsParse(const char* domain, char*ip) {
    struct hostent*p;

    if ( (p = gethostbyname(domain)) == NULL )
        return false;

    sprintf(ip,
            "%u.%u.%u.%u",
            (unsigned char)p->h_addr_list[0][0],
            (unsigned char)p->h_addr_list[0][1],
            (unsigned char)p->h_addr_list[0][2],
            (unsigned char)p->h_addr_list[0][3]);

    return true;
}
