#ifndef _YMCommonLabelTTF_H
#define _YMCommonLabelTTF_H
#include "cocos-ext.h"
#include "cocos2d.h" 
#include "YMLabelTTF.h"

class YMCommonLabelTTF : public CCLayer{
public:
	YMCommonLabelTTF();
	~YMCommonLabelTTF();
	static YMCommonLabelTTF *create(std::vector<YMLabelTTF *>pTTFs,float maxWidth);
	bool init(std::vector<YMLabelTTF *>pTTFs,float maxWidth);
	int getLengthOfString(const char* string,const char *fontname,int fontsz);
private:
	float m_maxWidth;
	float m_PrevWidth;//前面的宽度
	float m_LeftWidth;
	YMLabelTTF *m_PrevLabelTTF;
private:
	
};

#endif