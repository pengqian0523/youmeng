﻿#ifndef __YMSocketDataEvent_H__
#define __YMSocketDataEvent_H__

#include "YMEvent.h"
#include "YMSocketData.h"

class YMSocketDataEvent : public YMEvent {
public:
    YMSocketDataEvent(string type, int stamp, char data[], unsigned int dataSize): YMEvent(type) {
        _body.parse(data, dataSize);
        _stamp = stamp;
    }
    virtual ~YMSocketDataEvent() {};

    YMSocketData getBody() {
        return _body;
    }

    int getStamp() {
        return _stamp;
    }

	bool isError(){
		if (_body.isMember("err")){
			return true;
		}
		return false;
	}

	string getErrorMsg(){
		if (_body.isMember("err")){
			return _body["err"].asString();
		}
		return "";
	}

protected:
private:
    int _stamp;
    YMSocketData _body;
};

#endif // __TCPSOCKET_H__
