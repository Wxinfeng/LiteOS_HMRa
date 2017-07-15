/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenet.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-02-27
	*
	*	版本： 		V1.1
	*
	*	说明： 		与onenet平台的数据交互接口层
	*
	*	修改记录：	V1.0：协议封装、返回判断都在同一个文件，并且不同协议接口不同。
	*				V1.1：提供统一接口供应用层使用，根据不同协议文件来封装协议相关的内容。
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "net_device.h"

//协议文件
#include "onenet.h"
#include "fault.h"
//#include "edpkit.h"

//硬件驱动
#include "bsp_usart.h"
//#include "delay.h"

//图片数据文件
//#include "image_2k.h"

//C库
#include <string.h>
#include <stdio.h>



//EdpPacket *send_pkg;	//协议包


ONETNET_INFO oneNetInfo = {"6188393", "nLC=bNhMXMF1OVE=aucGnzEhVAA=",
							"112.93.129.154", "5683",
							NULL, 0,
							0, 0, 0, 0};



							
//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	devid：创建设备的devid
//				auth_key：创建设备的masterKey或apiKey
//
//	返回参数：	无
//
//	说明：		与onenet平台建立连接，成功或会标记oneNetInfo.netWork网络状态标志
//==========================================================
void OneNet_DevLink(const char* devid, const char* auth_key)
{

//	unsigned char *dataPtr;
//	
//	printf( "OneNet_DevLink\r\n"
//                        "DEVID: %s,     APIKEY: %s\r\n"
//                        , devid, auth_key);

//#if 1
//	send_pkg = PacketConnect1(devid, auth_key);						//根据devid 和 apikey封装协议包
//#else
//	send_pkg = PacketConnect2(devid, auth_key);						//根据产品id 和 鉴权信息封装协议包
//#endif
//	
//	NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);		//上传平台
//	
//	dataPtr = NET_DEVICE_GetIPD(250);								//等待平台响应
//	if(dataPtr != NULL)
//	{
//		oneNetInfo.netWork = OneNet_EDPKitCmd(dataPtr);				//解析数据
//	}
//	
//	DeleteBuffer(&send_pkg);										//删包
//	
//	if(oneNetInfo.netWork)											//如果接入成功
//	{
//		printf( "Tips:	NetWork OK\r\n");
//		
//		oneNetInfo.errCount = 0;
//		
//		NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);					//设置为数据收发模式
//	}
//	else
//	{
//		printf( "Tips:	NetWork Fail\r\n");
//		
//		if(++oneNetInfo.errCount >= 5)								//如果超过设定次数后，还未接入平台
//		{
//			oneNetInfo.netWork = 0;
//			faultType = faultTypeReport = FAULT_NODEVICE;			//标记为硬件错误
//		}
//	}
//	
}

//==========================================================
//	函数名称：	OneNet_PushData
//
//	函数功能：	PUSHDATA
//
//	入口参数：	dst_devid：接收设备的devid
//				data：数据内容
//				data_len：数据长度
//
//	返回参数：	无
//
//	说明：		设备与设备之间的通信
//==========================================================
_Bool OneNet_PushData(const char* dst_devid, const char* data, unsigned int data_len)
{
	
//	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)		//如果网络未连接 或 不为数据收发模式
//		return 1;
//	
//	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);										//设置为命令收发模式

//	send_pkg = PacketPushdata(dst_devid, data, data_len);
//	
//	NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);						//上传平台
//	
//	DeleteBuffer(&send_pkg);														//删包
//	
//	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
	
	return 0;

}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	0-成功		1-失败
//
//	说明：		
//==========================================================
_Bool OneNet_SendData(FORMAT_TYPE type, char *devid, char *apikey, DATA_STREAM *streamArray, unsigned short streamArrayCnt)
{
	
	_Bool status = 0;
	
	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)
		return 1;
	
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);
	
//	if(type != kTypeBin)																//二进制文件吧全部工作做好，不需要执行这些
//	{
//		memset(oneNetInfo.buffer, 0, sizeof(oneNetInfo.bufferSize));
//		
//		if(EDP_LoadDataStream(type, devid, apikey, streamArray, streamArrayCnt, oneNetInfo.buffer, oneNetInfo.bufferSize))
//		{
//			if(type == 0x01 || type == 0x03 || type == 0x04)
//				send_pkg = PacketSaveJson(NULL, oneNetInfo.buffer, (SaveDataType)type);	//封包
//			else
//				send_pkg = PacketSavedataSimpleString(NULL, oneNetInfo.buffer);			//封包
//			
//			NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);					//上传数据到平台
//			
//			DeleteBuffer(&send_pkg);													//删包
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
	faultTypeReport = FAULT_NONE; //发送之后清除标记
	
	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
	
	return status;
	
}

