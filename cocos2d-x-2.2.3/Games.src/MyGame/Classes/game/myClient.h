#ifndef _myClient_H
#define _myClient_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "YMIconv.h"
#include "YMEvent.h"

using namespace  cocos2d;
using namespace std;
using namespace cocos2d::ui;
using namespace extension;

class myClient : public CCLayer{
public:
	myClient();
	~myClient();
	static myClient *create();
	bool init();
	void hand0x0101Handler(YMEvent *evt);
	void hand0x0102Handler(YMEvent *evt);
	void btncallback(CCObject *obj,TouchEventType type);
private:

};

#endif