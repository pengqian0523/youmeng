#include "YMCursorTextField.h"

const static float DELTA = 0.5f;

using namespace cocos2d;
using namespace std;

YMCursorTextField::YMCursorTextField()
{
	CCTextFieldTTF();

	m_pCursorSprite = NULL;
	m_pCursorAction = NULL;

	m_pInputText = NULL;
	codeNumType = NULL;
}

YMCursorTextField::~YMCursorTextField()
{
	CC_SAFE_DELETE(m_pInputText);
	CC_SAFE_DELETE_ARRAY(codeNumType);
}

void YMCursorTextField::onEnter()
{
	CCTextFieldTTF::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
	this->setDelegate(this);
}

YMCursorTextField * YMCursorTextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
	YMCursorTextField *pRet = new YMCursorTextField();
	if(pRet && pRet->initWithString("", fontName, fontSize))
	{
		pRet->autorelease();
		if (placeholder)  
		{
			pRet->setPlaceHolder(placeholder);
		}
		pRet->init();
		pRet->initCursorSprite(fontSize);
		pRet->setHorizontalAlignment(kCCTextAlignmentLeft);
		return pRet;
	}
	//CC_SAFE_DELETE(pRet);
	return NULL;
}

bool YMCursorTextField::init(){
	this->inputFrameWidth = 400;
	this->inputMaxLength = 100;
	
	this->nLenCount = 0;
	this->codeNumType = new int[300];
	this->codeCur = 0;
	this->startCur = 0;
	this->endCur = 0;
	inpuText = "";
	return true;
}
void YMCursorTextField::initCursorSprite(const int mHeight)
{
	// ��ʼ�����
	const int column = 4;
	const int nHeight = (const int)mHeight;
	int pixels[25][column];
	for (int i = 0; i<25; i++) {
		for (int j=0; j<column; j++) {
			pixels[i][j] = 0xffffffff;
		}
	}
	CCTexture2D *texture = new CCTexture2D();
	texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(column, nHeight));
	
	m_pCursorSprite = CCSprite::createWithTexture(texture);
	CCSize winSize = getContentSize();
	m_cursorPos = ccp(0, winSize.height/2.0f);
	m_pCursorSprite->setPosition(m_cursorPos);
	this->addChild(m_pCursorSprite);
 	m_pCursorSprite->setVisible(true);
 	m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));
 	m_pCursorSprite->runAction(m_pCursorAction);
	m_pInputText = new std::string();
}

bool YMCursorTextField::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{    
	m_beginPos = pTouch->getLocation();
	return true;
}

CCRect YMCursorTextField::getRect()
{
	CCSize size = getContentSize();
	return  CCRectMake( 0, -size.height/2.0, inputFrameWidth, size.height);  
}

//��ȡ���������
const char* YMCursorTextField::getInputText(){
	const char* text = m_pInputText->c_str();
	return text;
}

//�������������
void YMCursorTextField::setInpuntText(char* text){
	*m_pInputText = "";
    setString(text);
	m_pCursorSprite->setPositionX(0);
	CC_SAFE_DELETE_ARRAY(codeNumType);
	codeNumType = new int[300];
	codeCur = 0;
	startCur = 0;
	endCur = 0;
	inpuText = "";
}

//����������� һ���ַ�����ȳ���������� �ַ������Զ���������
void YMCursorTextField::setInputWidth(float width){
	this->inputFrameWidth = width;
}

//�����������ʾ������ַ�����Unicode
void YMCursorTextField::setInputMaxLength(float length){
	this->inputMaxLength = length;
}

//�жϵ���¼����Ƿ���Ӧ�������Χ��
bool YMCursorTextField::isInTextField(cocos2d::CCTouch *pTouch)
{

	return getRect().containsPoint(convertTouchToNodeSpaceAR(pTouch));
}

void YMCursorTextField::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint endPos = pTouch->getLocation();
	// �ж��Ƿ�Ϊ����¼�
	if (::abs(endPos.x - m_beginPos.x) > DELTA || 
		::abs(endPos.y - m_beginPos.y)) 
	{
		// ���ǵ���¼�
		m_beginPos.x = m_beginPos.y = -1;
		return;
	}

	// �ж��Ǵ����뷨���ǹر����뷨
	isInTextField(pTouch) ? openIME() : closeIME();
}

