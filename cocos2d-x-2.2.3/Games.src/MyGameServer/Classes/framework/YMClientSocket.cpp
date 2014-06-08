﻿#include "YMClientSocket.h"
#include "YMIconv.h"
#include "YMDebugCmd.h"

YMClientSocket YMClientSocket::m_ins;
YMClientSocket *YMClientSocket::getIns() {
    return &m_ins;
}

YMClientSocket::YMClientSocket():
    tcpSocket(new YMTcpSocket()) {
    iStamp = 1;
    isConnected = false;
    YMTcpSocket::Init();
    int ret = tcpSocket->Create(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (ret) {
        CCLOG("Create socket:success,ret:%d\n", ret);
    } else {
        CCLOG("Create socket:fail,ret:%d", ret);
    }
}

YMClientSocket::~YMClientSocket() {
    YMTcpSocket::Clean();
    close();
    if ( tcpSocket ) {
        delete tcpSocket;
        tcpSocket = NULL;
    }
}

void YMClientSocket::addDataPacketListener(unsigned int eventType, CCObject *target, EventHandler handler) {
    this->addListener(int2String(eventType), target, handler);
}

void YMClientSocket::removeDataPacketListener(unsigned int eventType, CCObject *target, EventHandler handler) {
    this->removeListener(int2String(eventType), target, handler);
}

string YMClientSocket::int2String(unsigned int val) {
    char buff[25];
    sprintf(buff, "0x%04X", val);
    return std::string(buff);
}

int YMClientSocket::chars2Int(char chars[]) {
    int iVal = 0;
    for (int i = 0; i < 4; i++) {
        iVal += (chars[i] & 0xFF) << ((3 - i) * 8);
    }
    return iVal;
}

void YMClientSocket::int2Chars(char *chars, int val, int start) {
    chars[start++] = (char)((val >> 24) & 0xFF);
    chars[start++] = (char)((val >> 16) & 0xFF);
    chars[start++] = (char)((val >> 8) & 0xFF);
    chars[start++] = (char)((val >> 0) & 0xFF);
}

int YMClientSocket::connect(const char* ip, unsigned short port) {
    if (!isConnected) {
        int connectFlag = tcpSocket->Connect(ip, port);
        if (connectFlag != SOCKET_ERROR) {
            int errCode = pthread_attr_init(&threadAttr);
            CCLOG("pthread_attr_init error code:%d", errCode);
            if (errCode == 0) {
                errCode = pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
                CCLOG("pthread_attr_setdetachstate error code:%d", errCode);
                if (errCode != 0) {
                    pthread_attr_destroy(&threadAttr);
                }
                errCode = pthread_create(&threadId, &threadAttr, &YMClientSocket::threadHandler, NULL);
                CCLOG("pthread_create error code:%d", errCode); 
            }
            isConnected = true;
        }
        this->dispatchEvent(new YMSocketEvent(YMSocketEvent::EVENT_SOCKET_CONNECT, connectFlag));
        CCLOG("Connect %s:%d  %s\n", ip, port, (connectFlag != SOCKET_ERROR ? " success" : "fail"));
        return connectFlag;
    }
    return 1;
}

int YMClientSocket::close() {
    isConnected = false;
    this->dispatchEvent(new YMSocketEvent(YMSocketEvent::EVENT_SOCKET_CLOSE, 0));
    return tcpSocket->Close();
}

int YMClientSocket::Recv(char* buf, int len, int flags) {
    return tcpSocket->Recv(buf, len, flags);
}

int YMClientSocket::SendMsg(int cmd, YMSocketData *data) {
    char packBuffer[2048];
	memset(packBuffer,0,2048);
    int packSize;
    data->serializer(packBuffer, &packSize);
	CCLOG("%s",packBuffer);
    char blYs = 0;
    int sendSize = packSize + 13;
    char *sedBuffer = (char *)malloc(sendSize);

    iStamp++;
    //encrypt(packBuffer, packSize, cmd, iStamp);

    if (packSize >= 2048) {
        //压缩
        //compress()
        blYs = 0;
    }
    int2Chars(sedBuffer, (sendSize - 4));
    int2Chars(sedBuffer, cmd, 4);
    int2Chars(sedBuffer, iStamp, 8);
    sedBuffer[12] = blYs;
    memcpy(sedBuffer + 13, packBuffer, packSize);
    int i = tcpSocket->Send(sedBuffer, sendSize, 0);

    const char tName[20] = "前端：";
	//YMDebugCmd::shareYMDebugCmd()->AddsendMsg(tName, cmd, iStamp, data->getJsonString().c_str());
    CCLOG("%s 0x%04X, %d, %s", YMIconv::GBK2UTF(tName).c_str(), cmd, iStamp, data->getJsonString().c_str());
	
    free(sedBuffer);
	
    return iStamp;
}


int YMClientSocket::GetError() {
    return tcpSocket->GetError();
}

void YMClientSocket::dataIn(char buff[], unsigned int size) {
    unsigned int packSize = size - 9;
    int cmd = chars2Int(buff);
    int stamp = chars2Int(buff + 4);
    char blYs = buff[8];
    char *sPack = (char *)malloc(packSize);
    memcpy(sPack, buff + 9, packSize);
    if (blYs != 0) {
        //解压
        //uncompress()
    }
    //encrypt(sPack, packSize, cmd, stamp);
    YMSocketDataEvent *sEvent = new YMSocketDataEvent(int2String(cmd), stamp, sPack, packSize);
   
    
    const char tName[20] = "后台：";	
	//YMDebugCmd::shareYMDebugCmd()->AddDataIn(tName, cmd, iStamp, sEvent->getBody().getJsonString().c_str());
    CCLOG("%s 0x%04X, %d, %s", YMIconv::GBK2UTF(tName).c_str(), cmd, stamp, sEvent->getBody().getJsonString().c_str());
		
	YMClientSocket::getIns()->dispatchEvent(sEvent);
	free(sPack);
}

void YMClientSocket::getKey(int cmd, int stamp , int needKeylen, char buffer[]) {
    int byteKey = (((needKeylen & cmd) + (stamp ^ cmd)) % 0xFF);
    for (int i = 0; i < needKeylen; i++) {
        cmd -=  (((byteKey * needKeylen) ^ stamp) % 0xFF);
        stamp -=  byteKey;
        byteKey = ((cmd % 0xFF));
        buffer[i] = byteKey;
    }
}

void YMClientSocket::encrypt(char buffer[], int size, int cmd, int stamp) {
    int len = (size > 128) ? 128 : size;
    char keys[128];
    getKey(cmd, stamp, len, keys);
    for (int i = 0; i < len; i++) {
        buffer[i] = buffer[i] ^ keys[i];
    }
}

void YMClientSocket::hasError() {
    this->dispatchEvent(new YMSocketEvent(YMSocketEvent::EVENT_SOCKET_ERROR, GetError()));
};

void *YMClientSocket::threadHandler(void *arg) {
    char buff[4];
	//memset(buff,0,4);
    int len;
    while (1) {
        len = YMClientSocket::getIns()->Recv(buff, 4, 0);
        if (len > 0) {
            int pSize = YMClientSocket::getIns()->chars2Int(buff);
            char *sPack = (char *)alloca(pSize);
            YMClientSocket::getIns()->Recv(sPack, pSize, 0);
            YMClientSocket::getIns()->dataIn(sPack, pSize);
        } else {
            YMClientSocket::getIns()->hasError();
            CCLOG("threadHandler errorCode: %d" , YMClientSocket::getIns()->GetError());
        }
        if (len < 0) {
            CCLOG("%s", "==== connect break up ====");
            //服务端断开
            YMClientSocket::getIns()->close();
			//pthread_exit(0);
            break;
        }
    }
    return 0;
}
