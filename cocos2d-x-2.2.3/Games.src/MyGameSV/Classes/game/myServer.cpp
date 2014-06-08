#include "myServer.h"
#include "YMTcpSocket.h"

myServer::myServer(){
	
}

myServer::~myServer(){
	
}

myServer *myServer::create(){
	myServer *pRet = new myServer();
	if(pRet && pRet->init()){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool myServer::init(){
	YMTcpSocket::getIns()->schaleHandler();
	return 0;
}