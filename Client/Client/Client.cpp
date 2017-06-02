// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
#include <string.h>
#include "message.h"
#include "md5.h"

using namespace std;
#pragma  comment(lib,"ws2_32.lib")
char dbuf[128];

byte m801C[1024];
byte byte_src[4];
Msg_Head mhead;
Msg_Command801 m801CBody;

void IntToHex(byte byte_src[], int value)//big  大端
{

	//byte byte_src[4];//transform 
	/*int to hex for trasform*/
	memset(byte_src, 0, 4);
	byte_src[0] = (byte)((value & 0xFF000000) >> 24);
	byte_src[1] = (byte)((value & 0x00FF0000) >> 16);
	byte_src[2] = (byte)((value & 0x0000FF00) >> 8);
	byte_src[3] = (byte)((value & 0x000000FF));

	for (int i = 0; i < 4; i++)
		printf("%02x:%d||", byte_src[i], byte_src[i]);
}


void m801CInit(char mdbuf[])
{
	char mark[16];
	
	MD5 MD5(mdbuf);
	string result = MD5.md5();
	cout << "客户端md5:"<<result<<endl;
	mark,MD5.getResult();

	m801CBody.m801_summary_length = 16;
	strcpy((char*)m801CBody.m801serverRemark, (char*)MD5.getResult());
	m801CBody.m801Body_length = 128;
	//m801CBody.m801Body = "1234567890";

	mhead.msg_type = 0;
	mhead.msg_id = 801;
	mhead.msg_length = 154;

	
	memset(m801C,0,1024);
	IntToHex(byte_src, 154);
	for (int i = 0; i < 4; i++)
	{
		m801C[i] = byte_src[i];
		printf("KKK:%02x\n", m801C[i]);
	}
	IntToHex(byte_src, 0);
	for (int i = 4; i < 8; i++)
	{
		m801C[i] = byte_src[i-4];
		printf("KKK:%02x\n", m801C[i]);
	}
	IntToHex(byte_src, 801);
	for (int i = 8; i < 12; i++)
	{
		m801C[i] = byte_src[i-8];
		printf("KKK:%02x\n", m801C[i]);
	}

	IntToHex(byte_src, 16);
	for (int i = 12; i < 16; i++)
	{
		m801C[i] = byte_src[i-12];
		printf("KKK:%02x\n", m801C[i]);
	}

	//cout << "DDDDD:" << (char*)MD5.getResult() << endl;

	for (int i = 0; i < 16; i++)
		sprintf((char*)m801C + 16 + i, "%c", (m801CBody.m801serverRemark[i]));

	
	IntToHex(byte_src, 128);
	for (int i = 32; i < 36; i++)
	{
		m801C[i] = byte_src[i];
		printf("KKK:%02x\n", m801C[i]);
	}

	cout << "wocao " << endl;

}






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

	
	
	for (int i = 16; i < 144; i++)
	{
		dbuf[i - 16] = recData[i];
		printf("%c", recData[i]);
	}

	cout << "++++:" << dbuf << endl;
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
		send(sclient, (char*)m801C,36, 0);

		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
//			recData[ret] = 0x00;
			printf(recData);
		}
	//	if ()
		Analysis(recData);
		m801CInit(dbuf);
		getchar();

	}
	
	closesocket(sclient);
	WSACleanup();
	return 0;
}


