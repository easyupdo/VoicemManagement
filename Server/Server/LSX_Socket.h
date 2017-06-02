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

typedef struct Msg_Command800//800 command
{
	int m800Body_length;
	//char clear_text[CLEAR_TEXT_LENGTH];
	string m800Body;
}Msg_Command800;                                      

typedef struct Msg_Command801
{
	int m801_summary_length;//4Byte
	byte m801serverRemark[16];//16Byte //need a 16HexToString 32*4bit>>16Byte String
	int m801Body_length;//4Byte
	string m801Body;//801明文 128byte

}Msg_Command801;

typedef struct Msg_Result801
{
	int time_length;
	byte time[17];
	int remark_length;
	byte voiceRemark[16];
}Msg_Result801;

typedef struct Msg_Command802
{
	int channel;
	int start_time_length;//冗余
	byte start_time[17];
	int stop_time_length;
	byte stop_time[17];
}Msg_Command802;

typedef struct Msg_Result802
{
	int record_number;

	byte start1_time[17];
	int record1_length;
	byte start2_time[17];
	int record2_length;
	byte start3_time[17];
	int record3_length;
	byte start4_time[17];
	int record4_length;
	byte start5_time[17];
	int record5_length;
	byte start6_time[17];
	int record6_length;
	byte start7_time[17];
	int record7_length;
	byte start8_time[17];
	int record8_length;
	byte start9_time[17];
	int record9_length;
	byte start10_time[17];
	int record10_length;
	int CID_length;
	char CID[310];
}Msg_Result802;

typedef struct Msg_Command803
{
	//NO
}Msg_Command803;

typedef struct Msg_Command804
{
	int recoder_id;
	int time_offset;
}Msg_Command804;


typedef struct Msg_Command805
{
	int result;
	int file_length;
}Msg_Command805;

typedef struct Msg_Command806
{
	int file_length;
	byte voice[800];  //MAX==800
}Msg_Command806;

typedef struct Msg_Command807
{
	int data;
}Msg_Command807;

typedef struct Msg_Command808
{
	//No
}Msg_Command808;

typedef struct Msg_Command809
{
	//No
}Msg_Command809;





typedef struct allMsgInfo
{
	Msg_Head mHead;// 目前仅仅用于调试800cmd  其他 各命令直接在初始化是生成

	Msg_Command800 mC800Body;
	Msg_Command801 mC801Body;
	Msg_Result801  mR801Body;
	Msg_Command802 mC802Body;
	Msg_Result802  mR802Body;
	Msg_Command803 mC803Body;
	Msg_Command804 mC804Body;
	Msg_Command805 mC805Body;
	Msg_Command806 mC806Body;
	Msg_Command807 mC807Body;
	Msg_Command808 mC808Body;
	Msg_Command809 mC809Body;
	
}allMsgIngo;




class LSX_socket_control
{
public:
	//virtual ~LSX_socket_control();//暂未用
	
	/*WSA init*/
	int LSX_WSA_Init();
	void LSX_WSA_Clean();

	/*tcp control*/
	int LSX_Socket();
	void LSX_Bind();
	int LSX_Listen();
	void LSX_Connect();
	int LSX_Accept();
	void LSX_Close();
	/*socket address operation*/
	void LSX_AddressHost();
	void LSX_AddressServer();
	void LSX_AddressClient();
	~LSX_socket_control()
	{}
	

	
//private:
	WSADATA wsaData;
	WORD sockVersion;
	SOCKET lSocket;//create
	sockaddr_in sin;

	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen;

	
	
	
};


class LSX_message_control
{
public:
	LSX_message_control()
	{}
	LSX_message_control(string clear_text,int id,int type);

	/*Message operation*/
	int InitMsgHead(int id, int type);
	void InitMsgBody(string clear_text);
	void AllMsg(int x);

	/*time*/
	char * GetTime();
	/*CMD init*///++++++++++++++++++++++++
	void m801CInit();
	void m801RInit();
	void m802CInit(int startTimeLength, char startTime[], int stopTimeLength, char stopTime[]);
	void m802RInit();
	void m803CInit();
	void m804CInit(int id,int second);
	void m805CInit();
	void m806CInit();
	void m807CInit();
	void m808CInit();
	void m809CInit();
	



	/*string operation*/
	std::string IntToString(int x);
	int StringToInt(string x);

	void IntToHex(byte byte_src[], int value);
	int HexToInt(byte arry[], int postion);
	void MD5Init(int &a);//目前用于测试 方法
	void test();

	/*message control*/
	void LSX_SendCmd();
	void LSX_SendData();
	void LSX_ReceiveCmd();
	void LSX_ReceiveData();

	~LSX_message_control()
	{}

	/**/
	allMsgIngo mInfo;
	string allMsg;
	string m801Msg;

	int a;//测试数据
	LSX_socket_control socketControl;
	char revData[255];//buff
	string dataBuff;
	string cmdBuff;

	string md5ToKey;

	

	byte M[1024];//command//800测试
	///char time[18];//time

	byte m802C[60];//802测试

};