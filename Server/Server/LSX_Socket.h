#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;


#define CLEAR_TEXT_LENGTH 100

#define Command 0
#define Result 1

#define msg_request_comm 800
#define msg_request_return 801

typedef struct Msg_Head
{
	int msg_length;
	int msg_type;
	int msg_id;
}Msg_Head;

typedef struct Msg_Request_Body
{
	int clear_text_length;
	//char clear_text[CLEAR_TEXT_LENGTH];
	string msgBody;
}Msg_Request_Body;


class LSX_message_control
{
public:
	/*Message operation*/
	void InitMsgHead();
	void InitMsgBody();
	void AllMsg();

	/*string operation*/
	std::string IntToString(int x);
	int StringToInt(string x);
	void test(int &a);//目前用于测试的方法

	/*message control*/
	void LSX_SendCmd();
	void LSX_SendData();
	void LSX_ReceiveCmd();
	void LSX_ReceiveData();

	/**/
	Msg_Request_Body mBodyRequest;
	Msg_Head mHead;
	string allMsg;
	int a;//测试数据

};

class LSX_socket_control
{
public:
	//virtual ~LSX_socket_control();//暂未用
	
	/*WSA init*/
	void LSX_WSA_Init();
	void LSX_WSA_Clean();

	/*tcp control*/
	void LSX_Socket();
	void LSX_Bind();
	void LSX_Listen();
	void LSX_Connect();
	void LSX_Accept();
	void LSX_Close();

	/*socket address operation*/
	void LSX_AddressHost();
	void LSX_AddressServer(SOCKET &lSocket);
	void LSX_AddressClient();
	

	
//private:
	LSX_message_control message;
};


