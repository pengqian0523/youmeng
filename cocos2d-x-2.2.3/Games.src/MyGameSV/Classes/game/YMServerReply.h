#ifndef __YMServerReply_H__
#define __YMServerReply_H__

#include <stdlib.h>
#include "YMEventDispatcher.h"
#include "YMTcpSocket.h"
#include "YMSocketEvent.h"
#include "YMSocketDataEvent.h"
#include "YMSocketData.h"

class YMServerReply{
public:
    static YMServerReply *getIns();
private:
    static YMServerReply m_ins;
private:
    YMServerReply();
    ~YMServerReply();
public:
   bool sendData(int cmd, YMSocketDataEvent *sEvent);
   void Login(int cmd, YMSocketDataEvent *sEvent);
private:
   int m_cmd;
   YMSocketDataEvent *m_sEvent;
};

#endif