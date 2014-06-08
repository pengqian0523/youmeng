#include "YMSqlUtil.h"
#include "sqlite3.h"
#include "YMIconv.h"
#include "YMSocketData.h"
#include "YMSqlAnaly.h"

USING_NS_CC;


sqlite3 *pDB = NULL;//���ݿ�ָ�� 
char * errMsg = NULL;//������Ϣ 
std::string sqlstr;//SQLָ�� 
int result;//sqlite3_exec����ֵ 
CCDictionary *m_tableDict=new CCDictionary();

//�������ݿ�
void YMSqlUtil::initDB(const char *db )
{ 
	//��һ�����ݿ⣬��������ݿⲻ���ڣ��򴴽�һ�����ݿ��ļ�
	result = sqlite3_open(db, &pDB); 
	if( result != SQLITE_OK ) 
		CCLOG( YMIconv::GBK2UTF("�����ݿ�ʧ�ܣ�������:%d ������ԭ��:%s\n").c_str() , result, errMsg ); 
}


//tableIsExist�Ļص�����
int isExisted( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	bool *isExisted_=(bool*)para; 
	*isExisted_=(**column_value)!='0'; 
	return 0; 
}
//�жϱ���Ƿ����
//sqlstr = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
bool YMSqlUtil::tableIsExist( string name )
{
	if (pDB!=NULL)
	{
		//�жϱ��Ƿ����
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
		result =sqlite3_exec(pDB,sqlstr.c_str(),isExisted,&tableIsExisted,&errMsg);
		return tableIsExisted;
	}
	return false;
}


//�����ݿ����ж���Ϊname�ı�ʾ����ڣ�����������򴴽����ű�
//@ʾ�����string sqls = "create table user(id integer,username text,password text)";
void YMSqlUtil::createTable( string sql,string name)
{
	if (!tableIsExist(name))
	{
		//����������IDΪ���������Զ����� 
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLOG(YMIconv::GBK2UTF("������ʧ�ܣ�������:%d ������ԭ��:%s\n").c_str(), result, errMsg ); 
	}

}


//ɾ�����
//@ʾ�����sqlstr="drop table name"; 
void YMSqlUtil::deleteTable( string sql,string name )
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK ) 
			CCLOG(YMIconv::GBK2UTF("������ʧ�ܣ�������:%d ������ԭ��:%s\n").c_str(), result, errMsg ); 
	}
}


//��������
//@ʾ�����sqlstr=" insert into MyTable_1( name ) values ( '������' ) "; 
void YMSqlUtil::insertData( string sql ){
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg ); 
	if(result != SQLITE_OK ) 
		CCLOG(YMIconv::GBK2UTF("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n").c_str(), result, errMsg ); 
}


//ɾ������
//@ʾ�����sqlstr="delete from MyTable_1 where ID = 2"; 
void YMSqlUtil::deleteData( string sql )
{
	result=sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLOG(YMIconv::GBK2UTF("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n").c_str(), result, errMsg ); 
}


//�޸�����
//@ʾ�����        sqlstr="update MyTable_1 set name='������' where ID = 3"; 
void YMSqlUtil::updateData( string sql )
{
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) 
		CCLOG(YMIconv::GBK2UTF("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n").c_str(), result, errMsg ); 
}


//getDataCount�Ļص�����
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name) 
{ 
	int *count=(int*)para; 
	*count=n_column; 
	return 0; 
}
//��ȡ��¼������
//@ʾ�����string sqlsssss = "select count(*) from user";
//@ʾ�����  ȡ�ñ���ֶε����string sqlsssss = "select * from user";
int YMSqlUtil::getDataCount( string sql )
{
	int count=0; 
	sqlite3_exec( pDB, sql.c_str() , loadRecordCount, &count, &errMsg ); 
	return count;
}


//getDataInfo�Ļص�����
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

//��ȡһ����¼����Ϣ ���е�pSend��һ��ʵ���������Ժ�����Զ���һ���̳���CCObject�������������������ݿ���ȡ����������
std::string YMSqlUtil::getDataInfo( string sql,DataStorage *pDataStorage)
{
	sqlite3_exec( pDB, sql.c_str() , loadRecord,pDataStorage, &errMsg );
	return pDataStorage->content;

}

//�ر����ݿ�
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