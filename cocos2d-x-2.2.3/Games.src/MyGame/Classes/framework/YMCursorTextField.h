#ifndef CursorInputDemo_YMCursorTextField_h  
#define CursorInputDemo_YMCursorTextField_h    
#include "cocos2d.h"  
#define RECT_SZ getRect().size
#define CONTENT_SZ getContentSize()


USING_NS_CC;    
class YMCursorTextField: public CCTextFieldTTF, public CCTextFieldDelegate, public CCTouchDelegate  
{  
private:  
	// �����ʼλ��  
	CCPoint m_beginPos;  
	// ��꾫��  
	CCSprite *m_pCursorSprite;  
	// ��궯��  
	CCAction *m_pCursorAction;  
	// �������  
	CCPoint m_cursorPos;  
	//����򳤶�  
	float inputFrameWidth;  
	//�������������ַ���Unicode  
	float inputMaxLength;  
	int nLenCount;  
	int* codeNumType;    //ÿ���ַ���Ӧ���ֽ�����  
	int codeCur;         //��ǰ�ڼ����ַ�  
	int startCur;        //�п�ͷ�ַ��±�  
	int endCur;          //��ĩβ�±�  
	// �����������  
	std::string *m_pInputText;  
	std::string inpuText; //��ǰ���������  
public:  
	YMCursorTextField();  
	~YMCursorTextField();  
	 // static  
	static YMCursorTextField* textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);  
	// CCLayer  
	void onEnter();  
	void onExit();  
	bool init();  
	// ��ʼ����꾫��  
	void initCursorSprite(int nHeight);    
	// CCTextFieldDelegate  
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);  
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);  
	virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);  
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);  
	// CCLayer Touch  
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);  
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);  
	// �ж��Ƿ�����TextField��  
	bool isInTextField(CCTouch *pTouch);  
	// �õ�TextField����  
	CCRect getRect();
	// �����뷨  
	void openIME();  
	// �ر����뷨  
	void closeIME(); 
	const char* getInputText();  
	void setInpuntText(char* text);  
	void setInputWidth(float width);  
	void setInputMaxLength(float length);  
	CCSprite* getCursorSprite(){return m_pCursorSprite;}
	int Utf82Unicode(wchar_t* out,  int outsize , char* in,int insize);  
};  
#endif  
