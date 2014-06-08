#include "YMCommonLabelTTF.h"


YMCommonLabelTTF::YMCommonLabelTTF():
m_maxWidth(0),
m_PrevWidth(0),
m_PrevLabelTTF(NULL)
{

}

YMCommonLabelTTF::~YMCommonLabelTTF(){

}

YMCommonLabelTTF *YMCommonLabelTTF::create(std::vector<YMLabelTTF *>pTTFs,float maxWidth)
{
	YMCommonLabelTTF *pLable = new YMCommonLabelTTF();
	if( pLable && pLable->init(pTTFs,maxWidth) )
	{
		pLable->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pLable);
	}
	return pLable;
}

bool YMCommonLabelTTF::init(std::vector<YMLabelTTF *>pTTFs,float maxWidth){
	m_maxWidth=maxWidth;
	for(int i=0;i<pTTFs.size();i++){
		YMLabelTTF *pTTF=pTTFs.at(i);
		CCSize oldsz=pTTF->getContentSize();
		pTTF->setAnchorPoint(ccp(0,1));
		pTTF->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
		
		if(m_PrevWidth!=0){
			int count=m_PrevWidth/oldsz.width*strlen(pTTF->getString());
			char buff[2048];
			memset(buff,0,2048);
			memset(buff,' ',count+1);
			if(buff){
				std::string content=buff;
				content+=pTTF->getString();
				pTTF->setString(content.c_str(),getLengthOfString(buff,pTTF->getFontName(),pTTF->getFontSize()));
			}
			
		}
		CCSize prevOldsz=pTTF->getContentSize();
		pTTF->setSrcFontWidth(oldsz);
		pTTF->setDimensions(CCSize(m_maxWidth,0));
		CCSize newsz=pTTF->getContentSize();

		this->addChild(pTTF);
		if(m_PrevLabelTTF!=NULL){
			pTTF->setPosition(ccp(0,m_PrevLabelTTF->getPositionY()-m_PrevLabelTTF->getContentSize().height+oldsz.height));
		}
		int linecount=newsz.height/prevOldsz.height;
		m_PrevWidth = pTTF->getFontSize()*1.5+prevOldsz.width-(newsz.height/prevOldsz.height-1)*m_maxWidth;

		m_PrevLabelTTF=pTTF;
		
		CCLOG("(%f,%f)  (%f,%f)",oldsz.width,oldsz.height,newsz.width,newsz.height);
	}
	return true;
}

int YMCommonLabelTTF::getLengthOfString(const char* string,const char *fontname,int fontsz){
	YMLabelTTF *label=YMLabelTTF::create(string,fontname,fontsz);
	return label->getContentSize().width;
}