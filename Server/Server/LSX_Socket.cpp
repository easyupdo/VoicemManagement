#include "stdafx.h"
#include <WinSock2.h>
#include "LSX_Socket.h"
#include <string>
#include <iostream>
#include <sstream>

#include "LSX_md5.h"

using namespace std;





void LSX_message_control::InitMsgBody()
{
	mBodyRequest.msgBody = "helloworldThisistest";
	mBodyRequest.clear_text_length = mBodyRequest.msgBody.length();

	printf("MsgBodylength=%d\n",mBodyRequest.clear_text_length);
}

void LSX_message_control::InitMsgHead()
{
	mHead.msg_id = msg_request_comm;
	mHead.msg_type = Command;
	mHead.msg_length = sizeof(mHead.msg_id) + sizeof(mHead.msg_type) + sizeof(mHead.msg_length) + mBodyRequest.clear_text_length +sizeof(mBodyRequest.clear_text_length);
	printf("length=%d\n",sizeof(mHead.msg_id));
	printf("all length=%d\n",mHead.msg_length);
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


void LSX_message_control::AllMsg()
{
	//memset(allMsg,0,1024);
	//strcpy(Msg,mHead.msg_length);
	/*string str = "123";
	cout << str << endl;
	cout << "length" << sizeof(str) <<endl;
	string str2;
	str2 = str2 + str;
	cout << str.length() << endl;
	cout << "str2" << str2 << endl;
	int num = 10;
	cout << num << endl;
	num = atoi(str.c_str());
	cout <<"string"<< num << endl;
	std::stringstream ss;
	ss << num;
	ss >> str2;
	cout <<"int"<<str2 << endl;*/
	//string allMsg;
	allMsg = IntToString(123);
	cout << allMsg << endl;


	cout << StringToInt("123456") << endl;
	allMsg = IntToString(mHead.msg_length) + IntToString(mHead.msg_type) + IntToString(mHead.msg_id);
	allMsg = allMsg + IntToString(mBodyRequest.clear_text_length) + mBodyRequest.msgBody;

	cout << "allMsg:" << allMsg << endl;

	
}

void LSX_message_control::test( int &a)
{
	MD5 MD5("abc");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	a = 100;
}


void LSX_socket_control::LSX_AddressServer(SOCKET &lSocket)
{
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(lSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
}