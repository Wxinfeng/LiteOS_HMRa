/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	net_device.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-03-02
	*
	*	版本： 		V1.1
	*
	*	说明： 		网络设备应用层
	*
	*	修改记录：	V1.1：1.平台IP和PORT通过参数传入的方式确定，解决了不同协议网络设备驱动不通用的问题。
	*					  2.取消了手动配置网络，上电等待wifi模块自动连接，若不成功则使用OneNET公众号进行配网。
	*					  3.NET_DEVICE_SendCmd新增参数“mode”，决定是否清除本次命令的返回值。
	************************************************************
	************************************************************
	************************************************************
**/

#include "stm32f10x.h"	//单片机头文件

#include "net_device.h"	//网络设备应用层
#include "net_io.h"		//网络设备数据IO层

//硬件驱动
//#include "delay.h"
#include "bsp_led.h"
#include "key.h"
#include "bsp_usart.h"

#include "los_task.h"

//C库
#include <string.h>
#include <stdlib.h>
#include <stdio.h>




NET_DEVICE_INFO netDeviceInfo = {0, 0, 0, 0, 0}; //





//==========================================================
//	函数名称：	NET_DEVICE_IO_Init
//
//	函数功能：	初始化网络设备IO层
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		初始化网络设备的控制引脚、数据收发功能等
//==========================================================
void NET_DEVICE_IO_Init(void)
{

	GPIO_InitTypeDef gpioInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//ESP8266复位引脚
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_0;					//GPIOA0-复位
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	NET_IO_Init();											//网络设备数据IO层初始化
	
	netDeviceInfo.reboot = 0;

}

#if(NET_DEVICE_TRANS == 1)
//==========================================================
//	函数名称：	ESP8266_QuitTrans
//
//	函数功能：	退出透传模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		连续发送三个‘+’，然后关闭透传模式
//==========================================================
void ESP8266_QuitTrans(void)
{

	while((NET_IO->SR & 0X40) == 0);	//等待发送空
	NET_IO->DR = '+';
	LOS_TaskDelay(3); 					//大于串口组帧时间(10ms)
	
	while((NET_IO->SR & 0X40) == 0);	//等待发送空
	NET_IO->DR = '+';        
	LOS_TaskDelay(3); 					//大于串口组帧时间(10ms)
	
	while((NET_IO->SR & 0X40) == 0);	//等待发送空
	NET_IO->DR = '+';        
	LOS_TaskDelay(20);					//等待100ms
	
	NET_DEVICE_SendCmd("AT+CIPMODE=0\r\n", "OK", 1); //关闭透传模式

}

//==========================================================
//	函数名称：	ESP8266_EnterTrans
//
//	函数功能：	进入透传模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_EnterTrans(void)
{
	
	NET_DEVICE_SendCmd("AT+CIPMUX=0\r\n", "OK", 1);	//单链接模式

	NET_DEVICE_SendCmd("AT+CIPMODE=1\r\n", "OK", 1);//使能透传
	
	NET_DEVICE_SendCmd("AT+CIPSEND\r\n", ">", 1);	//发送数据
	
	LOS_TaskDelay(20);								//等待100ms

}

#endif

