#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "LogoScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
	CCFileUtils::sharedFileUtils()->addSearchPath("Resources");
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::sharedEngine()->end();
	//游戏结束时的释放
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	LogoScene *logoscene = new LogoScene();
	pDirector->runWithScene(logoscene);
                      
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    
}

void AppDelegate::updateRotaion(float pScreenWidth,float pScreenHeight)
{
	//GAME_WIDTH = pScreenWidth;
	//GAME_HEIGHT = pScreenHeight;
	//mControllerLayer->updateRotaion(pScreenWidth,pScreenHeight);
}
