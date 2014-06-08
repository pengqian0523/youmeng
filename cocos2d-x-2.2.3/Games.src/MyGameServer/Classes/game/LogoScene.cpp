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
	SOCKET serversocket;  //��ʵ����һ��unsig int���ͣ�������¼�Ѿ�����������δ�������׽��ֺ�
	SOCKET clientsocket;
	int addrlen = sizeof(SOCKADDR_IN);   //16
	int status;
	int numrcv;
	char buffer[MAXBUFLEN];

	YMTcpSocket *ptcp=new YMTcpSocket();

	
	serversocket = ptcp->Create(AF_INET,SOCK_STREAM/*�׽�������*/,0/*�׽���ʹ�õ��ض�Э��*/);
	if (INVALID_SOCKET == serversocket)
	{
		CCLOG("%s",YMIconv::GBK2UTF("����socketʧ��").c_str());
	}

	//��socket���ض���ַ(�ṹ���ַ)
	if (!ptcp->Bind(PORT))
	{
		CCLOG("%s",YMIconv::GBK2UTF("�󶨵�ַʧ��").c_str());
	}

	//�������������������󣬻���˵���Ǽ���
	if (!ptcp->Listen(2))
	{
		CCLOG("%s",YMIconv::GBK2UTF("����ʧ��").c_str());
	}

	//�������������Ǿͽ�����������
	char buf[1024];
	clientsocket = ptcp->Accept(*ptcp,buf);


	while(1)
	{
		numrcv = ptcp->Recv(buffer/*�����������ݻ�������ָ��*/,MAXBUFLEN/*���ܻ�������С*/,0/*������Ʒ�ʽ*/);

		if ((0==numrcv)||(numrcv==SOCKET_ERROR))
		{
			CCLOG("%s",YMIconv::GBK2UTF("�����ܵ�����").c_str());
			status = closesocket(clientsocket);
			if (SOCKET_ERROR==status)
			CCLOG("%s",YMIconv::GBK2UTF("�Ͽ�����ʧ��").c_str());

			status = WSACleanup();
			if (SOCKET_ERROR==status)
			CCLOG("%s",YMIconv::GBK2UTF("��������ʧ��").c_str());
			return;
		}
		CCLOG("%s:%s",YMIconv::GBK2UTF("���ܵ��ͻ��˷���������Ϣ������").c_str(),buffer);
		CCLOG("%s",YMIconv::GBK2UTF("������Ҫ���͸��ͻ��˷��͵���Ϣ���ݲ����س�����").c_str());

		int numsnt;
		std::string toSendtxt=YMIconv::GBK2UTF("������������Ϣ");
		
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
		
		CCLOG("%s",YMIconv::GBK2UTF("�ȴ��������ܿͻ��˷�������Ϣ......").c_str());
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