//==========================================================
//	函数名称：	NET_DEVICE_Exist
//
//	函数功能：	网络设备存在检查
//
//	入口参数：	无
//
//	返回参数：	返回结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_Exist(void)
{

	unsigned char timeOut = 50;
	_Bool status = 1;
	unsigned char count = 0;
	
	NET_DEVICE_ClrData();
	
	while(timeOut--)												//等待
	{
		LOS_TaskDelay(20);											//挂起等待
		
//		if(KeyScan(GPIOC, KEY0) == KEYDOWN)							//长按进入配网模式
//			count++;
//		else
//			count = 0;
		
		if((NET_IO_WaitRecive() == REV_OK) || count >= 20)			//如果收到数据
		{
			if((strstr((char *)netIOInfo.buf, "WIFI GOT IP")) && (count == 0))
			{
				status = 0;
			}
			else if(strstr((char *)netIOInfo.buf, "WIFI DISCONNECT") || count >= 20)
			{
				count = 0;
#if(PHONE_AP_MODE == 0)
				NET_DEVICE_SendCmd("AT+CWSMARTSTART=2\r\n", "OK", 1);
				printf( "请使用OneNET公众号配置SSID和PSWD\r\n");
				NET_DEVICE_ClrData();
				
				while(1)
				{
//					Led4_Set(LED_ON);Led5_Set(LED_ON);Led6_Set(LED_ON);Led7_Set(LED_ON);
					
					if(strstr((char *)netIOInfo.buf, "SMART SUCCESS"))
					{
						printf( "收到:\r\n%s\r\n", strstr((char *)netIOInfo.buf, "SSID:"));
						NET_DEVICE_ClrData();
						status = 1;
//						Led4_Set(LED_OFF);Led5_Set(LED_OFF);Led6_Set(LED_OFF);Led7_Set(LED_OFF);
						break;
					}
					
					LOS_TaskDelay(100);
//					Led4_Set(LED_OFF);Led5_Set(LED_OFF);Led6_Set(LED_OFF);Led7_Set(LED_OFF);
					LOS_TaskDelay(100);
				}
				
				NET_DEVICE_Reset();															//复位一下，不然不会自动连接
#else
				printf( "STA Tips:	Link Wifi\r\n");
				
				while(NET_DEVICE_SendCmd("AT+CWJAP=\"Huawei_ALE_bde1\",\"98b9f41a5d36\"\r\n", "GOT IP", 1))
				{
//					Led7_Set(LED_ON);
					LOS_TaskDelay(100);
					
//					Led7_Set(LED_OFF);
					LOS_TaskDelay(100);
				}
				
				status = 0;
				
				break;
#endif
			}
			else
				status = 1;
			
			break;
		}
	}
	
	return status;

}

//==========================================================
//	函数名称：	NET_DEVICE_Init
//
//	函数功能：	网络设备初始化
//
//	入口参数：	无
//
//	返回参数：	返回初始化结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_Init(char *ip, char *port)
{
	
	unsigned char errCount = 0;
	_Bool status = 1;
	char cfgBuffer[64];

	switch(netDeviceInfo.initStep)
	{
		case 0:
			
			memset(cfgBuffer, 0, sizeof(cfgBuffer));
			
			strcpy(cfgBuffer, "AT+CIPSTART=\"UDP\",\"");
			strcat(cfgBuffer, ip);
			strcat(cfgBuffer, "\",");
			strcat(cfgBuffer, port);
			strcat(cfgBuffer, "\r\n");
			printf( "STA Tips:	%s", cfgBuffer);
		
			while(NET_DEVICE_SendCmd(cfgBuffer, "CONNECT", 1))				//连接平台，检索“CONNECT”，如果失败会进入循环体
			{
//				Led7_Set(LED_ON);
				LOS_TaskDelay(100);
//				
//				Led7_Set(LED_OFF);
				LOS_TaskDelay(100);
				
				if(++errCount >= 10)
				{
					printf( "PT info Error,Use APP -> 8266\r\n");
					status = 1;
					break;
				}
			}
			
			if(errCount != 10)
				netDeviceInfo.initStep++;
		
		break;
			
#if(NET_DEVICE_TRANS == 1)
			
		case 2:

			ESP8266_EnterTrans();											//进入透传模式
			printf( "Tips:	EnterTrans\r\n");

			netDeviceInfo.initStep++;

		break;
			
#endif
		
		default:
			status = 0;
			printf( "Tips:	ESP8266 STA_Mode OK\r\n");
		break;
	}
	
	return status;

}

//==========================================================
//	函数名称：	NET_DEVICE_Reset
//
//	函数功能：	网络设备复位
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void NET_DEVICE_Reset(void)
{
	
#if(NET_DEVICE_TRANS == 1)
	ESP8266_QuitTrans();	//退出透传模式
	printf( "Tips:	QuitTrans\r\n");
#endif

	printf( "Tips:	ESP8266_Reset\r\n");
	
	NET_DEVICE_RST_ON;		//复位
	LOS_TaskDelay(50);
	
	NET_DEVICE_RST_OFF;		//结束复位
	LOS_TaskDelay(200);

}

