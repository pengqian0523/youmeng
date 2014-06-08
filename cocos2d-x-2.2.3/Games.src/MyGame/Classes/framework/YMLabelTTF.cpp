#include "YMLabelTTF.h"


YMLabelTTF::YMLabelTTF()
	:m_eType(kTypeNone),
	m_pImage(NULL),
	m_prevWidth(0)
{
	m_SrcFontsz=CCSize(0,0);
}

YMLabelTTF::~YMLabelTTF(){

}

YMLabelTTF *YMLabelTTF::create(const char *string, const char *fontName, float fontSize)
{
	YMLabelTTF *pLable = new YMLabelTTF;
	if( pLable && pLable->initWithString(string,fontName,fontSize) )
	{
		pLable->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pLable);
	}
	return pLable;
}

bool YMLabelTTF::initWithString(const char *string, const char *fontName, float fontSize)
{
	if( !CCLabelTTF::init() )
	{
		return false;
	}
	setString(string);
	setFontName(fontName);
	setFontSize(fontSize);

	return true;
}
void YMLabelTTF::setLineType(Type type)
{
	m_eType = type;

	reInitLine(m_prevWidth);
}

Type YMLabelTTF::getLineType() const
{
	return m_eType;
}

void YMLabelTTF::setString(const char *label,int prevWidth)
{
	CCLabelTTF::setString(label);
	m_prevWidth=prevWidth;
	reInitLine(prevWidth);
}

void YMLabelTTF::setFontName(const char *fontName)
{
	CCLabelTTF::setFontName(fontName);

	reInitLine(m_prevWidth);
}

void YMLabelTTF::setFontSize(float fontSize)
{
	CCLabelTTF::setFontSize(fontSize);

	reInitLine(m_prevWidth);
}

void YMLabelTTF::setColor(const ccColor3B& color3)
{
	CCLabelTTF::setColor(color3);

	reInitLine(m_prevWidth);
}

void YMLabelTTF::setDimensions(const CCSize& sz){
	CCLabelTTF::setDimensions(sz);

	reInitLine(m_prevWidth);
}

