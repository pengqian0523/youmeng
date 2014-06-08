#ifndef _YMLabelTTF_H
#define _YMLabelTTF_H
#include "cocos-ext.h"
#include "cocos2d.h" 

USING_NS_CC;
using namespace cocos2d::ui;
enum Type
{
	kTypeNone,
	kType_UnderLine,
	kType_DeleteLine,
	kType_Button,
	kType_Image
};

class YMLabelTTF : public CCLabelTTF{
public:
	YMLabelTTF();
	~YMLabelTTF();
	static YMLabelTTF *create(const char *string, const char *fontName, float fontSize);
	bool initWithString(const char *string, const char *fontName, float fontSize);
	virtual void setString(const char *label,int prevWidth=0);
	virtual void setFontName(const char *fontName);
	virtual void setFontSize(float fontSize);
	virtual void setColor(const ccColor3B& color3);
	virtual void setDimensions(const CCSize& sz);
	void setLineType(Type type);
	Type getLineType() const;
	void setSrcFontWidth(CCSize pSrcFontsz){m_SrcFontsz = pSrcFontsz;}
	void setTouchEnabled(bool IsTouchEnabled);
	void setButtonFile(std::string pNormal,std::string pressed,std::string disabled);
	void addTouchEventCallBack(CCObject *target,SEL_TouchEvent selector);
	void loadImageFile(std::string imageFile);
private:
	void reInitLine(int prevWidth=0);
private:
	Type m_eType;
	//CCSprite *m_pOther;
	std::vector<CCSprite *>m_pOthers;
	std::vector<UIButton *>m_pButtons;
	UIImageView *m_pImage;
	int m_prevWidth;
	std::string m_imageFile;
	CCSize m_SrcFontsz;
	CCPoint m_prevPos;
};

#endif