//�����ֻ�����ʱ��Ӧ�¼�
bool YMCursorTextField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
	if (m_pInputText->empty()) {
		return false;
	}
	m_pCursorSprite->setPositionX(getContentSize().width);
	return false;
}

//�����������ʱ��Ӧ
//@param pSender �����¼�����
//@param text ��������
//@param  �����ֽڳ���
bool YMCursorTextField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
{
	std::string sText = m_pInputText->c_str();
	wchar_t* wText = new wchar_t[1024];
	char t[1024];
	memset(t,0,1024);
	strcpy(t,sText.c_str());
	//���ַ���ת��ΪUnicode��������Unicode�ַ�����
	int cou = Utf82Unicode(wText,1024,t,sText.length());
	//���ַ����������涨ֵ ��������
	if(cou >= inputMaxLength) return true;
	//���λس�����
	if(text[0] == '\n'){
		closeIME();
		return true;
	}
	//��������������
	m_pInputText->append(text);

	//����
	CCLabelTTF* ttf = CCLabelTTF::create(text,"Verdana-Bold",26);
	float teWidth = ttf->getContentSize().width;
	
	//�����ǰ�ַ������
	inpuText.append(text);
	//��ǰ�ַ��ĳ���
	codeNumType[codeCur++] = nLen;
	std::string* localText = m_pInputText;
	setString(m_pInputText->c_str());
	//������ַ����ĳ��ȴ���ָ�����
	if(getContentSize().width > inputFrameWidth){
	    //���ڣ���ȡ�ַ�����ֱ���ַ����ĳ���С��ָ�����Ϊֹ
		setString(inpuText.c_str());
		while(getContentSize().width > inputFrameWidth){
	    	int nnLen = nLen;
	    	if(codeNumType[startCur] == 1){
	    		nnLen = 1;
	    	}
    		if(codeNumType[startCur] == 3){
	    		nnLen = 3;
    		}
    		startCur++;
	    	nLenCount += nnLen;
	        float gap = localText->size() - nLenCount;
			inpuText = localText->substr(nLenCount,gap);
			setString(inpuText.c_str());
			float coWidth = getContentSize().width;
		}
	}
	else{
	    //С�ڣ�ֱ��������ʾ���ַ���
		nLenCount = 0;
		startCur = 0;
		setString(m_pInputText->c_str());
	}
	//���ù��λ��
	m_pCursorSprite->setPositionX(getContentSize().width);
	CC_SAFE_DELETE_ARRAY(wText);
	return true;
}


//�����������ʱ��Ӧ
//@param pSender �����¼�����
//@param text ɾ������
//@param  �����ֽڳ���
bool YMCursorTextField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
{
    //�����ַ������ȼ�ȥnLen�ֽڳ�
	m_pInputText->resize(m_pInputText->size() - nLen);
	//��ǰ�ַ�����һ
	codeNumType[codeCur--] = 0;
	std::string* localText = m_pInputText;
	setString(m_pInputText->c_str());
	if(getContentSize().width > inputFrameWidth){
		CCLOG("%f",getContentSize().width);
	    //����ָ����ȣ���ȡ�ַ�����ֱ���ַ�������С��ָ�����
		while(getContentSize().width > inputFrameWidth){
	    	int nnLen = nLen;
	    	if(codeNumType[startCur - 1] == 1){
	    		nnLen = 1;
	    	}
	    	if(codeNumType[startCur - 1] == 3){
	    		nnLen = 3;
	    	}
	    	nLenCount -= nnLen;
	    	startCur--;
    		if(startCur <=0)
	    		startCur = 0;
	    	if(nLenCount <=0 )
	    		nLenCount = 0;
	    	float gap = localText->size() - nLenCount;
	    	const std::string text = localText->substr(nLenCount,gap);
	    	setString(text.c_str());
			inpuText = text;
		}
	}
	else{
		nLenCount = 0;
		startCur = 0;
		setString(m_pInputText->c_str());
	}
	//���ù��λ��
	m_pCursorSprite->setPositionX(getContentSize().width);
	if (m_pInputText->empty()) {
		m_pCursorSprite->setPositionX(0);
	}

	return true;
}

