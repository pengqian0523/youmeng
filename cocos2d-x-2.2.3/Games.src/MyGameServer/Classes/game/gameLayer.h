#ifndef _GameLayer_H
#define _GameLayer_H

#include "cocos2d.h"
using namespace  cocos2d;
using namespace std;

class gameLayer : public CCLayer{
public:
	gameLayer();
	~gameLayer();
	void onEnter();
private:

};

#endif