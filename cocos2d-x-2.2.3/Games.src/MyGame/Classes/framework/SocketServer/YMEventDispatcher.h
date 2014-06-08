#ifndef __YMEventDispatcher_H__
#define __YMEventDispatcher_H__

#include "../cocos2dx/platform/CCCommon.h"
#include "../cocos2dx/cocoa/CCObject.h"
#include "../cocos2dx/CCDirector.h"
#include "../cocos2dx/CCScheduler.h"
#include "YMEvent.h"
#include <vector>
#include <string>
#include <map>

using namespace std;
USING_NS_CC;

class YMEvent;

#define Event_Handler(_SELECTOR)(EventHandler)(&_SELECTOR)

typedef void (CCObject::*EventHandler)(YMEvent*);

struct listenerStruct {
    string eventType;
    CCObject *target;
    EventHandler callback;
};

typedef std::vector<listenerStruct> CallList;
typedef std::map<string, CallList> ListenerMap;

class YMEventDispatcher: public CCObject {
public:
    YMEventDispatcher();
    virtual ~YMEventDispatcher();

public:
    void addListener(string eventType, CCObject *target, EventHandler handler);
    void removeListener(string eventType, CCObject *target, EventHandler handler);
    void dispatchEvent(YMEvent *event);

    class DispatcherHelper: public CCObject {
    public:
        DispatcherHelper();
        virtual ~DispatcherHelper();
        void addListener(string eventType, CCObject *target, EventHandler handler);
        void removeListener(string eventType, CCObject *target, EventHandler handler);
        void dispatchEvent(YMEvent *event);
        virtual void update(float dt);
		 void releaseEvent();
    private:
		bool blRelease;
		bool blDispatcher;
		CallList addList;
		CallList removeList;
        list<YMEvent*> dispatcherList;
        ListenerMap m_list;
        long int getCallBackIndex(CallList tlist, CCObject *target, EventHandler handler);       
    };
private:
    DispatcherHelper *helper;
};

#endif // __YMEventDispatcher_H__
