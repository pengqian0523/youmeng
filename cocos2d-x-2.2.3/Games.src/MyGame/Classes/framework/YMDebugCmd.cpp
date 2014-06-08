#include "YMDebugCmd.h"
#include "AppMacros.h"
#include "YMClientSocket.h"
#include "YMDebugUserData.h"

#define FONT_NAME ("fonts/FZ.ttf")

YMDebugCmd *YMDebugCmd::m_shareYMDebugCmd=NULL;

YMDebugCmd::YMDebugCmd():
m_tmpStr("")
{
	m_inputLabel=NULL;
	m_connect=NULL;
	m_close=NULL;
	m_showb=false;
	m_isShow=true;
	m_pLayer=NULL;
	m_msg=new Msg;
	m_preHeight=0;
	m_bLock = false;
}

YMDebugCmd::~YMDebugCmd()
{
	this->unscheduleUpdate();
	m_shareYMDebugCmd=NULL;
	
	CC_SAFE_DELETE(m_msg);
}

YMDebugCmd * YMDebugCmd::shareYMDebugCmd()
{
	if(m_shareYMDebugCmd==NULL)
	{
		m_shareYMDebugCmd=new YMDebugCmd();
		m_shareYMDebugCmd->init();
		m_shareYMDebugCmd->autorelease();
	}
	return m_shareYMDebugCmd;
}


bool YMDebugCmd::init(){
	this->scheduleUpdate();
	return true;
}

void YMDebugCmd::openCMD(){
	if(m_showb==false&&YMDebugUserData::shareYMDebugUserData()->getDebugValue()){
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(YMDebugCmd::MsgListen),"YMDebugCmd",NULL);
		m_showb=true;
		m_touchcount=0;
		m_pLayer = UILayer::create();  
		UILayout *myLayerout= dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/debug.json"));
		m_pLayer->addWidget(myLayerout);
		this->addChild(m_pLayer);
		CCSize sz=CCDirector::sharedDirector()->getWinSize();
		m_pLayer->setPosition(ccp(sz.width/2.0f-myLayerout->getContentSize().width/2.0f,sz.height/2.0f-myLayerout->getContentSize().height/2.0f));

		UIButton *pSend=getUIButtonFromJson("send_btn",toucheventselector(YMDebugCmd::sendCallBack));

		getUIButtonFromJson("close_cmd",toucheventselector(YMDebugCmd::closecmd));

		m_InputYMCursorTextField = YMCursorTextField::textFieldWithPlaceHolder(YMIconv::GBK2UTF("请输入发送内容").c_str(), TITLE_FONT_NAME, FONTSZ);
		m_InputYMCursorTextField->setAnchorPoint(ccp(0,0.5));
		UILayout *inputbg=dynamic_cast<UILayout*>(m_pLayer->getWidgetByName("input_layer"));
		CCSize inputsz=inputbg->getSize();
		inputbg->addNode(m_InputYMCursorTextField);
		m_InputYMCursorTextField->setPosition(ccp(FONTSZ/2.0f,inputsz.height-m_InputYMCursorTextField->getContentSize().height));  
		m_InputYMCursorTextField->setInputWidth(inputsz.width*0.8f);
		m_InputYMCursorTextField->setColorSpaceHolder(ccc3(100,100,100));     
		m_InputYMCursorTextField->setColor(ccc3(0,255,0));  

		m_CmdYMCursorTextField = YMCursorTextField::textFieldWithPlaceHolder("0x0101", TITLE_FONT_NAME, FONTSZ);
		m_CmdYMCursorTextField->setAnchorPoint(ccp(0,0.5));
		UILayout *cmdbg=dynamic_cast<UILayout*>(m_pLayer->getWidgetByName("cmd"));
		CCSize cmdsz=cmdbg->getSize();
		cmdbg->addNode(m_CmdYMCursorTextField);
		m_CmdYMCursorTextField->setPosition(ccp(0,cmdsz.height/2.0f));  
		m_CmdYMCursorTextField->setInputWidth(cmdsz.width);
		m_CmdYMCursorTextField->setColorSpaceHolder(ccc3(100,100,100));    
		m_CmdYMCursorTextField->setColor(ccc3(0,255,0));
		UILayout *layerout=dynamic_cast<UILayout *>(m_pLayer->getWidgetByName("output_layer"));

		getUIButtonFromJson("clear_btn",toucheventselector(YMDebugCmd::clear));
		m_prePos=ccp(0,layerout->getContentSize().height);
	}

}

void YMDebugCmd::closecmd(CCObject *obj, TouchEventType type){
	if(type==TOUCH_EVENT_ENDED){
		
		this->removeAllChildrenWithCleanup(true);
		m_shareYMDebugCmd=NULL;
		m_showb=false;
	}
}

UIButton* YMDebugCmd::getUIButtonFromJson(const char* ButtonName,SEL_TouchEvent pEvent){
	UIButton* pBtn = dynamic_cast<UIButton*>(m_pLayer->getWidgetByName(ButtonName));   
	CCAssert(pBtn != NULL,"");
	pBtn->addTouchEventListener(this,pEvent); 
	pBtn->setTouchEnabled(true);
	
	return pBtn;
}

