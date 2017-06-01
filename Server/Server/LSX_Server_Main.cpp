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
	LSX_message_control message("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh",800,0);
	message.test(num);
	cout << "numtest=====" << num << endl;
	
	
	message.socketControl.LSX_WSA_Init();

	message.socketControl.LSX_Socket();
	message.socketControl.LSX_AddressServer();
	message.socketControl.LSX_Bind();
	message.socketControl.LSX_Listen();

	

	//循环接收数据
	
	message.socketControl.nAddrlen = sizeof(message.socketControl.remoteAddr);
	
	while (true)
	{
		printf("等待连接...\n");
		message.socketControl.LSX_Accept();
		printf("接受到一个连接：%s \r\n", inet_ntoa(message.socketControl.remoteAddr.sin_addr));

		//接收数据
		while (true)
		{
		
			message.LSX_ReceiveData();
			message.LSX_SendCmd();

		}
		message.socketControl.LSX_Close();
	}
	message.socketControl.LSX_Close();
	message.socketControl.LSX_WSA_Clean();
	return 0;
}
