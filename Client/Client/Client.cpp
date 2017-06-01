// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
using namespace std;
#pragma  comment(lib,"ws2_32.lib")



int HexToInt(byte arry[], int postion)//big 大端
{
	int value;
	value = (int)((arry[postion + 3] & 0xFF)
		| ((arry[postion + 2] << 8) & 0xFF00)
		| ((arry[postion + 1] << 16) & 0xFF0000)
		| ((arry[postion + 0] << 24) & 0xFF000000));
	return value;
}


void Analysis(char recData[])
{
	int id;
	char *p;
	p = recData;
	char tmp[4];
	int i;

	for (i = 0; i < 144; i++)
	{
		printf("%02x:",(byte)recData[i]);
		//cout << "=================" << tmp[i] << endl;
	}

	printf("\n");
	printf("解码：");
	printf("%d", HexToInt((byte*)recData,0));
	printf("%d", HexToInt((byte*)recData, 4));
	printf("%d", HexToInt((byte*)recData, 8));
	printf("%d", HexToInt((byte*)recData, 12));
	for (int i = 16; i < 144;i++)
	printf("%c",recData[i]);
	
}


int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	while (1)
	{
		char * sendData = "你好，TCP服务端，我是客户端!\n";
		send(sclient, sendData, strlen(sendData), 0);

		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
			recData[ret] = 0x00;
			printf(recData);
		}
	//	if ()
		Analysis(recData);
		getchar();
	}
	
	closesocket(sclient);
	WSACleanup();
	return 0;
}


