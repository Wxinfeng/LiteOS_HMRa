/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-27
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "net_device.h"

//Э���ļ�
#include "onenet.h"
#include "fault.h"
//#include "edpkit.h"

//Ӳ������
#include "bsp_usart.h"
//#include "delay.h"

//ͼƬ�����ļ�
//#include "image_2k.h"

//C��
#include <string.h>
#include <stdio.h>



//EdpPacket *send_pkg;	//Э���


ONETNET_INFO oneNetInfo = {"6188393", "nLC=bNhMXMF1OVE=aucGnzEhVAA=",
							"112.93.129.154", "5683",
							NULL, 0,
							0, 0, 0, 0};



							
//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	devid�������豸��devid
//				auth_key�������豸��masterKey��apiKey
//
//	���ز�����	��
//
//	˵����		��onenetƽ̨�������ӣ��ɹ������oneNetInfo.netWork����״̬��־
//==========================================================
void OneNet_DevLink(const char* devid, const char* auth_key)
{

//	unsigned char *dataPtr;
//	
//	printf( "OneNet_DevLink\r\n"
//                        "DEVID: %s,     APIKEY: %s\r\n"
//                        , devid, auth_key);

//#if 1
//	send_pkg = PacketConnect1(devid, auth_key);						//����devid �� apikey��װЭ���
//#else
//	send_pkg = PacketConnect2(devid, auth_key);						//���ݲ�Ʒid �� ��Ȩ��Ϣ��װЭ���
//#endif
//	
//	NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);		//�ϴ�ƽ̨
//	
//	dataPtr = NET_DEVICE_GetIPD(250);								//�ȴ�ƽ̨��Ӧ
//	if(dataPtr != NULL)
//	{
//		oneNetInfo.netWork = OneNet_EDPKitCmd(dataPtr);				//��������
//	}
//	
//	DeleteBuffer(&send_pkg);										//ɾ��
//	
//	if(oneNetInfo.netWork)											//�������ɹ�
//	{
//		printf( "Tips:	NetWork OK\r\n");
//		
//		oneNetInfo.errCount = 0;
//		
//		NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);					//����Ϊ�����շ�ģʽ
//	}
//	else
//	{
//		printf( "Tips:	NetWork Fail\r\n");
//		
//		if(++oneNetInfo.errCount >= 5)								//��������趨�����󣬻�δ����ƽ̨
//		{
//			oneNetInfo.netWork = 0;
//			faultType = faultTypeReport = FAULT_NODEVICE;			//���ΪӲ������
//		}
//	}
//	
}

//==========================================================
//	�������ƣ�	OneNet_PushData
//
//	�������ܣ�	PUSHDATA
//
//	��ڲ�����	dst_devid�������豸��devid
//				data����������
//				data_len�����ݳ���
//
//	���ز�����	��
//
//	˵����		�豸���豸֮���ͨ��
//==========================================================
_Bool OneNet_PushData(const char* dst_devid, const char* data, unsigned int data_len)
{
	
//	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)		//�������δ���� �� ��Ϊ�����շ�ģʽ
//		return 1;
//	
//	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);										//����Ϊ�����շ�ģʽ

//	send_pkg = PacketPushdata(dst_devid, data, data_len);
//	
//	NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);						//�ϴ�ƽ̨
//	
//	DeleteBuffer(&send_pkg);														//ɾ��
//	
//	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
	
	return 0;

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	0-�ɹ�		1-ʧ��
//
//	˵����		
//==========================================================
_Bool OneNet_SendData(FORMAT_TYPE type, char *devid, char *apikey, DATA_STREAM *streamArray, unsigned short streamArrayCnt)
{
	
	_Bool status = 0;
	
	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)
		return 1;
	
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);
	
//	if(type != kTypeBin)																//�������ļ���ȫ���������ã�����Ҫִ����Щ
//	{
//		memset(oneNetInfo.buffer, 0, sizeof(oneNetInfo.bufferSize));
//		
//		if(EDP_LoadDataStream(type, devid, apikey, streamArray, streamArrayCnt, oneNetInfo.buffer, oneNetInfo.bufferSize))
//		{
//			if(type == 0x01 || type == 0x03 || type == 0x04)
//				send_pkg = PacketSaveJson(NULL, oneNetInfo.buffer, (SaveDataType)type);	//���
//			else
//				send_pkg = PacketSavedataSimpleString(NULL, oneNetInfo.buffer);			//���
//			
//			NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);					//�ϴ����ݵ�ƽ̨
//			
//			DeleteBuffer(&send_pkg);													//ɾ��
//		}
//		else
//			status = 1;
//	}
//	else
//	{
//		printf( "Tips:	OneNet_SendData-kTypeBin\r\n");
//		
//		OneNet_SendData_EDPType2();
//	}
//	
	faultTypeReport = FAULT_NONE; //����֮��������
	
	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
	
	return status;
	
}

