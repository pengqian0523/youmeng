#include "LogoScene.h"
#include "YMSocketData.h"
#include "myServer.h"

LogoScene::LogoScene(){
	
}

LogoScene::~LogoScene(){

}

void LogoScene::onEnter(){
	CCScene::onEnter();
	myServer *pmyServer = myServer::create();
}