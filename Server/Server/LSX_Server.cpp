// Server.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include "LSX_Socket.h"

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argc, char* argv[])
{	
	int num = 111;
	LSX_socket_control socketControl;
	socketControl.message.InitMsgBody();
	socketControl.message.InitMsgHead();
	
	socketControl.message.AllMsg();
	socketControl.message.test(num);
	cout << "=====" << num << endl;
	
	
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}


	//绑定IP和端口
	/*
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	*/
	socketControl.LSX_AddressServer(slisten);


	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据
		while (true)
		{
		
			int ret = recv(sClient, revData, 255, 0);
			if (ret > 0)
			{
				revData[ret] = 0x00;
				printf(revData);
			}

			//发送数据
			char * sendData = "你好，TCP客户端！\n";
			const char *p = socketControl.message.allMsg.data();
			send(sClient, p, strlen(sendData), 0);
		}
			closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}
