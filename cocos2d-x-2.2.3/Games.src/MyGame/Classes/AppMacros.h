#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

//design screen size
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(960, 640);

//Screen Size
#define SCREEN_SIZE cocos2d::CCDirector::sharedDirector()->getVisibleSize();

//Resource Scale
#define RESOURCE_SCALE	((CCDirector::sharedDirector()->getVisibleSize().width/designResolutionSize.width) > (CCDirector::sharedDirector()->getVisibleSize().height/designResolutionSize.height) ?\
						 (CCDirector::sharedDirector()->getVisibleSize().width/designResolutionSize.width) : (CCDirector::sharedDirector()->getVisibleSize().height/designResolutionSize.height))

#define RESOURCE_SCALEX (CCDirector::sharedDirector()->getVisibleSize().width/designResolutionSize.width)

#define RESOURCE_SCALEY (CCDirector::sharedDirector()->getVisibleSize().height/designResolutionSize.height)


// Default font
#define TITLE_FONT_NAME	("fonts/FZ.ttf")
// The font size 24 is designed for design resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE	(cocos2d::CCDirector::sharedDirector()->getVisibleSize().width / designResolutionSize.width * 24)
//font color
#define TITLE_FONT_COLOR ccc3(0x73,0x41,0x29)

#endif /* __APPMACROS_H__ */