//==========================================================
//	函数名称：	NET_DEVICE_ReLink
//
//	函数功能：	重连平台
//
//	入口参数：	无
//
//	返回参数：	返回连接结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_ReLink(char *ip, char *port)
{
	
	_Bool status = 0;
	char cfgBuffer[64];
	
#if(NET_DEVICE_TRANS == 1)
	ESP8266_QuitTrans();
	printf( "Tips:	QuitTrans\r\n");
#endif
	
	NET_DEVICE_SendCmd("AT+CIPCLOSE\r\n", "OK", 1);												//连接前先关闭一次
	printf( "Tips:	CIPCLOSE\r\n");
	LOS_TaskDelay(100);																			//等待

	memset(cfgBuffer, 0, sizeof(cfgBuffer));
			
	strcpy(cfgBuffer, "AT+CIPSTART=\"UDP\",\"");
	strcat(cfgBuffer, ip);
	strcat(cfgBuffer, "\",");
	strcat(cfgBuffer, port);
	strcat(cfgBuffer, "\r\n");
	printf( "STA Tips:	%s", cfgBuffer);

	status = NET_DEVICE_SendCmd(cfgBuffer, "CONNECT", 1);										//重新连接
	
#if(NET_DEVICE_TRANS == 1)
		ESP8266_EnterTrans();
		printf( "Tips:	EnterTrans\r\n");
#endif
	
	return status;

}

//==========================================================
//	函数名称：	NET_DEVICE_SendCmd
//
//	函数功能：	向网络设备发送一条命令，并等待正确的响应
//
//	入口参数：	cmd：需要发送的命令
//				res：需要检索的响应
//				mode：1-清除接收		0-不清除(能获取返回信息)
//
//	返回参数：	返回连接结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_SendCmd(char *cmd, char *res, _Bool mode)
{
	
	unsigned char timeOut = 200;
	
	NET_IO_Send((unsigned char *)cmd, strlen((const char *)cmd));	//写命令到网络设备
	
	while(timeOut--)												//等待
	{
		if(NET_IO_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)netIOInfo.buf, res) != NULL)	//如果检索到关键词
			{
				if(mode)
					NET_IO_ClearRecive();							//清空缓存
				
				return 0;
			}
		}
		
		LOS_TaskDelay(2);											//挂起等待
	}
	
	return 1;

}

//==========================================================
//	函数名称：	NET_DEVICE_SendData
//
//	函数功能：	使网络设备发送数据到平台
//
//	入口参数：	data：需要发送的数据
//				len：数据长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void NET_DEVICE_SendData(unsigned char *data, unsigned short len)
{
	
#if(NET_DEVICE_TRANS == 1)
	NET_IO_Send(data, len);  						//发送设备连接请求数据
#else
	char cmdBuf[200];
	
	NET_IO_ClearRecive();							//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	
	printf("NET_DEVICE_SendData : %s\r\n",cmdBuf);
	for (int i=0;i<len;i++)
	{
	printf("SendData : 0x%02x\r\n",data[i]);
	}
	
	if(!NET_DEVICE_SendCmd(cmdBuf, ">", 1))			//收到‘>’时可以发送数据
	{
		NET_IO_Send(data, len);  					//发送设备连接请求数据
	}
	
	LOS_TaskDelay(10);								//等待一下
#endif

}

