#include "LogoScene.h"
#include "YMLabelTTF.h"
#include "YMCommonLabelTTF.h"
#include "gameLayer.h"
#include "YMSocketData.h"
#include "YMClientSocket.h"

#include "ServerSocket.h"
#define FONT_SZ "fonts/arial.ttf"
#define MAXBUFLEN 1024
#define PORT 6060

LogoScene::LogoScene(){
	
}

LogoScene::~LogoScene(){

}

void LogoScene::onEnter(){
	CCScene::onEnter();

	
	gameLayer *pgameLayer=new gameLayer();
	this->addChild(pgameLayer);

	UILayer *pLayer=UILayer::create();
	UILayout *mylayout = dynamic_cast<UILayout *>(GUIReader::shareReader()->widgetFromJsonFile("logoscene/logoscene.json"));
	this->addChild(pLayer);
	pLayer->addWidget(mylayout);

// 	ServerSocket lsock=ServerSocket(6060);
// 	CCLOG("Listening Port 6060");
// 	
// 	Socket dsock=lsock.accept();
// 	CCLOG("Connection Established");
// 	
// 	lsock.close();
// 	//cout<<"Stop Listening"<<endl;
// 	
// 	CCLOG("%d",dsock.getSock());
// 	
// 
// 	dsock.close();
// 	CCLOG("Connection Closed");
	
	WSADATA Data;
	struct sockaddr_in serverSockAddr;
	struct sockaddr_in clientSockAddr;
	SOCKET serversocket;  //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号
	SOCKET clientsocket;
	int addrlen = sizeof(SOCKADDR_IN);   //16
	int status;
	int numrcv;
	char buffer[MAXBUFLEN];

	YMTcpSocket *ptcp=new YMTcpSocket();

	
	serversocket = ptcp->Create(AF_INET,SOCK_STREAM/*套接字类型*/,0/*套接字使用的特定协议*/);
	if (INVALID_SOCKET == serversocket)
	{
		CCLOG("%s",YMIconv::GBK2UTF("创建socket失败").c_str());
	}

	//绑定socket到特定地址(结构体地址)
	if (!ptcp->Bind(PORT))
	{
		CCLOG("%s",YMIconv::GBK2UTF("绑定地址失败").c_str());
	}

	//允许外来申请链接请求，或者说就是监听
	if (!ptcp->Listen(2))
	{
		CCLOG("%s",YMIconv::GBK2UTF("监听失败").c_str());
	}

	//当有请求申请是就接受链接请求
	char buf[1024];
	clientsocket = ptcp->Accept(*ptcp,buf);


	while(1)
	{
		numrcv = ptcp->Recv(buffer/*接受输入数据缓冲器的指针*/,MAXBUFLEN/*接受缓冲区大小*/,0/*传输控制方式*/);

		if ((0==numrcv)||(numrcv==SOCKET_ERROR))
		{
			CCLOG("%s",YMIconv::GBK2UTF("链接受到限制").c_str());
			status = closesocket(clientsocket);
			if (SOCKET_ERROR==status)
			CCLOG("%s",YMIconv::GBK2UTF("断开链接失败").c_str());

			status = WSACleanup();
			if (SOCKET_ERROR==status)
			CCLOG("%s",YMIconv::GBK2UTF("清理链接失败").c_str());
			return;
		}
		CCLOG("%s:%s",YMIconv::GBK2UTF("接受到客户端发过来的消息内容是").c_str(),buffer);
		CCLOG("%s",YMIconv::GBK2UTF("输入想要发送给客户端发送的消息内容并按回车发送").c_str());

		int numsnt;
		std::string toSendtxt=YMIconv::GBK2UTF("服务器发的消息");
		
		numsnt=ptcp->Send(toSendtxt.c_str(), strlen(toSendtxt.c_str()) + 1, 0);
		if (numsnt != (int)strlen(toSendtxt.c_str()) + 1)
		{
			CCLOG("%s",YMIconv::GBK2UTF("Connection terminated").c_str());
			status=ptcp->Close();
			if (status == SOCKET_ERROR)
			CCLOG("%s",YMIconv::GBK2UTF("ERROR: closesocket unsuccessful").c_str());
			status=ptcp->Clean();
			if (status == SOCKET_ERROR)
			CCLOG("%s",YMIconv::GBK2UTF("ERROR: WSACleanup unsuccessful").c_str());
			return;
		}
		
		CCLOG("%s",YMIconv::GBK2UTF("等待继续接受客户端发来的消息......").c_str());
	}







// 	YMSocketData data("{\"id\":\"123\"}");
// 
// 	if (!YMClientSocket::getIns()->getIns()->isConnected) {
// 		if (YMClientSocket::getIns()->connect("192.168.1.101", 6060) != SOCKET_ERROR) {
// 			YMClientSocket::getIns()->addDataPacketListener(0x0101, this, Event_Handler(LogoScene::hand0x0101Handler));
// 			YMClientSocket::getIns()->SendMsg(0x0101, &data);
// 			
// 		}
// 	} else {
// 		YMClientSocket::getIns()->addDataPacketListener(0x0101, this, Event_Handler(LogoScene::hand0x0101Handler));
// 		YMClientSocket::getIns()->SendMsg(0x0101, &data);
// 	}
}

void LogoScene::hand0x0101Handler(YMEvent *ymevt){
	CCLOG("aaaaa");
}

void LogoScene::ButtonCallBack(CCObject *obj,TouchEventType pType){
	if(pType==TOUCH_EVENT_ENDED){
		CCLOG("touch_____!");
	}
}