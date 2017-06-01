#include "stdafx.h"
#include <WinSock2.h>
#include "LSX_Socket.h"
#include <string>
#include <iostream>
#include <sstream>

#include "LSX_md5.h"

using namespace std;




LSX_message_control::LSX_message_control(string clear_text, int id, int type)
{
	/*800 Command*/
	mInfo.mC800Body.m800Body_length = clear_text.length();
	mInfo.mC800Body.m800Body = clear_text;
	cout << "Body_length:" << mInfo.mC800Body.m800Body_length << endl;
	int msg_length = InitMsgHead(id, type);
	AllMsg(msg_length);


	/*801 Command*/
	//mInfo.mC801Body.m
	/*802*/
	m802CInit();
	/*803...*/

}

void LSX_message_control:: m802CInit()
{
	mInfo.mC802Body.channel = 1;
	mInfo.mC802Body.start_time;
	mInfo.mC802Body.start_time_length;
	mInfo.mC802Body.stop_time;
	mInfo.mC802Body.stop_time_length;

}


int LSX_message_control::InitMsgHead(int id,int type)
{
	mInfo.mHead.msg_id= id;
	mInfo.mHead.msg_type =type;
	mInfo.mHead.msg_length = sizeof(mInfo.mHead.msg_id) + sizeof(mInfo.mHead.msg_type) + sizeof(mInfo.mHead.msg_length) + mInfo.mC800Body.m800Body_length + sizeof(mInfo.mC800Body.m800Body_length);

	cout << "allMsg_length:" << mInfo.mHead.msg_length << endl;
	return mInfo.mHead.msg_length;
}

string LSX_message_control::IntToString(int x)
{
	string str;
	std::stringstream tmp;
	tmp << x;
	tmp >> str;
	return str;
}

int LSX_message_control:: StringToInt(string x)
{
	return (atoi(x.c_str()));
}





void LSX_message_control::AllMsg(int x)
{
	//memset(M,0,sizeof(M));
	IntToHex(x);//144  00：00：00：90
	for (int i = 0; i < 4; i++)
	{
		M[i] = byte_src[i];
		printf("KKK:%02x\n", M[i]);
	}
		

	IntToHex(mInfo.mHead.msg_type);//0
	for (int i = 4; i < 8; i++)
	{
		M[i] = byte_src[i-4];
		printf("KKK:%02x\n", M[i]);
	}
		

	IntToHex(mInfo.mHead.msg_id);//800   00：00：00：03：20
	for (int i = 8; i < 12; i++)
	{
		M[i] = byte_src[i-8];
		printf("KKK:%02x\n", M[i]);
	}

	IntToHex(mInfo.mC800Body.m800Body_length);//128   00:00:00:80
	for (int i = 12; i < 16; i++)
	{
		M[i] = byte_src[i-12];
		printf("KKK:%02x\n", M[i]);
	}

	
	//int X = mInfo.m800Body.m800Body.length();
	char buf[128];

	//strcpy(letter, word.c_str());
	strcpy(buf, mInfo.mC800Body.m800Body.c_str());

	cout << "buf:" <<buf<< endl;


	for (int i = 16; i < 144; i++)
	{
		M[i] = buf[i-16];
	}
	//head
	for (int i =0; i <16;i++)
	printf("%02x:",M[i]);

	printf("\n");
	printf("body: ");
	//msg body
	for (int i = 16; i < 144; i++)
		printf("%02x：",M[i]);


	//To String
	
	//cout << "allMsg:%c" << M << endl;
	printf("allMsg:%s",M);

	
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 0));
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 4));
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 8));

	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 12));
	
}

void LSX_message_control::IntToHex(int value)//big  大端
{

	/*int to hex for trasform*/
	memset(byte_src,0,4);
	byte_src[0] = (byte)((value & 0xFF000000) >> 24);
	byte_src[1] = (byte)((value & 0x00FF0000) >> 16);
	byte_src[2] = (byte)((value & 0x0000FF00) >> 8);
	byte_src[3] = (byte)((value & 0x000000FF));

	for (int i = 0; i < 4;i++)
	printf("%02x:%d||",byte_src[i],byte_src[i]);
}

int LSX_message_control::HexToInt(byte arry[], int postion)//big 大端
{
	int value;
	value = (int)((arry[postion+3] & 0xFF)
		| ((arry[postion + 2] << 8) & 0xFF00)
		| ((arry[postion + 1] << 16) & 0xFF0000)
		| ((arry[postion + 0] << 24) & 0xFF000000));
	return value;
}




void LSX_message_control::test( int &a)
{
	MD5 MD5("abc");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	a = 100;
	
	this->md5ToKey = (char*)MD5.getResult();
	cout << "NNNNNNNNNNNN:" << md5ToKey << endl;

	IntToHex(86987456);

	



}


void LSX_socket_control::LSX_AddressServer()
{
	//绑定IP和端口
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	/*
	if (bind(lSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	*/
	
}




int LSX_socket_control::LSX_WSA_Init()
{
	//初始化WSA
	//WSADATA wsaData;
	sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
}
int LSX_socket_control::LSX_Socket()
{
	//创建套接字
	lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lSocket == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
}

void LSX_socket_control::LSX_Bind()
{
	//绑定服务器地址
	if (bind(lSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
}

int LSX_socket_control::LSX_Listen()
{
	//开始监听
	if (listen(lSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
}

int LSX_socket_control::LSX_Accept()
{
	sClient = accept(lSocket, (SOCKADDR *)&(remoteAddr), &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
	}
	return 0;
}

void LSX_message_control::LSX_ReceiveData()
{
	int ret = recv(socketControl.sClient,revData, 255, 0);
	if (ret > 0)
	{
		revData[ret] = 0x00;
		printf(revData);
	}
}

void LSX_message_control::LSX_SendCmd()
{
	send(socketControl.sClient, (char*)M, 144, 0);
}

void LSX_socket_control::LSX_Close()
{
	closesocket(sClient);
}
void LSX_socket_control::LSX_WSA_Clean()
{
	WSACleanup();
}

