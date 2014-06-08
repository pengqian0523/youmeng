#ifndef _YMDebugUserData_h  
#define _YMDebugUserData_h    
#include "cocos2d.h" 
#include "cocos-ext.h"
#include "YMClientSocket.h"
#include "../cocos2dx/support/tinyxml2/tinyxml2.h"

using namespace tinyxml2;


class YMDebugUserData: public CCLayer
{  
private:
	XMLElement *m_RootNode;
	bool m_debug;
public:  
	YMDebugUserData();  
	~YMDebugUserData();  
	static YMDebugUserData* shareYMDebugUserData();
	bool init();
	void readXmlByFileName(const char *xmlFile);
	void readXmlByBinary(char *xmlData,unsigned long size);
	void initXML(XMLElement *xmlNode);
	bool getDebugValue();
};  
#endif  
