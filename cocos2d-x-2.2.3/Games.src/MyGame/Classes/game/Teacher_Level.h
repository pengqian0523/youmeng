#ifndef _TEACHER_H
#define _TEACHER_H

#include "cocos2d.h"
using namespace  cocos2d;
using namespace std;

class Level:public CCObject{
protected:
	std::string m_place;
public:
	Level();
	~Level();
	static Level* create();
	void setPlace(std::string place){m_place=place;}
	std::string getPlace(){return m_place;}
};

class Teacher:public Level{
protected:
	std::string m_name;
	int m_sex;
	int m_age;
	std::string m_address;
	std::string m_pname;
public:
	Teacher();
	~Teacher();
	static Teacher* create();
	void setName(std::string pName){m_name = pName;}
	std::string getName(){return m_name;}
	void setSex(int sex){m_sex = sex;}
	int getSex(){return m_sex;}
	void setAge(int age){m_age=age;}
	int getAge(){return m_age;}
	void setAddr(std::string pAddr){m_address = pAddr;}
	std::string getAddr(){return m_address;}
	void setpName(std::string pName){m_pname = pName;}
	std::string getpName(){return m_pname;}
};

class Teacher_Level : public Teacher{
public:
	Teacher_Level();
	~Teacher_Level();
	static Teacher_Level* create();
	bool init();
	void show();
public:
	int m_salary;
};

#endif