bool YMCursorTextField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
	return false;
}

void YMCursorTextField::openIME()
{
	m_pCursorSprite->setVisible(true);
	this->attachWithIME();
}

void YMCursorTextField::closeIME()
{
	m_pCursorSprite->setVisible(false);
	this->detachWithIME();
}

void YMCursorTextField::onExit()
{
	CCTextFieldTTF::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
int  YMCursorTextField::Utf82Unicode(wchar_t* out,  int outsize , char* in,int insize)  
{  
    //-------------------------------------------------------------------------------------------  
    //������Ч���ж�  
    if(out == NULL || in == NULL || insize<0)  
    {  
        return -1;  
    }  
	int typeCount = 0;
    int totalNum = 0;  
    char *p = in;  
    for(int i=0;i<insize;i++)  
    {  
        if (*p >= 0x00 && *p <= 0x7f)//˵�����λΪ'0'������ζ��utf8����ֻ��1���ֽڣ�  
        {  
            p++;  
            totalNum += 1;  
        }  
        else if ((*p & (0xe0))== 0xc0)//ֻ���������λ���������λ�ǲ���110�����������ζ��utf8������2���ֽڣ�  
        {  
            p++;  
            p++;  
            totalNum += 1;  
        }  
        else if ((*p & (0xf0))== 0xe0)//ֻ���������λ���������λ�ǲ���1110�����������ζ��utf8������3���ֽڣ�  
        {  
            p++;  
            p++;  
            p++;  
            totalNum += 1;  
        }  
    }  
    if( outsize < totalNum )//������Ч���жϣ�  
    {  
        return -1;  
    }  
    //------------------------------------------------  
        int resultsize = 0;  
  
        p = in;  
        char* tmp = (char *)out;  
        while(*p)  
        {  
            if (*p >= 0x00 && *p <= 0x7f)//˵�����λΪ'0'������ζ��utf8����ֻ��1���ֽڣ�  
            {  
                *tmp = *p;  
                tmp++;  
                //*tmp = '/0';  
                tmp++;  
                resultsize += 1;  
            }  
            else if ((*p & 0xe0)== 0xc0)//ֻ���������λ���������λ�ǲ���110�����������ζ��utf8������2���ֽڣ�  
            {  
                wchar_t t = 0;  
                char t1 = 0;  
                char t2 = 0;  
  
                t1 = *p & (0x1f);//��λ�ĺ�5λ����ȥ����ͷ����110�����־λ��  
                p++;  
                t2 = *p & (0x3f);//��λ�ĺ�6λ����ȥ����ͷ����10�����־λ��  
  
                *tmp = t2 | ((t1 & (0x03)) << 6);  
                tmp++;  
                *tmp = t1 >> 2;//�����䱣������λ  
                tmp++;  
                resultsize += 1;  
            }  
            else if ((*p & (0xf0))== 0xe0)//ֻ���������λ���������λ�ǲ���1110�����������ζ��utf8������3���ֽڣ�  
            {  
                wchar_t t = 0;  
                wchar_t t1 = 0;  
                wchar_t t2 = 0;  
                wchar_t t3 = 0;  
                t1 = *p & (0x1f);  
                p++;  
                t2 = *p & (0x3f);  
                p++;  
                t3 = *p & (0x3f);  
  
                *tmp = ((t2 & (0x03)) << 6) | t3;  
                tmp++;  
                *tmp = (t1 << 4) | (t2 >> 2);  
                tmp++;  
                resultsize += 1;  
            }  
            p++;  
        }  
        /*�����ǽ����������������򿪴˶Σ�  
        *tmp = '/0';  
        tmp++;  
        *tmp = '/0';  
        resultsize += 2;  
        */  
        return resultsize;  
}  