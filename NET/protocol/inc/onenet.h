#ifndef _ONENET_H_
#define _ONENET_H_


#include "edp_stream.h"




typedef struct
{

    char devID[15];
    char apiKey[35];
	
	char ip[16];
	char port[8];
	
	char *buffer;
	unsigned short bufferSize;	//
	
	unsigned char netWork : 1; 	//1-OneNET����ɹ�		0-OneNET����ʧ��
	unsigned char sendData : 1;
	unsigned char errCount : 3; //�������
	unsigned char reverse : 3;

} ONETNET_INFO;

extern ONETNET_INFO oneNetInfo;





#define CHECK_CONNECTED			0	//������
#define CHECK_CLOSED			1	//�ѶϿ�
#define CHECK_GOT_IP			2	//�ѻ�ȡ��IP
#define CHECK_NO_DEVICE			3	//���豸
#define CHECK_INITIAL			4	//��ʼ��״̬
#define CHECK_NO_CARD			5	//û��sim��
#define CHECK_NO_ERR			255 //

#define DEVICE_CMD_MODE			0 //AT����ģʽ
#define DEVICE_DATA_MODE		1 //ƽ̨�����·�ģʽ
#define DEVICE_HEART_MODE		2 //��������ģʽ




void OneNet_DevLink(const char* devid, const char* auth_key);

_Bool OneNet_SendData(FORMAT_TYPE type, char *devid, char *apikey, DATA_STREAM *streamArray, unsigned short streamArrayCnt);

_Bool OneNet_PushData(const char* dst_devid, const char* data, unsigned int data_len);

void OneNet_SendData_EDPType2(void);

void OneNet_HeartBeat(void);

_Bool OneNet_EDPKitCmd(unsigned char *data);


#endif
