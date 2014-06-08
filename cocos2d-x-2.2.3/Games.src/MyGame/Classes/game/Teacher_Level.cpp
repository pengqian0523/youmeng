#include "Teacher_Level.h"
#include "cocos-ext.h"
#include "YMIconv.h"

#define FONT_SZ "fonts/Thonburi.ttf"

using namespace cocos2d::ui;
using namespace extension;

Teacher::Teacher(){
	m_name=YMIconv::GBK2UTF("����");
	m_sex=0;
	m_age=30;
	m_address=YMIconv::GBK2UTF("����");
	m_pname=YMIconv::GBK2UTF("��ʦ");
}

Teacher::~Teacher(){

}

Teacher* Teacher::create(){
	Teacher *pRet=new Teacher();
	if(pRet){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}




Level::Level(){
	m_place=YMIconv::GBK2UTF("������");
}

Level::~Level(){

}

Level* Level::create(){
	Level *pRet=new Level();
	if(pRet){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}



Teacher_Level::Teacher_Level(){
	m_salary=6000;
}

Teacher_Level::~Teacher_Level(){

}

Teacher_Level* Teacher_Level::create(){
	Teacher_Level *pRet=new Teacher_Level();
	if(pRet && pRet->init()){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool Teacher_Level::init(){
	show();
	return true;
}

void Teacher_Level::show(){
	std::string psex="";
	if(m_sex==0){
		psex=YMIconv::GBK2UTF("��");
	}else{
		psex=YMIconv::GBK2UTF("Ů");
	}
	char buff[2048];
	sprintf(buff,YMIconv::GBK2UTF("����:%s,�Ա�:%s,����:%d,��ַ:%s,ְ��:%s,ְλ:%s,����:%d").c_str(),m_name.c_str(),psex.c_str(),m_age,m_address.c_str(),m_pname.c_str(),m_place.c_str(),m_salary);
	CCLOG(buff);
}