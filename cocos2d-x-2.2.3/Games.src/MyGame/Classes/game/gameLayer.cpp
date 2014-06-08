#include "gameLayer.h"
#include "cocos-ext.h"
#include "YMLabelTTF.h"
#include "YMCommonLabelTTF.h"

#define FONT_SZ "fonts/Thonburi.ttf"

using namespace cocos2d::ui;
using namespace extension;


Test::Test(){

}

Test::~Test(){

}
Test* Test::create(){
	Test *pRet=new Test();
	if(pRet){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}



gameLayer* gameLayer::m_shareGamelayer=NULL;
gameLayer::gameLayer(){
	
}

gameLayer::~gameLayer(){

}

void gameLayer::onEnter(){
	CCLayer::onEnter();
}

gameLayer* gameLayer::shareGameLayer(){
	if(m_shareGamelayer==NULL){
		m_shareGamelayer=new gameLayer();
		m_shareGamelayer->init();
	}
	return m_shareGamelayer;
}

bool gameLayer::init(){
	m_pTests=new CCArray(10);
	for(int i=0;i<10;i++){
		Test *pTest=Test::create();
		pTest->setPlace(i);
		char buff[100];
		sprintf(buff,"%d",i+1);
		pTest->setName(buff);
		m_pTests->addObject(pTest);
	}
	m_id=111;

	connectThreadStart();



	return true;
}

void* connectSocket(void* args)
{
	gameLayer::shareGameLayer()->sconnect("192.168.1.104", 8080);
	return NULL;
}

int gameLayer::connectThreadStart(){
	//    connect(GAMESERVER, CCString::create(GAMESERVER_PORT)->intValue());

	int errCode = 0;
	do{
		pthread_attr_t tAttr;
		errCode = pthread_attr_init(&tAttr);

		CC_BREAK_IF(errCode!=0);

		errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);

		if (errCode!=0) {
			pthread_attr_destroy(&tAttr);
			break;
		}

		errCode = pthread_create(&m_gameThread, &tAttr, connectSocket, this);

	}while (0);
	return errCode;
}

int gameLayer::sconnect(const char* ip, unsigned int port)
{
	CCLOG("Client begin connect IP: %s:%d ",ip,port);
	struct sockaddr_in sa;
	struct hostent* hp;

	hp = gethostbyname(ip);
	if(!hp){
		return -1;
	}
	memset(&sa, 0, sizeof(sa));
	memcpy((char*)&sa.sin_addr, hp->h_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(port);

	m_socketHandle = socket(sa.sin_family, SOCK_STREAM, 0);

	if(m_socketHandle < 0){
		CCLOG("failed to create socket");
		return -1;
	}
	if(::connect(m_socketHandle, (sockaddr*)&sa, sizeof(sa)) < 0){
		CCLOG("failed to connect socket");
		//::close(m_socketHandle);
		return -1;
	}

	CCLOG("Client connect sucess! ip: %s:%d" ,ip,port);

	CCNotificationCenter::sharedNotificationCenter()->postNotification(m_connectok.c_str(), NULL);
	return 0;
}

int chars2Int(const char chars[]) {
	int iVal = 0;
	for (int i = 0; i < 4; i++) {
		iVal += (chars[i] & 0xFF) << ((3 - i) * 8);
	}
	return iVal;
}

void* listenSocketData(void* obj)
{
	char buffer[5];
	string contents;
	int ret = 0;
	// 先接受4字节，获取服务返回长度
	bool rs = true;
	while(rs)
	{
		contents ="";
		ret = recv(gameLayer::shareGameLayer()->m_socketHandle,buffer,4,0);
		// 服务器关闭
		if(ret == 0)
		{
			//            CCLog(Error: server close);
			rs = false;
		}
		if(ret == 4)
		{
			buffer[4]='\0';
			int packetlen = chars2Int(buffer);
			CCLOG("packetlen %d",packetlen);
			char *buf=new char(packetlen);
			int rets = 0;
			while((ret = recv(gameLayer::shareGameLayer()->m_socketHandle,buf,packetlen-rets,0))>0)
			{
				contents.append(buf,ret);
				packetlen-=ret;
				if(packetlen<=0)
					break;
			}
			CCLog("recv content:%s",contents.c_str());
			CCString* str = CCString::create(/*Utils::getUnPackMsg(contents)*/contents.c_str());
			char receivedata[2048];
			CCNotificationCenter::sharedNotificationCenter()->postNotification(receivedata, str);
			CCLOG("%s",str);
			CCLOG("%s",receivedata);
			delete buf;
		}else {
			CCLog("Error: recv data Error %d",ret);
		}
	}
	return NULL;
}


void gameLayer::initReceiveThread(CCObject* obj)
{
	int errCode = 0;
	pthread_attr_t tAttr;
	errCode = pthread_attr_init(&tAttr);

	errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);

	if (errCode!=0) {
		pthread_attr_destroy(&tAttr);
	}else{
		errCode = pthread_create(&m_gameThread, &tAttr, listenSocketData, this);
	}
	if(errCode == 0){
		CCLOG("Receive Thread OK!!!");
	}else{
		CCLOG("Receive Thread Error!!!!");
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification(m_jointable.c_str(), NULL);
}