void YMLabelTTF::reInitLine(int prevWidth)
{
	if( m_eType == kTypeNone )
	{
		return;
	}
	
	for(int i=0;i<m_pOthers.size();i++){
		if(m_pOthers.at(i) != NULL) 
		{ 
			removeChild(m_pOthers.at(i), true); 
		}
	}
	m_pOthers.clear();
	for(int i=0;i<m_pButtons.size();i++){
		UIButton *pBtn=m_pButtons.at(i);
		if(pBtn !=NULL ){
			UILayer *pParent=(UILayer *)pBtn->getParent();
			this->removeChild(pParent,true);
		}
	}
	m_pButtons.clear();
	
	CCSize size = m_obRect.size; 
	
	if(m_SrcFontsz.width==0&&m_SrcFontsz.height==0){
		return;
	}
	unsigned int * pixels; 
	int capacity = static_cast<int>(size.width); 
	pixels = new unsigned int [capacity]; 
	unsigned int color = getOpacity() << 24 | getColor().b << 16 | getColor().g << 8 | getColor().r; 

	for(int i = 0; i < capacity; i++) 
	{ 
		pixels[i] = color; 
	} 
	
	int lineCount=size.height/m_SrcFontsz.height;
	float reWidth=lineCount*m_tDimensions.width;
	CCSprite *pPrevOther=NULL;
	for(float j=m_tDimensions.width;j<=reWidth;j+=m_tDimensions.width){
		CCTexture2D *texture = new CCTexture2D(); 
		float width=0;
		if(j<reWidth){
			if(j==m_tDimensions.width){
				width=m_tDimensions.width-prevWidth;
			}else{
				width=m_tDimensions.width;
			}
		}else{
			width=m_tDimensions.width-(reWidth-m_SrcFontsz.width)+prevWidth;
		}
		texture->initWithData(pixels, kCCTexture2DPixelFormat_RGBA8888, 1, 1, CCSizeMake(width, 1)); 
		if(m_eType == kType_Image&&!m_imageFile.empty()){
			CCImage *pImage=new CCImage();
			pImage->initWithImageFile(m_imageFile.c_str());
			texture->initWithImage(pImage);
			pImage->release();
		}
		
		CCSprite *pOther = CCSprite::createWithTexture(texture); 
		m_pOthers.push_back(pOther);
		pOther->setAnchorPoint(CCPointZero);
		if( m_eType == kType_UnderLine )
		{
			if(j==m_tDimensions.width){
				pOther->setPosition(ccp(m_obRect.origin.x+prevWidth, size.height-m_SrcFontsz.height));
			}else{
				pOther->setPosition(ccp(m_obRect.origin.x, m_prevPos.y-m_SrcFontsz.height));
			}
			m_prevPos=pOther->getPosition();
			this->addChild(pOther); 
		}
		else if(m_eType == kType_DeleteLine)
		{
			if(j==m_tDimensions.width){
				pOther->setPosition(ccp(m_obRect.origin.x+prevWidth, size.height-m_SrcFontsz.height*3/5));
			}else{
				pOther->setPosition(ccp(m_obRect.origin.x, m_prevPos.y-m_SrcFontsz.height));
			}
			m_prevPos=pOther->getPosition();
			this->addChild(pOther); 
		}else if(m_eType == kType_Image){
			pOther->setTextureRect(CCRect(0,0,width,m_SrcFontsz.height));
			if(j==m_tDimensions.width){
				pOther->setPosition(ccp(m_obRect.origin.x+prevWidth, size.height-m_SrcFontsz.height));
			}else{
				pOther->setPosition(ccp(m_obRect.origin.x, m_prevPos.y-m_SrcFontsz.height));
			}
			m_prevPos=pOther->getPosition();
			this->addChild(pOther); 
		}else if(m_eType == kType_Button){
			UILayer *layer=UILayer::create();
			this->addChild(layer);
			UIButton* pButton = UIButton::create();
			pButton->setScale9Enabled(true);
			pButton->setSize(CCSize(width,m_SrcFontsz.height));
			pButton->setAnchorPoint(ccp(0,0));
			m_pButtons.push_back(pButton);
			layer->addWidget(pButton);

			if(j==m_tDimensions.width){
				layer->setPosition(ccp(m_obRect.origin.x+prevWidth, size.height-m_SrcFontsz.height));
			}else{
				layer->setPosition(ccp(m_obRect.origin.x, m_prevPos.y-m_SrcFontsz.height));
			}
			m_prevPos=layer->getPosition();
		}
		
		texture->release(); 
	}
	delete[] pixels; 
}

void YMLabelTTF::setButtonFile(std::string pNormal,std::string pressed,std::string disabled){
	for(int i=0;i<m_pButtons.size();i++){
		UIButton *pButton=m_pButtons.at(i);
		if(pButton){
			if(!pNormal.empty())
				pButton->loadTextureNormal(pNormal.c_str());
			if(!pressed.empty())
				pButton->loadTexturePressed(pressed.c_str());
			if(!disabled.empty())
				pButton->loadTextureDisabled(disabled.c_str());
		}
	}
}

void YMLabelTTF::addTouchEventCallBack(CCObject *target,SEL_TouchEvent selector){
	for(int i=0;i<m_pButtons.size();i++){
		UIButton *pButton=m_pButtons.at(i);
		if(pButton){
			pButton->addTouchEventListener(target,selector);
		}
	}
}

void YMLabelTTF::setTouchEnabled(bool IsTouchEnabled){
	for(int i=0;i<m_pButtons.size();i++){
		UIButton *pButton=m_pButtons.at(i);
		if(pButton){
			pButton->setTouchEnabled(IsTouchEnabled);
		}
	}
}

void YMLabelTTF::loadImageFile(std::string imageFile){
	m_imageFile=imageFile;
}