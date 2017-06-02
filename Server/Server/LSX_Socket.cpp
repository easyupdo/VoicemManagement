#include "stdafx.h"
#include <WinSock2.h>
#include "LSX_Socket.h"
#include <string>
#include <iostream>
#include <sstream>

#include "LSX_md5.h"

using namespace std;


char* LSX_message_control::GetTime()
{
	//声明变量
	SYSTEMTIME sys_time;
//BUG
	//将变量值设置为本地时间
	GetLocalTime(&sys_time);
	
	char time[18];//生成时间字符串
	sprintf(time, "%04d", sys_time.wYear);//year
	sprintf(time + 4, "%02d", sys_time.wMonth);
	sprintf(time + 6, "%02d", sys_time.wDay);
	sprintf(time + 8, "%02d", sys_time.wHour);
	sprintf(time + 10, "%02d", sys_time.wMinute);
	sprintf(time + 12, "%02d", sys_time.wSecond);
	sprintf(time + 14, "%s", "000");
	cout << "时间:" << time << endl;
	cout << "时间:" << (byte*)time << endl;
	return time;//返回时间的字符串
}

LSX_message_control::LSX_message_control(string clear_text, int id, int type)//参数用于测试集成接口
{
	//************************************MSG******************************
	/*800 Command*/
	mInfo.mC800Body.m800Body_length = clear_text.length();
	mInfo.mC800Body.m800Body = clear_text;
	cout << "Body_length:" << mInfo.mC800Body.m800Body_length << endl;
	int msg_length = InitMsgHead(id, type);
	AllMsg(msg_length);
	
	/*801 Command*/

	/*801 Result*/
	m801RInit();
	test();
	
	/*802*/
	//m802CInit();
	/*803...*/




}
/*
//仪>>服 摘要：经过skey对800加密   //客户端功能
void LSX_message_control::m801CInit()
{
	mInfo.mC801Body.m801_summary_length =16;//16byte
	mInfo.mC801Body.m801serverRemark;
	mInfo.mC801Body.m801Body_length;
	mInfo.mC801Body.m801Body;
}
*/

