#ifndef CursorInputDemo_YMCursorTextField_h  
#define CursorInputDemo_YMCursorTextField_h    
#include "cocos2d.h"  
#define RECT_SZ getRect().size
#define CONTENT_SZ getContentSize()


USING_NS_CC;    
class YMCursorTextField: public CCTextFieldTTF, public CCTextFieldDelegate, public CCTouchDelegate  
{  
private:  
	// 点击开始位置  
	CCPoint m_beginPos;  
	// 光标精灵  
	CCSprite *m_pCursorSprite;  
	// 光标动画  
	CCAction *m_pCursorAction;  
	// 光标坐标  
	CCPoint m_cursorPos;  
	//输入框长度  
	float inputFrameWidth;  
	//允许输入的最大字符数Unicode  
	float inputMaxLength;  
	int nLenCount;  
	int* codeNumType;    //每个字符对应的字节数量  
	int codeCur;         //当前第几个字符  
	int startCur;        //行开头字符下标  
	int endCur;          //行末尾下标  
	// 输入框总内容  
	std::string *m_pInputText;  
	std::string inpuText; //当前输入框内容  
public:  
	YMCursorTextField();  
	~YMCursorTextField();  
	 // static  
	static YMCursorTextField* textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);  
	// CCLayer  
	void onEnter();  
	void onExit();  
	bool init();  
	// 初始化光标精灵  
	void initCursorSprite(int nHeight);    
	// CCTextFieldDelegate  
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);  
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);  
	virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);  
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);  
	// CCLayer Touch  
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);  
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);  
	// 判断是否点击在TextField处  
	bool isInTextField(CCTouch *pTouch);  
	// 得到TextField矩形  
	CCRect getRect();
	// 打开输入法  
	void openIME();  
	// 关闭输入法  
	void closeIME(); 
	const char* getInputText();  
	void setInpuntText(char* text);  
	void setInputWidth(float width);  
	void setInputMaxLength(float length);  
	CCSprite* getCursorSprite(){return m_pCursorSprite;}
	int Utf82Unicode(wchar_t* out,  int outsize , char* in,int insize);  
};  
#endif  
