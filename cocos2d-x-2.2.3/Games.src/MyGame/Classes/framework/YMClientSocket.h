#ifndef __YMClientSocket_H__
#define __YMClientSocket_H__

#ifdef WIN32
#include "platform/third_party/win32/pthread/pthread.h"
#else
#include <pthread.h>
#endif
//#include "../cocos2dx/platform/third_party/emscripten/libz/zlib.h"
#include <stdlib.h>
#include "YMEventDispatcher.h"
#include "YMTcpSocket.h"
#include "YMSocketEvent.h"
#include "YMSocketDataEvent.h"
#include "YMSocketData.h"

class YMClientSocket: public YMEventDispatcher {
public:
    static YMClientSocket *getIns();
private:
    static YMClientSocket m_ins;
private:
    YMClientSocket();
    virtual ~YMClientSocket();
public:
    int connect(const char* ip, unsigned short port);
    int close();
    int SendMsg(int cmd, YMSocketData *data);
    int GetError();
	void addDataPacketListener(unsigned int eventType, CCObject *target, EventHandler handler);
	void removeDataPacketListener(unsigned int eventType, CCObject *target, EventHandler handler);
private:
    static void *threadHandler(void *arg);
public:
	bool isConnected;
private:
    unsigned int iStamp;
    pthread_t threadId;
    pthread_attr_t threadAttr;
    YMTcpSocket *tcpSocket;
    int chars2Int(char chars[],int start);
    void int2Chars(char *chars, int val, int start = 0);
	string int2String(unsigned int val);
    int Recv(char* buf, int len, int flags = 0);
    void dataIn(char buff[], unsigned int size);
    void getKey(int cmd, int stamp, int needKeylen, char buffer[]);
    void encrypt(char buffer[], int size, int cmd, int stamp);
	void hasError();
};

#endif