//==========================================================
//	函数名称：	NET_DEVICE_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *NET_DEVICE_GetIPD(unsigned short timeOut)
{

#if(NET_DEVICE_TRANS == 0)
	unsigned short byte = 0, count = 0;
	char sByte[5];
	char *ptrIPD;
#endif
	
	do
	{
		if(NET_IO_WaitRecive() == REV_OK)								//如果接收完成
		{
#if(NET_DEVICE_TRANS == 0)
//			printf("GetIPD : \r\n");
//			printf ("\r\n %s\r\n",netIOInfo.buf);
//			printf ("\r\n*******************************************\r\n");
			ptrIPD = strstr((char *)netIOInfo.buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//printf( "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strstr(ptrIPD, ",");ptrIPD++;					//找到','  ，然后偏移到下一个字符，代表数据长度的第一个数字
				
				while(*ptrIPD != ':')									//在','和':' 之间的都是数据长度的数据
				{
					sByte[count++] = *ptrIPD++;
				}
				byte = (unsigned short)atoi(sByte);						//将字符转为数值形式
				
				ptrIPD++;												//此时ptrIPD指针还指向':'，所以需要偏移到下个数据
				for(count = 0; count < byte; count++)					//复制数据
				{
					netIOInfo.buf[count] = *ptrIPD++;
				}
				memset(netIOInfo.buf + byte,
						0, sizeof(netIOInfo.buf) - byte);				//将后边的数据清空
				netDeviceInfo.ipdBytes = byte;
				
				return netIOInfo.buf;									//复制完成，返回数据指针
			}
#else
			netDeviceInfo.ipdBytes = byte;
			return netIOInfo.buf;
#endif
		}
		
		LOS_TaskDelay(2);												//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}

//==========================================================
//	函数名称：	NET_DEVICE_ClrData
//
//	函数功能：	清空网络设备数据接收缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void NET_DEVICE_ClrData(void)
{

	NET_IO_ClearRecive();	//清空缓存

}

//==========================================================
//	函数名称：	NET_DEVICE_Check
//
//	函数功能：	检查网络设备连接状态
//
//	入口参数：	无
//
//	返回参数：	返回状态
//
//	说明：		
//==========================================================
unsigned char NET_DEVICE_Check(void)
{
	
	unsigned char status = 0;
	unsigned char timeOut = 200;
	
#if(NET_DEVICE_TRANS == 1)
	ESP8266_QuitTrans();
	printf( "Tips:	QuitTrans\r\n");
#endif

	NET_IO_ClearRecive();												//清空缓存
	NET_IO_Send((unsigned char *)"AT+CIPSTATUS\r\n",  14);				//发送状态监测
	
	while(--timeOut)
	{
		if(NET_IO_WaitRecive() == REV_OK)
		{
			if(strstr((const char *)netIOInfo.buf, "STATUS:2"))			//获得IP
			{
				status = 2;
				printf( "ESP8266 Got IP\r\n");
			}
			else if(strstr((const char *)netIOInfo.buf, "STATUS:3"))	//建立连接
			{
				status = 0;
				printf( "ESP8266 Connect OK\r\n");
			}
			else if(strstr((const char *)netIOInfo.buf, "STATUS:4"))	//失去连接
			{
				status = 1;
				printf( "ESP8266 Lost Connect\r\n");
			}
			else if(strstr((const char *)netIOInfo.buf, "STATUS:5"))	//物理掉线
			{
				status = 3;
				printf( "ESP8266 Lost\r\n");			//设备丢失
			}
			
			break;
		}
		
		LOS_TaskDelay(2);
	}
	
	if(timeOut == 0)
	{
		status = 3;
		printf( "ESP8266 TimeOut\r\n");
	}
	
	return status;

}

//==========================================================
//	函数名称：	NET_DEVICE_ReConfig
//
//	函数功能：	设备网络设备初始化的步骤
//
//	入口参数：	步骤值
//
//	返回参数：	无
//
//	说明：		该函数设置的参数在网络设备初始化里边用到
//==========================================================
void NET_DEVICE_ReConfig(unsigned char step)
{

	netDeviceInfo.initStep = step;

}

//==========================================================
//	函数名称：	NET_DEVICE_Set_DataMode
//
//	函数功能：	设置设备数据收发模式
//
//	入口参数：	模式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void NET_DEVICE_Set_DataMode(unsigned char mode)
{

	netDeviceInfo.dataType = mode;

}

//==========================================================
//	函数名称：	NET_DEVICE_Get_DataMode
//
//	函数功能：	获取设备数据收发模式
//
//	入口参数：	无
//
//	返回参数：	模式
//
//	说明：		
//==========================================================
unsigned char NET_DEVICE_Get_DataMode(void)
{

	return netDeviceInfo.dataType;

}

