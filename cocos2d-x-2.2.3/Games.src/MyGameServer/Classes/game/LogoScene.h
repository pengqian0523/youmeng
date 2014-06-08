#ifndef _LOGO_SCENE_H
#define _LOGO_SCENE_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "YMIconv.h"
#include "YMEvent.h"

using namespace  cocos2d;
using namespace std;
using namespace cocos2d::ui;
using namespace extension;

class LogoScene : public CCScene{
public:
	LogoScene();
	~LogoScene();
	void onEnter();
	void ButtonCallBack(CCObject *obj,TouchEventType pType);
	void hand0x0101Handler(YMEvent *ymevt);
private:

};

#endif