UILabel* YMDebugCmd::getUILabelFromJson(const char* LayeroutName){
	UILabel* pLabel = dynamic_cast<UILabel*>(m_pLayer->getWidgetByName(LayeroutName));   
	CCAssert(pLabel != NULL,"");
	return pLabel;
}

long YMDebugCmd::hex2int(const string& hexStr)
{
	char *offset;
	if(hexStr.length() > 2)
	{
		if(hexStr[0] == '0' && hexStr[1] == 'x')
		{
			return strtol(hexStr.c_str(), &offset, 0);
		}
	}
	return strtol(hexStr.c_str(), &offset, 16);
}

void YMDebugCmd::sendCallBack(CCObject *obj, TouchEventType type){
	if(type==TOUCH_EVENT_ENDED){
		if(m_CmdYMCursorTextField){
			std::string cmdstr=m_CmdYMCursorTextField->getString();
			int cmd= hex2int(cmdstr.c_str());
			char sdmsg[512];
			if(m_InputYMCursorTextField){
				sprintf(sdmsg,"%s",m_InputYMCursorTextField->getString());
			}
			YMSocketData sd(sdmsg);

			YMClientSocket::getIns()->SendMsg(cmd, &sd);
		}
	}
}

void YMDebugCmd::createInputList(std::string cmd,std::string inputstr){
	UILayout *input=dynamic_cast<UILayout*>(m_pLayer->getWidgetByName("input_layer"));
	if(m_inputLabel==NULL){
		m_inputLabel = CCLabelTTF::create(inputstr.c_str(), "Arial", 16);
		m_inputLabel->setAnchorPoint(ccp(0,0.5));
		input->addNode(m_inputLabel,2);
	}else{
		m_inputLabel->setString(inputstr.c_str());
	}	
	m_inputLabel->setPosition(ccp(0,input->getContentSize().height/2.0));
}

void YMDebugCmd::ShowOutput(std::string str){
	if(m_pLayer){
		UILayout *layerout=dynamic_cast<UILayout *>(m_pLayer->getWidgetByName("output_layer"));
		CCLabelTTF *pShowOutput=CCLabelTTF::create(str.c_str(),FONT_NAME,16,CCSize(layerout->getContentSize().width*0.95,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
		layerout->addNode(pShowOutput);
		pShowOutput->setAnchorPoint(ccp(0,0.5));
		pShowOutput->setPositionY(m_prePos.y-m_preHeight/2.0f-pShowOutput->getContentSize().height/2.0f);
		m_prePos.y=pShowOutput->getPositionY();
		if(m_prePos.y-pShowOutput->getContentSize().height/2.0<0){
			layerout->setPositionY(layerout->getPositionY()+pShowOutput->getContentSize().height);
		}
		m_preHeight=pShowOutput->getContentSize().height;
	}
}

void YMDebugCmd::AddsendMsg(const char * tname, int cmd,unsigned int iStamp, std::string str){
	char buffer[2560];
	sprintf(buffer,"%s 0x%04X, %d, %s",YMIconv::GBK2UTF(tname).c_str(), cmd, iStamp, str.c_str());
	m_tmpStr = buffer;
	if( !m_bLock ){
		m_msg->jsonstring=buffer;
		m_tmpStr.clear();
	}
	
}

void YMDebugCmd::AddDataIn(const char * tname, int cmd,unsigned int iStamp, std::string str){
	char buffer[2048];
	sprintf(buffer,"%s 0x%04X, %d,",YMIconv::GBK2UTF(tname).c_str(), cmd, iStamp);
	m_tmpStr = buffer + str;
	if( !m_bLock ){
		if(getShowbool()){ 
			m_msg->jsonstring=m_tmpStr;
			m_tmpStr.clear();
		}
	}
}

void YMDebugCmd::MsgListen(CCObject *obj){
	Msg *msg=(Msg *)obj;
	if(msg){
		ShowOutput(msg->jsonstring.c_str());
	}
}

void YMDebugCmd::update(float dt)
{
	if( m_msg->jsonstring.size() > 0)
	{
		m_bLock = true;
		CCNotificationCenter::sharedNotificationCenter()->postNotification("YMDebugCmd",(CCObject *)m_msg); 
		m_msg->jsonstring.clear();
		m_bLock = false;
	}
	if(m_tmpStr.size() > 0 )
	{
		m_msg->jsonstring = m_tmpStr;
		m_tmpStr.clear();
	}
}

void YMDebugCmd::clear(CCObject *obj, TouchEventType type){
	if(type==TOUCH_EVENT_ENDED){
		UILayout *layerout=dynamic_cast<UILayout *>(m_pLayer->getWidgetByName("output_layer"));
		layerout->removeAllNodes();
		m_prePos=ccp(0,layerout->getContentSize().height); 
		layerout->setPositionY(0);
	}
}