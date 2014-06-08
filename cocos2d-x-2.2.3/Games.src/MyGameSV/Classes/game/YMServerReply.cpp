#include "YMServerReply.h"
#include "YMIconv.h"
#include "sqlite3.h"
#include "YMSqlUtil.h"
#include "YMSqlAnaly.h"

#if WIN32
#pragma comment(lib, "sqlite3.lib")
#endif


YMServerReply YMServerReply::m_ins;
YMServerReply *YMServerReply::getIns() {
    return &m_ins;
}

YMServerReply::YMServerReply(){

}

YMServerReply::~YMServerReply() {
    
}


bool YMServerReply::sendData(int cmd, YMSocketDataEvent *sEvent){
	//根据发送指令不同，发送给客户端的数据也不同
	
	 //创建数据库，如果数据库存在的话，就会打开数据库
    YMSqlUtil::initDB("ym.db");

    //数据库中美有表格的话就先创建一个表格先
    string sqlsss = "create table hero(uname text,upass text,id text,pname text,kind integer,sex integer)";
	Hero *pHero=new Hero();
	pHero->unametype=kStr;
	pHero->upasstype=kStr;
	pHero->pidtype=kStr;
	pHero->pnametype=kStr;
	pHero->kindtype=kInt;
	pHero->sextype=kInt;
	
	YMSqlUtil::getStorageDictByTable("hero")->setObject(pHero,"hero");

    YMSqlUtil::createTable(sqlsss,"hero");

	//YMSqlUtil::insertData("insert into hero values('root','pass','10000001','qqqq',1,1)");
	if(cmd==0x0101){
		Login(cmd,sEvent);
	}
	return true;
}

void YMServerReply::Login(int cmd, YMSocketDataEvent *sEvent){
	std::string sqlssssss = "select * from hero where uname="+YMIconv::GBK2UTF("'")+sEvent->getBody()["uname"].asString()+"'"+" and upass="+YMIconv::GBK2UTF("'")+sEvent->getBody()["upass"].asString()+"'";
	DataStorage *pDataStorage=new DataStorage();
	pDataStorage->tablename="hero";
	std::string ccc=YMSqlUtil::getDataInfo(sqlssssss,pDataStorage);
	YMSqlUtil::closeDB(); 
	YMSocketData sd(ccc.c_str());
	if(!ccc.empty()){
			CCLOG("%s",YMIconv::GBK2UTF("登陆成功!").c_str());
			YMTcpSocket::getIns()->SendMsg(cmd,&sd);
	}else{
		CCLOG("%s",YMIconv::GBK2UTF("用户名或密码错误!").c_str());
		YMSocketData sd1=YMSocketData();
		sd1["err"]=YMIconv::GBK2UTF("用户名或密码错误!").c_str();
		YMTcpSocket::getIns()->SendMsg(cmd,&sd1);
	}
}