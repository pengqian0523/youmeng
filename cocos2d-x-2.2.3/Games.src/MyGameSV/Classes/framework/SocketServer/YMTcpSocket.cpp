#include "YMTcpSocket.h"
#include <stdio.h>
#include "YMServerReply.h"

#ifdef WIN32
#pragma comment(lib,"wsock32")
#endif

YMTcpSocket YMTcpSocket::m_ins;
YMTcpSocket* YMTcpSocket::getIns(){
	return &m_ins;
}

YMTcpSocket::YMTcpSocket(SOCKET sock):
m_sock(sock),
m_maxConnect(100),
iStamp(1)
{
    
}

YMTcpSocket::~YMTcpSocket() 
{
	CCLOG("!!!");
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

void YMTcpSocket::setSocket(const SOCKET sock)
{
	m_sock = sock;
}

SOCKET YMTcpSocket::getSocket() const
{
	return m_sock;
}


int YMTcpSocket::Create(int af, int type, int protocol) 
{
    m_sock = socket(af, type, protocol);
	if( m_sock == INVALID_SOCKET )
	{
		Clean();
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

    int opt = 4;

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
    m_clientSock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);

    if ( m_clientSock == SOCKET_ERROR) {
        return false;
    }

    if ( fromip != NULL) {
        sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));
		CCLOG("client ip %s",fromip);
    }

    return true;
}

int YMTcpSocket::Send(const char* buf, int len, int flags) {
    int bytes;
    int count = 0;
	
    while ( count < len ) {
        bytes = send(m_clientSock, buf + count, len - count, flags);
        if ( bytes == -1 || bytes == 0 ) {
            return -1;
        }
        count += bytes;
    }

    return count;
}

int YMTcpSocket::Recv(char* buf, int len, int flags) {
    return recv(m_clientSock, buf, len, flags);
}

int YMTcpSocket::Close() 
{
	this->dispatchEvent(new YMSocketEvent(YMSocketEvent::EVENT_SOCKET_CLOSE, 0));
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

string YMTcpSocket::int2String(unsigned int val) {
	char buff[25];
	sprintf(buff, "0x%04X", val);
	return std::string(buff);
}

int YMTcpSocket::chars2Int(char chars[],int start) {
	int iVal = 0;
	char buff[10];
	iVal += (chars[start++]-48)*(int)pow(2.0,16);

	iVal += (chars[start++]-48)*(int)pow(2.0,8);

	iVal += (chars[start++]-48)*(int)pow(2.0,4);

	iVal += (chars[start++]-48)*(int)pow(2.0,0);
	return iVal;
}

void YMTcpSocket::int2Chars(char *chars, int val, int start) {
	char buff[10];
	sprintf(buff,"%04X",val);
	for(int i=0;i<4;i++){
		chars[start++] = buff[i];
	}
}

void YMTcpSocket::getKey(int cmd, int stamp , int needKeylen, char buffer[]) {
	int byteKey = (((needKeylen & cmd) + (stamp ^ cmd)) % 0xFF);
	for (int i = 0; i < needKeylen; i++) {
		cmd -=  (((byteKey * needKeylen) ^ stamp) % 0xFF);
		stamp -=  byteKey;
		byteKey = ((cmd % 0xFF));
		buffer[i] = byteKey;
	}
}


void YMTcpSocket::hasError() {
	this->dispatchEvent(new YMSocketEvent(YMSocketEvent::EVENT_SOCKET_ERROR, GetError()));
};

void YMTcpSocket::encrypt(char buffer[], int size, int cmd, int stamp) {
	int len = (size > 128) ? 128 : size;
	char keys[128];
	getKey(cmd, stamp, len, keys);
	for (int i = 0; i < len; i++) {
		buffer[i] = buffer[i] ^ keys[i];
	}
}

int YMTcpSocket::SendMsg(int cmd, YMSocketData *data) {
	char packBuffer[2048];
	memset(packBuffer,0,2048);
	int packSize;
	data->serializer(packBuffer, &packSize);
	int sendSize = packSize + 8;
	char *sedBuffer = (char *)malloc(sendSize);
	iStamp++;
	//encrypt(packBuffer+8, packSize, cmd, iStamp);
	int2Chars(sedBuffer, cmd, 0);
	int2Chars(sedBuffer, iStamp, 4);
	memcpy(sedBuffer + 8, packBuffer, packSize);
	int i = Send(sedBuffer, sendSize, 0);

	const char tName[20] = "后台发送数据：";
	//YMDebugCmd::shareYMDebugCmd()->AddsendMsg(tName, cmd, iStamp, data->getJsonString().c_str());
	CCLOG("%s 0x%04X, %d, %s", YMIconv::GBK2UTF(tName).c_str(), cmd, iStamp, data->getJsonString().c_str());

	free(sedBuffer);

	return iStamp;
}

int YMTcpSocket::createThread(){
	int errCode = pthread_attr_init(&threadAttr);
	CCLOG("pthread_attr_init error code:%d", errCode);
	if (errCode == 0) {
		errCode = pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
		CCLOG("pthread_attr_setdetachstate error code:%d", errCode);
		if (errCode != 0) {
			pthread_attr_destroy(&threadAttr);
		}
		errCode = pthread_create(&threadId, &threadAttr, &YMTcpSocket::Handler, NULL);
		CCLOG("pthread_create error code:%d", errCode); 
	}
	return errCode;
}

void YMTcpSocket::schaleHandler(){
	Init();
	m_ins.Create(AF_INET, SOCK_STREAM, /*IPPROTO_IP*/0);

	
	YMTcpSocket::getIns()->Bind(6666);
	int iconnect=0;
	YMTcpSocket::getIns()->Listen();
	while (1) {
		char fromip[200];
		m_Server[iconnect] = YMTcpSocket::getIns()->Accept(*YMTcpSocket::getIns(),fromip);
		//YMTcpSocket::getIns()->tcpSocket->setSocket(m_Server[iconnect]);
		if(m_Server[iconnect]>0){
			//发送数据
			CCLOG("have connect %d times!",iconnect);
			iconnect++;
			if(iconnect>m_maxConnect){
				CCLOG("It is over connect times");
				YMSocketData sd1=YMSocketData();
				sd1["w"]="It is over connect times!";
				YMTcpSocket::getIns()->SendMsg(0x0101,&sd1);
			}else{
				createThread();
			}
		}else{
			CCLOG("accept faild with error %d",WSAGetLastError());
		}
	}
	YMTcpSocket::getIns()->Close();
}

void* YMTcpSocket::Handler(void *arg) {
	SOCKET hsock=YMTcpSocket::getIns()->m_sock;
	if(hsock!=-1){
		CCLOG("begin recv!!!");
	}
	char recvmsg[2048];
	unsigned long recv_sz=2048;
	memset(recvmsg,0,recv_sz);
	while(1){
		int num=YMTcpSocket::getIns()->Recv(recvmsg, recv_sz, 0);
		if(num>0){
			int cmd=YMTcpSocket::getIns()->chars2Int(recvmsg,0);
			int iStamp=YMTcpSocket::getIns()->chars2Int(recvmsg,4);
			
			//encrypt(recvmsg+8,recv_sz,cmd,iStamp);

			YMSocketDataEvent *sEvent = new YMSocketDataEvent(YMTcpSocket::getIns()->int2String(cmd), YMTcpSocket::getIns()->iStamp, recvmsg+8, strlen(recvmsg)-8);
			YMServerReply::getIns()->sendData(cmd,sEvent);
		}
	}
}