//==========================================================
//	�������ƣ�	OneNet_SendData_EDPType2
//
//	�������ܣ�	�ϴ����������ݵ�ƽ̨
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		���ǵ����豸����������ʱ������ʹ�������豸��͸��ģʽ
//==========================================================
#define PKT_SIZE 1024
void OneNet_SendData_EDPType2(void)
{

//	char text[] = "{\"ds_id\":\"pic\"}";								//ͼƬ����ͷ
//	unsigned int len = sizeof(Array);
//	unsigned char *pImage = (unsigned char *)Array;					
//	
//	printf( "image len = %d", len);
//	
//	send_pkg = PacketSaveBin(NULL, text, len);							//һ����ͼƬ���ݾ�HardFault_Handler��
//										//ԭ����������ʱ��д��devid��ƽ̨����ϴ���ͼƬ�������·�һ�Σ����´���buf������Ӷ�����HardFault_Handler
//	//send_pkg = PacketSaveBin(oneNetInfo.devID, text, len);//���ڴ��ڽ���������˷�ֹ�������Խ��Ĵ������²����ˡ�
//	
//	NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);			//��ƽ̨�ϴ����ݵ�
//	
//	DeleteBuffer(&send_pkg);											//ɾ��
//	
//	while(len > 0)
//	{
//		LOS_TaskDelay(10);												//��ͼʱ��ʱ�������һ�㣬�����������һ����ʱ
//		
//		if(len >= PKT_SIZE)
//		{
//			NET_DEVICE_SendData(pImage, PKT_SIZE);						//���ڷ��ͷ�Ƭ
//			
//			pImage += PKT_SIZE;
//			len -= PKT_SIZE;
//		}
//		else
//		{
//			NET_DEVICE_SendData(pImage, (unsigned short)len);			//���ڷ������һ����Ƭ
//			len = 0;
//		}
//	}

}

//==========================================================
//	�������ƣ�	OneNet_HeartBeat
//
//	�������ܣ�	�������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_HeartBeat(void)
{

//	unsigned char heartBeat[2] = {PINGREQ, 0}, sCount = 5;
//	unsigned char errType = 0;
//	unsigned char *dataPtr;
//	
//	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)	//�������Ϊ���� �� ��Ϊ�����շ�ģʽ
//		return;
//	
//	NET_DEVICE_Set_DataMode(DEVICE_HEART_MODE);									//����Ϊ�����շ�ģʽ
//	
//	while(--sCount)																//ѭ��������
//	{
//		NET_DEVICE_SendData(heartBeat, sizeof(heartBeat));						//��ƽ̨�ϴ���������

//		dataPtr = NET_DEVICE_GetIPD(200);										//��ȡ���ݣ��ȵ�2s
//		if(dataPtr != NULL)														//�������ָ�벻Ϊ��
//		{
//			if(dataPtr[0] == PINGRESP)											//��һ������Ϊ������Ӧ
//			{
//				errType = CHECK_NO_ERR;											//����޴���
//				printf( "Tips:	HeartBeat OK\r\n");
//				break;
//			}
//		}
//		else
//		{
//			if(dataPtr[0] != PINGRESP)											//����δ��Ӧ
//			{
//				printf( "Check Device\r\n");
//					
//				errType = NET_DEVICE_Check();									//�����豸״̬���
//			}
//		}
//			
//		LOS_TaskDelay(2);														//��ʱ�ȴ�
//	}
//	
//	if(sCount == 0)																//����
//	{
//		printf( "HeartBeat TimeOut\r\n");
//		
//		errType = NET_DEVICE_Check();											//�����豸״̬���
//	}
//	
//	if(errType == CHECK_CONNECTED || errType == CHECK_CLOSED || errType == CHECK_GOT_IP)
//		faultTypeReport = faultType = FAULT_PRO;								//���ΪЭ�����
//	else if(errType == CHECK_NO_DEVICE)
//		faultTypeReport = faultType = FAULT_NODEVICE;							//���Ϊ�豸����
//	else
//		faultTypeReport = faultType = FAULT_NONE;								//�޴���
//	
//	NET_DEVICE_ClrData();														//�������
//	
//	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);									//����Ϊ�����շ�ģʽ

}

//==========================================================
//	�������ƣ�	OneNet_EDPKitCmd
//
//	�������ܣ�	EDPЭ���������
//
//	��ڲ�����	data��ƽ̨�·�������
//
//	���ز�����	��
//
//	˵����		��ʱֻ�������ӽ���
//==========================================================
_Bool OneNet_EDPKitCmd(unsigned char *data)
{

//	if(data[0] == CONNRESP) //������Ӧ
//	{
//		printf( "DevLink: %d\r\n", data[3]);
//		
//		//0		���ӳɹ�
//		//1		��֤ʧ�ܣ�Э�����
//		//2		��֤ʧ�ܣ��豸ID��Ȩʧ��
//		//3		��֤ʧ�ܣ�������ʧ��
//		//4		��֤ʧ�ܣ��û�ID��Ȩʧ��
//		//5		��֤ʧ�ܣ�δ��Ȩ
//		//6		��֤ʧ�ܣ�������δ����
//		//7		��֤ʧ�ܣ����豸�ѱ�����
//		//8		��֤ʧ�ܣ��ظ��������������
//		//9		��֤ʧ�ܣ��ظ��������������
//		
//		if(data[3] == 0)
//			return 1;
//		else
//			return 0;
//	}
	
	return 0;

}
