#include "myClient.h"
#include "YMLabelTTF.h"
#include "YMCommonLabelTTF.h"
#include "gameLayer.h"
#include "YMSocketData.h"
#include "YMTcpSocket.h"
#include "YMClientSocket.h"
#include "Teacher_Level.h"

#define MAXSIZE 2048
#define PORT 6666
#define DESIP  "127.0.0.1"

#define FONT_SZ "fonts/arial.ttf"

myClient::myClient(){
	
}

myClient::~myClient(){
	CCLOG("free 123");
}

myClient *myClient::create(){
	myClient *pRet = new myClient();
	if(pRet && pRet->init()){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool myClient::init(){
	Teacher_Level *pTeacher_Level=Teacher_Level::create();


// 	SOCKET sockfd;
// 	struct sockaddr_in des_addr;
// 	char sendmsg[MAXSIZE], recvmsg[MAXSIZE]; 
// 	YMSocketData data("{\"uname\":\"root\",\"upass\":\"pass\"}");
// 	if (!YMClientSocket::getIns()->getIns()->isConnected) {
// 		if (YMClientSocket::getIns()->connect("192.168.1.104", PORT) != SOCKET_ERROR) {
// 			YMClientSocket::getIns()->addDataPacketListener(0x0101, this, Event_Handler(myClient::hand0x0101Handler));
// 			YMClientSocket::getIns()->SendMsg(0x0101, &data);
// 		}
// 	} 
// 	YMClientSocket::getIns()->addDataPacketListener(0x0102, this, Event_Handler(myClient::hand0x0102Handler));
// 	YMClientSocket::getIns()->SendMsg(0x0102, &data);
	return true;
}

void myClient::btncallback(CCObject *obj,TouchEventType type){
	if(type==TOUCH_EVENT_ENDED){
		CCLOG("test");
		YMClientSocket::getIns()->addDataPacketListener(0x0103, this, Event_Handler(myClient::hand0x0102Handler));
		YMSocketData data("{\"id\":\"33333333333333333\"}");
		YMClientSocket::getIns()->SendMsg(0x0103, &data);
	}
}

void myClient::hand0x0101Handler(YMEvent *evt){
	YMSocketDataEvent *sEvent=(YMSocketDataEvent*)evt;
	CCLOG("1:%s",sEvent->getBody().getJsonString().c_str());
}

void myClient::hand0x0102Handler(YMEvent *evt){
	YMSocketDataEvent *sEvent=(YMSocketDataEvent*)evt;
	CCLOG("2:%s",sEvent->getBody().getJsonString().c_str());
}