#include "LogoScene.h"
#include "YMLabelTTF.h"
#include "YMCommonLabelTTF.h"
#include "gameLayer.h"
#include "YMSocketData.h"
#include "YMClientSocket.h"
#include "myClient.h"

#include "ServerSocket.h"
#define FONT_SZ "fonts/arial.ttf"

LogoScene::LogoScene(){
	flag=0;
}

LogoScene::~LogoScene(){
	CCLOG("1111");
}

void LogoScene::onEnter(){
	CCScene::onEnter();

	UILayer *pLayer=UILayer::create();
	UILayout *mylayout = dynamic_cast<UILayout *>(GUIReader::shareReader()->widgetFromJsonFile("logoscene/logoscene.json"));
	this->addChild(pLayer);
	pLayer->addWidget(mylayout);
	
	char sedBuffer[1024];
	char packBuffer[1024];
	sprintf(packBuffer,"123456789");
	sedBuffer[12]=0;
	memcpy(sedBuffer + 13, packBuffer, strlen(packBuffer)+1);

	UIButton *btn=UIButton::create();
	btn->loadTextureNormal("logoscene/btn-test-0.png");
	btn->loadTexturePressed("logoscene/btn-test-1.png");
	btn->setTouchEnabled(true);
	mylayout->addChild(btn);
	btn->addTouchEventListener(this,toucheventselector(myClient::btncallback));
	btn->setPosition(ccp(200,200));

	myClient *pmyClient = myClient::create();
	this->addChild(pmyClient,10);
}

void LogoScene::hand0x0101Handler(YMEvent *ymevt){
	CCLOG("aaaaa");
}