#include "YMEventDispatcher.h"

YMEventDispatcher::YMEventDispatcher() {
    helper = new DispatcherHelper();
}

YMEventDispatcher::~YMEventDispatcher() {	
    if ( helper ) {
		helper->dispatchEvent(new YMEvent(YMEvent::YMEvent_RELEASE));
		helper->update(0);
		helper->releaseEvent();
        delete helper;
        helper = NULL;
    }
}

/**
 * 广播事件监听
 */
void YMEventDispatcher::dispatchEvent(YMEvent *event) {
    if (helper != NULL) {
        event->setTarget(this);
        helper->dispatchEvent(event);
    } else {
        //delete event;
		CC_SAFE_DELETE(event);
    }
}

/**
 * 添加事件监听
 */
void YMEventDispatcher::addListener(string eventType, CCObject *target, EventHandler handler) {
    if (helper != NULL) {
        helper->addListener(eventType, target, handler);
    }
}

/**
 * 移除事件监听
 */
void YMEventDispatcher::removeListener(string eventType, CCObject *target, EventHandler handler) {
    if (helper != NULL) {
        helper->removeListener(eventType, target, handler);
    }
}

/**
 * ======================================================
 */

YMEventDispatcher::DispatcherHelper::DispatcherHelper() {
    blDispatcher = false;
    blRelease = false;
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

YMEventDispatcher::DispatcherHelper::~DispatcherHelper() {	
    //releaseEvent();
}

void YMEventDispatcher::DispatcherHelper::releaseEvent() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    blRelease = true;
    while (m_list.begin() != m_list.end()) {
        CallList cList = m_list[m_list.begin()->first];
        cList.clear();
        m_list.erase(m_list.begin());
    }
    //while (dispatcherList.size() > 0) {
    //    Event *event = *(dispatcherList.begin());
	list<YMEvent*>::const_iterator it;
	while( (it = dispatcherList.begin()) != dispatcherList.end() ){
		YMEvent *event = *it;
        if (event != NULL) {
            delete event;
        }
        dispatcherList.pop_front();
    }
    dispatcherList.clear();
}

long int YMEventDispatcher::DispatcherHelper::getCallBackIndex(CallList tlist, CCObject *target, EventHandler handler) {
    long int len = tlist.size();
    for (long int i = 0; i < len; i++) {
        if (tlist[i].callback == handler && tlist[i].target == target) {
            return i;
        }
    }
    return -1;
}

/**
 * 添加事件监听
 */
void YMEventDispatcher::DispatcherHelper::addListener(string eventType, CCObject *target, EventHandler handler) {
    if (!blRelease) {
        struct listenerStruct addItem;
        addItem.eventType = eventType;
        addItem.target = target;
        addItem.callback = handler;
        if (!blDispatcher) {
            long int index = getCallBackIndex(m_list[eventType], target, handler);
            if (index == -1) {
                m_list[eventType].push_back(addItem);
            }
        } else {
            addList.push_back(addItem);
        }
    }
}

/**
 * 移除事件监听
 */
void YMEventDispatcher::DispatcherHelper::removeListener(string eventType, CCObject *target, EventHandler handler) {
    if (!blRelease) {
        if (!blDispatcher) {
            ListenerMap::iterator item = m_list.find(eventType);
            if (item != m_list.end()) {
                unsigned long index = getCallBackIndex(item->second, target, handler);
                if (index >= 0 && index < item->second.size()) {
                    item->second.erase(item->second.begin() + index);
                }
				if (item->second.size()==0){
					m_list.erase(item);
				}
            }
        } else {
            struct listenerStruct removeItem;
            removeItem.eventType = eventType;
            removeItem.target = target;
            removeItem.callback = handler;
            removeList.push_back(removeItem);
        }
    }
}
/**
 * 广播事件
 */
void YMEventDispatcher::DispatcherHelper::dispatchEvent(YMEvent *event) {
    if (!blRelease) {
        dispatcherList.push_back(event);
    } else {
        if (event != NULL) {
            delete event;
        }
    }
}

void YMEventDispatcher::DispatcherHelper::update(float dt) {
    if (this != NULL) {
        if (dispatcherList.size() > 0) {
            blDispatcher = true;
            YMEvent *event = *(dispatcherList.begin());
            dispatcherList.pop_front();
            string eventType = event->getType();
            ListenerMap::iterator item = m_list.find(eventType);
            if (item != m_list.end()) {
                CallList::iterator listItem = item->second.begin();
                while (listItem != item->second.end()) {
                    EventHandler handler = listItem->callback;
                    CCObject *target = listItem->target;
                    if (target != NULL && handler != NULL) {
                        (target->*handler)(event);
                    } else {
                        removeList.push_back(listenerStruct(*listItem));
                    }
                    listItem++;
                }
            }
            delete event;
            blDispatcher = false;
        }
        if (removeList.size() > 0) {
            for (CallList::iterator item = removeList.begin(); item != removeList.end(); item++) {
                removeListener(item->eventType, item->target, item->callback);
            }
            removeList.clear();
        }
        if (addList.size() > 0) {
            for (CallList::iterator item = addList.begin(); item != addList.end(); item++) {
                addListener(item->eventType, item->target, item->callback);
            }
            addList.clear();
        }
    }
}
