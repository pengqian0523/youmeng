#include "YMSqlUtil.h"
#include "sqlite3.h"
#include "YMIconv.h"
#include "YMSocketData.h"
#include "YMSqlAnaly.h"

USING_NS_CC;


sqlite3 *pDB = NULL;//数据库指针 
char * errMsg = NULL;//错误信息 
std::string sqlstr;//SQL指令 
int result;//sqlite3_exec返回值 
CCDictionary *m_tableDict=new CCDictionary();

//创建数据库
void YMSqlUtil::initDB(const char *db )
{ 
	//打开一个数据库，如果该数据库不存在，则创建一个数据库文件
	result = sqlite3_open(db, &pDB); 
	if( result != SQLITE_OK ) 
		CCLOG( YMIconv::GBK2UTF("打开数据库失败，错误码:%d ，错误原因:%s\n").c_str() , result, errMsg ); 
}


//tableIsExist的回调函数
int isExisted( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	bool *isExisted_=(bool*)para; 
	*isExisted_=(**column_value)!='0'; 
	return 0; 
}
//判断表格是否存在
//sqlstr = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
bool YMSqlUtil::tableIsExist( string name )
{
	if (pDB!=NULL)
	{
		//判断表是否存在
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
		result =sqlite3_exec(pDB,sqlstr.c_str(),isExisted,&tableIsExisted,&errMsg);
		return tableIsExisted;
	}
	return false;
}


//在数据库中判断名为name的表示否存在，如果不存在则创建这张表
//@示例语句string sqls = "create table user(id integer,username text,password text)";
void YMSqlUtil::createTable( string sql,string name)
{
	if (!tableIsExist(name))
	{
		//创建表，设置ID为主键，且自动增加 
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLOG(YMIconv::GBK2UTF("创建表失败，错误码:%d ，错误原因:%s\n").c_str(), result, errMsg ); 
	}

}


//删除表格
//@示例语句sqlstr="drop table name"; 
void YMSqlUtil::deleteTable( string sql,string name )
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLOG(YMIconv::GBK2UTF("创建表失败，错误码:%d ，错误原因:%s\n").c_str(), result, errMsg ); 
	}
}


//插入数据
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) "; 
void YMSqlUtil::insertData( string sql ){
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
		CCLOG(YMIconv::GBK2UTF("插入记录失败，错误码:%d ，错误原因:%s\n").c_str(), result, errMsg ); 
}


//删除数据
//@示例语句sqlstr="delete from MyTable_1 where ID = 2"; 
void YMSqlUtil::deleteData( string sql )
{
	result=sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLOG(YMIconv::GBK2UTF("插入记录失败，错误码:%d ，错误原因:%s\n").c_str(), result, errMsg ); 
}


//修改数据
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3"; 
void YMSqlUtil::updateData( string sql )
{
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLOG(YMIconv::GBK2UTF("插入记录失败，错误码:%d ，错误原因:%s\n").c_str(), result, errMsg ); 
}


//getDataCount的回调函数
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name) 
{ 
	int *count=(int*)para; 
	*count=n_column; 
	return 0; 
}
//获取记录的条数
//@示例语句string sqlsssss = "select count(*) from user";
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";
int YMSqlUtil::getDataCount( string sql )
{
	int count=0; 
	sqlite3_exec( pDB, sql.c_str() , loadRecordCount, &count, &errMsg ); 
	return count;
}


//getDataInfo的回调函数
int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	DataStorage *pDataStorage=(DataStorage *)para;
	
	int *tableType=(int *)malloc(n_column);
	YMSqlUtil::dataConver(pDataStorage->tablename,tableType);
	std::string content="{";
	for(int i=0;i<n_column;i++){
		content+="\"";
		content+=column_name[i];
		content+="\"";
		content+=":";
		if(tableType[i]==kStr){
			content+="\"";
			content+=column_value[i];
			content+="\"";
		}else if(tableType[i]==kList){
		//content+=column_value[i];
		}else{
			content+=column_value[i];
		}
		if(i<n_column-1){
			content+=",";
		}else{
			content+="}";
		}
	}
	CCLOG("%s",content.c_str());
	pDataStorage->content=content;
	return 0; 
} 

void YMSqlUtil::dataConver(std::string tablename,int *tableType){
	if(tablename.compare(HERO_TABLE)==0){
		Hero *pHero=(Hero *)m_tableDict->objectForKey(tablename);
		int i=0;
		tableType[i++]=pHero->unametype;
		tableType[i++]=pHero->upasstype;
		tableType[i++]=pHero->pidtype;
		tableType[i++]=pHero->pnametype;
		tableType[i++]=pHero->kindtype;
		tableType[i++]=pHero->sextype;
	}
}

//获取一条记录的信息 其中的pSend是一个实体类我们以后可以自定义一个继承了CCObject的类来代替他保存数据库中取出来的数据
std::string YMSqlUtil::getDataInfo( string sql,DataStorage *pDataStorage)
{
	sqlite3_exec( pDB, sql.c_str() , loadRecord,pDataStorage, &errMsg );
	return pDataStorage->content;

}

//关闭数据库
void YMSqlUtil::closeDB()
{
	sqlite3_close(pDB); 
}

CCDictionary* YMSqlUtil::getStorageDictByTable(std::string table){
	return m_tableDict;
}

CCObject *YMSqlUtil::getObjectByTable(std::string table){
	return (CCObject *)m_tableDict->objectForKey(table);
}