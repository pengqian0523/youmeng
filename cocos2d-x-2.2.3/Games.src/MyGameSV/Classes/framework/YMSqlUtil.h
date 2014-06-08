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
	//��������һ��db���ݿ� dbΪ���ݿ������
	static void initDB(const char *db);
	//�����жϱ���Ƿ����
	static bool tableIsExist(string name);
	//��������һ������Ϊname�ı�񣬴���ʱ����ƥ��ʱ���иñ�Ĵ�����������򲻴���
	static void createTable(string sql,string name);
	//����ɾ��һ�ű���Ϊname�ı��ɾ��ʱ����ƥ���Ƿ��иñ�Ĵ��������������ִ��ɾ������
	static void deleteTable(string sql,string name);
	//��������в���һ������
	static void insertData(string sql);
	//���������ɾ��һ������
	static void deleteData(string sql);
	//����������޸�һ������
	static void updateData(string sql);
	//��ȡһ����¼������
	static int getDataCount(string sql);
	//��ȡһ����¼����Ϣ
	static std::string  getDataInfo(string sql,DataStorage *pDataStorage);
	static CCDictionary* getStorageDictByTable(std::string table);
	static CCObject *getObjectByTable(std::string table);
	//�رմ򿪵����ݿ�
	static void closeDB();
	static void dataConver(std::string tablename,int *tableType);



protected:
	//int isExisted( void * para, int n_column, char ** column_value, char ** column_name );
private:
	
};
#endif