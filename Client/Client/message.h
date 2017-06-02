#ifndef _MESSAGE_H
#define _MESSAGE_H
#endif

#include <iostream>
using namespace std;

typedef unsigned char byte;
typedef struct Msg_Head
{
	int msg_length;
	int msg_type;
	int msg_id;
}Msg_Head;

//Result 801
typedef struct Msg_Command801
{
	int m801_summary_length;//4Byte
	byte m801serverRemark[20];//16Byte //need a 16HexToString 32*4bit>>16Byte String
	int m801Body_length;//4Byte
	string m801Body;//801明文 128byte

}Msg_Command801;


//仪器>>服务器 Result
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

//仪器>>服务器  通知服务器查询具体id语音记录的结果
typedef struct Msg_Command805
{
	int result;
	int file_length;
}Msg_Command805;


//仪器>>服务器  发送语音信息
typedef struct Msg_Command806
{
	int file_length;
	byte voice[800];  //MAX==800
}Msg_Command806;


//仪器>>服务器 通知服务器结束
typedef struct Msg_Command807
{
	int data;
}Msg_Command807;


void m801CInit();
void m801RInit();
void m803CInit();
void m805CInit();

void m806CInit();
void m807CInit();