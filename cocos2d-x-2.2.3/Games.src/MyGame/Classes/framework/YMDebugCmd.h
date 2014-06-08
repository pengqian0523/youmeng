#ifndef _YMDebugCmd_h  
#define _YMDebugCmd_h    
#include "cocos2d.h" 
#include "cocos-ext.h"
#include "YMClientSocket.h"
#include "YMCursorTextField.h"
#define  FONTSZ 20

using namespace cocos2d::ui;
using namespace cocos2d::extension;

//…Ë÷√±≥æ∞—’…´
#define DROPDOWNLIST_NORMAL_COLOR       ccc4(128, 128, 128, 100)
#define DROPDOWNLIST_SELECTED_COLOR     ccc4(200, 200, 200, 100)
#define DROPDOWNLIST_HIGHLIGHT_COLOR    ccc4(0, 0, 255, 100)

#define DROPDOWNLIST_NORMAL_COLOR3       ccc3(128, 128, 128)
#define DROPDOWNLIST_SELECTED_COLOR3     ccc3(200, 200, 200)
#define DROPDOWNLIST_HIGHLIGHT_COLOR3    ccc3(0, 0, 255)

struct Msg{
	std::string jsonstring;
};

class YMDebugCmd: public CCLayer
{  
private:
	UILayer *m_pLayer;
	
	int m_touchcount;
	CCLabelTTF* m_inputLabel;
	UIButton *m_connect;
	UIButton *m_close;
	YMCursorTextField *m_InputYMCursorTextField;
	YMCursorTextField *m_CmdYMCursorTextField;
	UIButton *m_openCMD;
	bool m_showb;
	static YMDebugCmd *m_shareYMDebugCmd;
	bool m_isShow;
	Msg *m_msg;
	std::string m_tmpStr;
	CCPoint m_prePos;
	float m_preHeight;
	bool m_bLock;
public:  
	YMDebugCmd();  
	~YMDebugCmd();  
	static YMDebugCmd* shareYMDebugCmd();  
	bool init();
	void createInputList(std::string cmd,std::string inputstr);
	void sendCallBack(CCObject *obj, TouchEventType type);
	long hex2int(const string& hexStr);
	UIButton* getUIButtonFromJson(const char* ButtonName,SEL_TouchEvent pEvent);
	UILabel* getUILabelFromJson(const char* LayeroutName);
	void ShowOutput(std::string str);
	void openCMD();
	bool getShowbool(){return m_showb;}
	void closecmd(CCObject *obj, TouchEventType type);
	void AddsendMsg(const char * tname, int cmd,unsigned int iStamp, std::string str);
	void AddDataIn(const char * tname, int cmd,unsigned int iStamp, std::string str);
	void MsgListen(CCObject *obj);
	virtual void update(float dt);
	void clear(CCObject *obj, TouchEventType type);
};  
#endif  
