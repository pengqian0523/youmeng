#ifndef _Data_Util_
#define _Data_Util_
#include <string>
#include "cocos2d.h"
#include "YMSqlAnaly.h"

using namespace std;
using namespace cocos2d;

class YMSqlUtil
{
public:
	//用来创建一个db数据库 db为数据库的名字
	static void initDB(const char *db);
	//用来判断表格是否存在
	static bool tableIsExist(string name);
	//用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建
	static void createTable(string sql,string name);
	//用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作
	static void deleteTable(string sql,string name);
	//用来向表中插入一条数据
	static void insertData(string sql);
	//用来向表中删除一条数据
	static void deleteData(string sql);
	//用来向表中修改一条数据
	static void updateData(string sql);
	//获取一个记录的条数
	static int getDataCount(string sql);
	//读取一条记录的信息
	static std::string  getDataInfo(string sql,DataStorage *pDataStorage);
	static CCDictionary* getStorageDictByTable(std::string table);
	static CCObject *getObjectByTable(std::string table);
	//关闭打开的数据库
	static void closeDB();
	static void dataConver(std::string tablename,int *tableType);



protected:
	//int isExisted( void * para, int n_column, char ** column_value, char ** column_name );
private:
	
};
#endif