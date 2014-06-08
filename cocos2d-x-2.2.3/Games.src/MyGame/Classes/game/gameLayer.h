#ifndef _GameLayer_H
#define _GameLayer_H

#include "cocos2d.h"
using namespace  cocos2d;
using namespace std;

#ifdef WIN32
#include "platform/third_party/win32/pthread/pthread.h"
#else
#include <pthread.h>
#endif

class Test:public CCObject{
private:
	std::string m_name;
	int m_place;
public:
	Test();
	~Test();
	static Test* create();
	void setName(std::string pName){m_name = pName;}
	std::string getName(){return m_name;}
	void setPlace(int place){m_place = place;}
	int getPlace(){return m_place;}
};

class gameLayer : public CCLayer{
public:
	gameLayer();
	~gameLayer();
	void onEnter();
	static gameLayer* shareGameLayer();
	bool init();
	int getid(){return m_id;}
	int connectThreadStart();
	//void* connectSocket(void* args);
	int sconnect(const char* ip, unsigned int port);
	void initReceiveThread(CCObject* obj);
	//int chars2Int(char chars[]);
	//void* listenSocketData(void* obj);
	CCArray *m_pTests;
public:
	
	static gameLayer* m_shareGamelayer;
	int m_id;
	SOCKET m_socketHandle;
	std::string m_connectok;
	pthread_t m_gameThread;
	std::string m_jointable;
	std::string m_receivedata;
};

#endif