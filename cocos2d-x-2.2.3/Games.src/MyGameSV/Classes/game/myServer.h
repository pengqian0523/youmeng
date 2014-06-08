#ifndef _myServer_H
#define _myServer_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "YMIconv.h"
#include "YMEvent.h"

using namespace  cocos2d;
using namespace std;
using namespace cocos2d::ui;
using namespace extension;

class myServer : public CCLayer{
public:
	myServer();
	~myServer();
	static myServer *create();
	bool init();
private:

};

#endif