//服>>仪 摘要 ：通过801 command中的明文 使用rkey加密
void LSX_message_control::m801RInit()
{
	MD5 MD5("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	mInfo.mR801Body.time_length = 17;////17Byte
	strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//当前时间 发送给 仪
	mInfo.mR801Body.remark_length = 16;//16Byte
	strcpy((char*)(mInfo.mR801Body.voiceRemark), (char*)MD5.getResult());//保存的是16进制
	
	char buf[33];
	for (int i = 0; i<16; i++)
		sprintf(buf + i * 2, "%02x", mInfo.mR801Body.voiceRemark[i]);//将16Byte的16进制的摘要转为字符串输出
	buf[32] = 0; //将16进制转字符输出

		cout << "服务器验证md5：" << buf<< endl;


		cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
		strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//start serach time //17byte
		cout << "开始时间：" << mInfo.mC802Body.start_time << endl;
}


//服务器>>仪器
void LSX_message_control:: m802CInit(int startTimeLength,char startTime[],int stopTimeLength,char stopTime[])
{
	//用户查询某一时间段中的息信test
	mInfo.mC802Body.channel = 1;
	strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//start serach time //17byte
	cout << "开始时间：" << mInfo.mC802Body.start_time << endl;
	mInfo.mC802Body.start_time_length = startTimeLength;
	mInfo.mC802Body.stop_time_length=stopTimeLength;
	strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//over serach time //17byte
	mInfo.mC802Body.stop_time;


	Msg_Head m802Head;
	m802Head.msg_id = 802;
	m802Head.msg_type = 0;
	m802Head.msg_length = 58;

	



}


//仪>>服  返回服务器查询信息 Result
/*
void LSX_message_control::m802RInit()
{
	mInfo.mR802Body.record_number;
	mInfo.mR802Body.start1_time;
	mInfo.mR802Body.record1_length;	
}
*/


//仪器>>服务器
void LSX_message_control::m803CInit()
{
	//NO body
}

//服务器>>仪器  获取具体某一条id的语音记录信息
void LSX_message_control::m804CInit(int id,int second)
{
	mInfo.mC804Body.recoder_id=id;//索引 1-100
	mInfo.mC804Body.time_offset = second;//秒
}

//仪器>>服务器  通知服务器查询具体id语音记录的结果
void LSX_message_control::m805CInit()
{
	mInfo.mC805Body.result;
	mInfo.mC805Body.file_length;

}

//仪器>>服务器  发送语音信息
void LSX_message_control::m806CInit()
{
	
}


//仪器>>服务器 通知服务器结束
void LSX_message_control::m807CInit()
{
	mInfo.mC807Body.data;//1 over 2 exception over
}


//服务器>>仪器 通知语音记录仪停止传送语音数据
void LSX_message_control::m808CInit()
{

}


//服务器>>仪器	心跳包
void LSX_message_control::m809CInit(){}



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
	byte byte_src[4];//transform 
	memset(M,0,sizeof(M));
	IntToHex(byte_src,x);//144  00：00：00：90
	for (int i = 0; i < 4; i++)
	{
		M[i] = byte_src[i];
		printf("KKK:%02x\n", M[i]);
	}
		

	IntToHex(byte_src,mInfo.mHead.msg_type);//0
	for (int i = 4; i < 8; i++)
	{
		M[i] = byte_src[i-4];
		printf("KKK:%02x\n", M[i]);
	}
		

	IntToHex(byte_src,mInfo.mHead.msg_id);//800   00：00：00：03：20
	for (int i = 8; i < 12; i++)
	{
		M[i] = byte_src[i-8];
		printf("KKK:%02x\n", M[i]);
	}

	IntToHex(byte_src,mInfo.mC800Body.m800Body_length);//128   00:00:00:80
	for (int i = 12; i < 16; i++)
	{
		M[i] = byte_src[i-12];
		printf("KKK:%02x\n", M[i]);
	}

	
	//int X = mInfo.m800Body.m800Body.length();
	char buf[129];//128+1

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

void  LSX_message_control::IntToHex(byte byte_src[], int value)//big  大端
{

	//byte byte_src[4];//transform 
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




void LSX_message_control::test()
{
	
	byte byte_src[4];
	IntToHex(byte_src, 58);
	for (int i = 0; i < 4; i++)
	{
		m802C[i] = byte_src[i];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 0);
	for (int i = 4; i < 8; i++)
	{
		m802C[i] = byte_src[i-4];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 802);
	for (int i = 8; i < 12; i++)
	{
		m802C[i] = byte_src[i-8];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 1);//通道
	for (int i = 12; i < 16; i++)
	{
		m802C[i] = byte_src[i-12];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 17);//长度0x11
	for (int i = 16; i < 20; i++)
	{
		m802C[i] = byte_src[i-16];
		printf("KKK:%02x\n", m802C[i]);
	}
	//11111BUG

	for (int i = 0; i < 17; i++)
		sprintf((char*)m802C + 20 + i, "%c", (mInfo.mC802Body.start_time[i]));

	IntToHex(byte_src, 17);
	for (int i = 37; i < 41; i++)
	{
		m802C[i] = byte_src[i-37];
		printf("************KKK:%02x\n", m802C[i]);
	}

	for (int i = 0; i<17; i++)
		sprintf((char *)m802C + 41 + i, "%c", (mInfo.mC802Body.start_time[i]));

	printf("802:\n");
	for (int i = 0; i < 58; i++)
	{
		printf("%02x:", m802C[i]);
	}

}
void LSX_message_control::MD5Init( int &a)
{
	//MD5 MD5("abc");
	//string result = MD5.md5();
	//cout << "abc MD5 is:" << result << endl;
	a = 100;
	/*
	this->md5ToKey = (char*)MD5.getResult();
	cout << "NNNNNNNNNNNN:" << md5ToKey << endl;
	*/
	//	IntToHex(86987456);

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

	for (int i = 0; i < 144; i++)
	{
		printf("%02x:", (byte)revData[i]);
		//cout << "=================" << tmp[i] << endl;
	}

	printf("\n");
	printf("解码：");
	printf("%d", HexToInt((byte*)revData, 0));
	printf("%d", HexToInt((byte*)revData, 4));
	printf("%d", HexToInt((byte*)revData, 8));
	printf("%d", HexToInt((byte*)revData, 12));


	/*
	for (int i = 16; i < 144; i++)
	{
		dbuf[i - 16] = revData[i];
		printf("%c", revData[i]);
	}

	cout << "++++:" << dbuf << endl;
	*/

}

void LSX_message_control::LSX_SendCmd()
{
	//send(socketControl.sClient, (char*)m802C, 58, 0);//802
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

