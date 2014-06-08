#include "YMDebugUserData.h"
#include "AppMacros.h"
#include "../cocos2dx/platform/CCCommon.h"
#define  FILE_NAME "configs/debug.xml"



YMDebugUserData::YMDebugUserData()
{
	m_debug=false;
}

YMDebugUserData::~YMDebugUserData()
{
	
}

YMDebugUserData * YMDebugUserData::shareYMDebugUserData()
{
	static YMDebugUserData *pRet=NULL; 
	if(pRet==NULL)
	{
		pRet= new YMDebugUserData();
		pRet->init();
	}
	return pRet;
}


bool YMDebugUserData::init(){
	readXmlByFileName(FILE_NAME);

	return true;
}

void YMDebugUserData::readXmlByFileName(const char *xmlFile) {
	unsigned long size;
	tinyxml2::XMLDocument xmlDoc;
	char *xmlFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlFile, "rb", &size);
	readXmlByBinary(xmlFileContent, size);
	//ÊÍ·ÅÄÚ´æ
	delete[] xmlFileContent;
}

void YMDebugUserData::readXmlByBinary(char *xmlData,unsigned long size){
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.Parse(xmlData, size);
	initXML(xmlDoc.RootElement());
}

void YMDebugUserData::initXML(XMLElement *xmlNode){	
	if (xmlNode != NULL) {
		m_RootNode=xmlNode;
		XMLElement *ChildNode=m_RootNode->FirstChildElement();
		while(ChildNode){
			m_debug=ChildNode->BoolAttribute("debug");
			ChildNode=ChildNode->NextSiblingElement();
		}
	}
}

bool YMDebugUserData::getDebugValue(){
	
	return m_debug;
}