//==========================================================
//	函数名称：	OneNet_SendData_EDPType2
//
//	函数功能：	上传二进制数据到平台
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		若是低速设备，数据量大时，建议使用网络设备的透传模式
//==========================================================
#define PKT_SIZE 1024
void OneNet_SendData_EDPType2(void)
{

//	char text[] = "{\"ds_id\":\"pic\"}";								//图片数据头
//	unsigned int len = sizeof(Array);
//	unsigned char *pImage = (unsigned char *)Array;					
//	
//	printf( "image len = %d", len);
//	
//	send_pkg = PacketSaveBin(NULL, text, len);							//一发完图片数据就HardFault_Handler，
//										//原因是如果封包时填写了devid，平台会吧上传的图片数据在下发一次，导致串口buf溢出，从而进入HardFault_Handler
//	//send_pkg = PacketSaveBin(oneNetInfo.devID, text, len);//我在串口接收里边做了防止数组访问越界的处理，这下不怕了。
//	
//	NET_DEVICE_SendData(send_pkg->_data, send_pkg->_write_pos);			//向平台上传数据点
//	
//	DeleteBuffer(&send_pkg);											//删包
//	
//	while(len > 0)
//	{
//		LOS_TaskDelay(10);												//传图时，时间间隔会大一点，这里额外增加一个延时
//		
//		if(len >= PKT_SIZE)
//		{
//			NET_DEVICE_SendData(pImage, PKT_SIZE);						//串口发送分片
//			
//			pImage += PKT_SIZE;
//			len -= PKT_SIZE;
//		}
//		else
//		{
//			NET_DEVICE_SendData(pImage, (unsigned short)len);			//串口发送最后一个分片
//			len = 0;
//		}
//	}

}

//==========================================================
//	函数名称：	OneNet_HeartBeat
//
//	函数功能：	心跳检测
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_HeartBeat(void)
{

//	unsigned char heartBeat[2] = {PINGREQ, 0}, sCount = 5;
//	unsigned char errType = 0;
//	unsigned char *dataPtr;
//	
//	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)	//如果网络为连接 或 不为数据收发模式
//		return;
//	
//	NET_DEVICE_Set_DataMode(DEVICE_HEART_MODE);									//设置为心跳收发模式
//	
//	while(--sCount)																//循环检测计数
//	{
//		NET_DEVICE_SendData(heartBeat, sizeof(heartBeat));						//向平台上传心跳请求

//		dataPtr = NET_DEVICE_GetIPD(200);										//获取数据，等到2s
//		if(dataPtr != NULL)														//如果数据指针不为空
//		{
//			if(dataPtr[0] == PINGRESP)											//第一个数据为心跳响应
//			{
//				errType = CHECK_NO_ERR;											//标记无错误
//				printf( "Tips:	HeartBeat OK\r\n");
//				break;
//			}
//		}
//		else
//		{
//			if(dataPtr[0] != PINGRESP)											//心跳未响应
//			{
//				printf( "Check Device\r\n");
//					
//				errType = NET_DEVICE_Check();									//网络设备状态检查
//			}
//		}
//			
//		LOS_TaskDelay(2);														//延时等待
//	}
//	
//	if(sCount == 0)																//超出
//	{
//		printf( "HeartBeat TimeOut\r\n");
//		
//		errType = NET_DEVICE_Check();											//网络设备状态检查
//	}
//	
//	if(errType == CHECK_CONNECTED || errType == CHECK_CLOSED || errType == CHECK_GOT_IP)
//		faultTypeReport = faultType = FAULT_PRO;								//标记为协议错误
//	else if(errType == CHECK_NO_DEVICE)
//		faultTypeReport = faultType = FAULT_NODEVICE;							//标记为设备错误
//	else
//		faultTypeReport = faultType = FAULT_NONE;								//无错误
//	
//	NET_DEVICE_ClrData();														//情况缓存
//	
//	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);									//设置为数据收发模式

}

//==========================================================
//	函数名称：	OneNet_EDPKitCmd
//
//	函数功能：	EDP协议命令解析
//
//	入口参数：	data：平台下发的数据
//
//	返回参数：	无
//
//	说明：		暂时只做了连接解析
//==========================================================
_Bool OneNet_EDPKitCmd(unsigned char *data)
{

//	if(data[0] == CONNRESP) //连接响应
//	{
//		printf( "DevLink: %d\r\n", data[3]);
//		
//		//0		连接成功
//		//1		验证失败：协议错误
//		//2		验证失败：设备ID鉴权失败
//		//3		验证失败：服务器失败
//		//4		验证失败：用户ID鉴权失败
//		//5		验证失败：未授权
//		//6		验证失败：激活码未分配
//		//7		验证失败：该设备已被激活
//		//8		验证失败：重复发送连接请求包
//		//9		验证失败：重复发送连接请求包
//		
//		if(data[3] == 0)
//			return 1;
//		else
//			return 0;
//	}
	
	return 0;

}
