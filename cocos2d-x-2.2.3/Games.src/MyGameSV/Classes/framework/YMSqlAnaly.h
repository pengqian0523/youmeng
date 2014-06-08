#ifndef _SQL_Util_
#define _SQL_Util_
#include <string>
#include "cocos2d.h"

#define HERO_TABLE "hero"
using namespace std;
using namespace cocos2d;

enum datatype{
	kList=-1,
	kInt,
	kULint,
	kFloat,
	kDouble,
	kBool,
	kStr
};

struct Hero : public CCObject{
	datatype unametype;
	datatype upasstype;
	datatype pidtype;
	datatype pnametype;
	datatype kindtype;
	datatype sextype;
};

struct DataStorage{
	std::string tablename;
	std::string content;
};

#endif