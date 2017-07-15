



/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	app.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2016-11-23
	*
	*	版本： 		V1.0
	*
	*	说明： 		完成单片机初始化、外接IC初始化和任务的创建及运行
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//协议
#include "onenet.h"
#include "fault.h"

#include "ocean_sample.h"

//OS
#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "los_bsp_adapter.h"

//网络设备
#include "net_device.h"

//硬件驱动
#include "bsp_led.h"
#include "key.h"

//C库
#include <string.h>
#include <time.h>
#include <stdio.h>


#include "app.h"



//看门狗任务
#define IWDG_TASK_PRIO		6
#define IWDG_STK_SIZE		64
//OS_STK IWDG_TASK_STK[IWDG_STK_SIZE];
UINT32 IWDG_Task(VOID);

//串口任务
#define USART_TASK_PRIO		7
#define USART_STK_SIZE		1024
//OS_STK USART_TASK_STK[USART_STK_SIZE]; //
UINT32 USART_Task(VOID);

//心跳任务
#define HEART_TASK_PRIO		8
#define HEART_STK_SIZE		512
//OS_STK HEART_TASK_STK[HEART_STK_SIZE]; //
UINT32 HEART_Task(VOID);

//故障处理任务
#define FAULT_TASK_PRIO		9 //
#define FAULT_STK_SIZE		512
//OS_STK FAULT_TASK_STK[FAULT_STK_SIZE]; //
UINT32 FAULT_Task(VOID);

//传感器任务
#define SENSOR_TASK_PRIO	10
#define SENSOR_STK_SIZE		512
//__align(8) OS_STK SENSOR_TASK_STK[SENSOR_STK_SIZE]; //UCOS使用浮点数进行printf、sprintf时一定要8字节对齐
UINT32 SENSOR_Task(VOID);

//数据发送任务
#define SEND_TASK_PRIO		11
#define SEND_STK_SIZE		64
//OS_STK SEND_TASK_STK[SEND_STK_SIZE];
UINT32 SEND_Task(VOID);

//按键任务
#define KEY_TASK_PRIO		12
#define KEY_STK_SIZE		256
//__align(8) OS_STK KEY_TASK_STK[KEY_STK_SIZE]; //UCOS使用浮点数进行printf、sprintf时一定要8字节对齐
UINT32 KEY_Task(VOID);

//网络初始化任务
#define NET_TASK_PRIO		13 //
#define NET_STK_SIZE		256
//OS_STK NET_TASK_STK[NET_STK_SIZE]; //
UINT32 NET_Task(VOID);

//数据反馈任务
#define DATA_TASK_PRIO		14 //
#define DATA_STK_SIZE		512
//__align(8) OS_STK DATA_TASK_STK[DATA_STK_SIZE]; //UCOS使用浮点数进行printf、sprintf时一定要8字节对齐
UINT32 DATA_Task(VOID);

////信息更改任务
//#define ALTER_TASK_PRIO		15 //
//#define ALTER_STK_SIZE		512
//OS_STK ALTER_TASK_STK[ALTER_STK_SIZE]; //
//void ALTER_Task(void *pdata);

//时钟任务
#define CLOCK_TASK_PRIO		16 //
#define CLOCK_STK_SIZE		256
//OS_STK CLOCK_TASK_STK[CLOCK_STK_SIZE]; //
UINT32 CLOCK_Task(VOID);



#define NET_TIME	60			//设定时间--单位秒
unsigned short timerCount = 0;	//时间计数--单位秒

char rank;

//任务ID
static UINT32 IWDG_TaskID;
static UINT32 USART_TaskID;
static UINT32 HEART_TaskID;
static UINT32 FAULT_TaskID;
static UINT32 SENSOR_TaskID;
static UINT32 SEND_TaskID;
static UINT32 KEY_TaskID;
static UINT32 NET_TaskID;
static UINT32 DATA_TaskID;

//OS_TMR *tmr;					//软件定时器句柄

/*
************************************************************
*	函数名称：	OS_TimerCallBack
*
*	函数功能：	定时检查网络状态标志位
*
*	入口参数：	ptmr：软件定时器句柄
*				p_arg：软件定时器回调函数参数
*
*	返回参数：	无
*
*	说明：		定时器任务。定时检查网络状态，若持续超过设定时间无网络连接，则进行平台重连
************************************************************
*/
//void OS_TimerCallBack(OS_TMR *ptmr, void *p_arg)
//{
//	
//	if(oneNetInfo.netWork == 0)											//如果网络断开
//	{
//		if(++timerCount >= NET_TIME) 									//如果网络断开超时
//		{
//			UsartPrintf(USART_DEBUG, "Tips:		Timer Check Err\r\n");
//			
//			checkInfo.NET_DEVICE_OK = 0;								//置设备未检测标志
//			
//			NET_DEVICE_ReConfig(0);										//设备初始化步骤设置为开始状态
//			
//			oneNetInfo.netWork = 0;
//		}
//	}
//	else
//	{
//		timerCount = 0;													//清除计数
//	}
//	

//}

/*
************************************************************
*	函数名称：	IWDG_Task
*
*	函数功能：	清除看门狗
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		看门狗任务
************************************************************
*/
UINT32 IWDG_Task(VOID)
{

	while(1)
	{
		if(oneNetInfo.netWork == 0)
		{	
	//		Iwdg_Feed(); 		//喂狗	
			LOS_EvbLedControl(LOS_LED1, LED_ON);
			LOS_TaskDelay(500);
			LOS_EvbLedControl(LOS_LED1, LED_OFF);
			LOS_TaskDelay(500);
		}
		else
		{
			LOS_EvbLedControl(LOS_LED3, LED_ON);
			LOS_EvbLedControl(LOS_LED1, LED_ON);
			LOS_TaskDelay(500);
			LOS_EvbLedControl(LOS_LED3, LED_OFF);
			LOS_TaskDelay(500);
		}
	}
}

/*
************************************************************
*	函数名称：	KEY_Task
*
*	函数功能：	扫描按键是否按下，如果有按下，进行对应的处理
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		按键任务
************************************************************
*/
UINT32 KEY_Task(VOID)
{
//					LOS_EvbLedControl(LOS_LED1, LED_ON);
//					LOS_EvbUartWriteStr("key Test\n");
//					LOS_TaskDelay(500);
//					LOS_EvbLedControl(LOS_LED1, LED_OFF);
//					LOS_TaskDelay(500);

}

/*
************************************************************
*	函数名称：	HEART_Task
*
*	函数功能：	心跳检测
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		心跳任务。发送心跳请求并等待心跳响应，若在设定时间内没有响应则会进行平台重连
************************************************************
*/
UINT32 HEART_Task(VOID)
{

	while(1)
	{
		
		LOS_TaskDelay(20);	//挂起任务
	
	}

}

/*
************************************************************
*	函数名称：	SEND_Task
*
*	函数功能：	上传传感器数据
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		数据发送任务
************************************************************
*/
UINT32 SEND_Task(VOID)
{

	while(1)
	{
		
		oneNetInfo.sendData = 1;		//标记数据反馈
		
		LOS_TaskDelay(15);	//挂起任务
		
	}

}

/*
************************************************************
*	函数名称：	USART_Task
*
*	函数功能：	处理平台下发的命令
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		串口接收任务。在数据模式下时，等待平台下发的命令并解析、处理
************************************************************
*/
UINT32 USART_Task(VOID)
{
	
	unsigned char *dataPtr;														//数据指针

//	while(1)
//	{
//		if(oneNetInfo.netWork && NET_DEVICE_Get_DataMode() == DEVICE_DATA_MODE)	//当有网络 且 在命令接收模式时
//		{
//			dataPtr = NET_DEVICE_GetIPD(0);										//不等待，获取平台下发的数据
//			if(dataPtr != NULL)													//如果数据指针不为空，则代表收到了数据
//			{
//				OneNet_App(dataPtr);											//集中处理
//			}
//		}
//		
//		LOS_TaskDelay(2);														//挂起任务10ms
//	
//	}

}

/*
************************************************************
*	函数名称：	SENSOR_Task
*
*	函数功能：	传感器数据采集、显示
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		传感器数据采集任务。进行外接传感器的数据采集、读取、显示
************************************************************
*/
UINT32 SENSOR_Task(VOID)
{

	while(1)
	{
	
//		LOS_EvbLedControl(LOS_LED1, LED_ON);
//		//LOS_EvbUartWriteStr("SENSOR Test\n");
//		LOS_TaskDelay(500);
//		LOS_EvbLedControl(LOS_LED1, LED_OFF);
//		LOS_TaskDelay(500);
//		LOS_TaskDelay(100); 										//挂起任务500ms
//			Get_rank(0);
			LOS_TaskDelay(500);
	}

}

/*
************************************************************
*	函数名称：	DATA_Task
*
*	函数功能：	平台下发命令的数据反馈
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		数据反馈任务。这是平台下发指令后的反馈函数，透传模式的时候收到之后立即返回，非透传模式因为需要检索'>'符号，所以使用任务的方式来反馈。
************************************************************
*/
UINT32 DATA_Task(VOID)
{
	while(1)
	{
//			rank = Get_rank(0);
//			
//			if (KeyScan())
//			{
//			LOS_EvbLedControl(LOS_LED2, LED_OFF);
//			Beep_Set(BEEP_OFF);
//			}
//			
//			printf("rank: %d\r\n",rank);
			LOS_TaskDelay(500);
	}

}

/*
************************************************************
*	函数名称：	FAULT_Task
*
*	函数功能：	网络状态错误处理
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		故障处理任务。当发生网络错误、设备错误时，会标记对应标志位，然后集中进行处理
************************************************************
*/
UINT32 FAULT_Task(VOID)
{

	while(1)
	{
		
		if(faultType != FAULT_NONE)									//如果错误标志被设置
		{
			printf( "WARN:	Fault Process\r\n");
			Fault_Process();										//进入错误处理函数
		}
		
		LOS_TaskDelay(10);											//挂起任务50ms
	
	}

}

/*
************************************************************
*	函数名称：	NET_Task
*
*	函数功能：	网络连接、平台接入
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		网络连接任务任务。会在心跳检测里边检测网络连接状态，如果有错，会标记状态，然后在这里进行重连
************************************************************
*/
UINT32 NET_Task(VOID)
{
	
	NET_DEVICE_IO_Init();													//网络设备IO初始化
	NET_DEVICE_Reset();														//网络设备复位
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);								//设置为命令收发模式(例如ESP8266要区分AT的返回还是平台下发数据的返回)

	while(1)
	{
		
		if(!oneNetInfo.netWork && (checkInfo.NET_DEVICE_OK == DEV_OK))		//当没有网络 且 网络模块检测到时
		{
			
			NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);						//设置为命令收发模式
			
			if(!NET_DEVICE_Init(oneNetInfo.ip, oneNetInfo.port))			//初始化网络设备，能连入网络
			{
//				OneNet_DevLink(oneNetInfo.devID, oneNetInfo.apiKey);		//接入平台
				oneNetInfo.netWork = 1;
				
				LOS_OceanCon_Sample();
				printf("NET_OK\r\n");
				
				if(oneNetInfo.netWork)
				{
//					Beep_Set(BEEP_ON);										//短叫提示成功
//					LOS_TaskDelay(40);
//					Beep_Set(BEEP_OFF);
							
				}
				else
				{
////					Beep_Set(BEEP_ON);										//长叫提示失败
////					LOS_TaskDelay(100);
////					Beep_Set(BEEP_OFF);
					
				}
			}
		}
		
		if(checkInfo.NET_DEVICE_OK == DEV_ERR) 								//当网络设备未做检测
		{
			NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);						//设置为命令收发模式
			
			if(timerCount >= NET_TIME) 										//如果网络连接超时
			{
				NET_DEVICE_Reset();											//复位网络设备
				timerCount = 0;												//清零连接超时计数
				faultType = FAULT_NONE;										//清除错误标志
			}
			
			if(!NET_DEVICE_Exist())											//网络设备检测
			{
				printf( "NET Device :Ok\r\n");
				checkInfo.NET_DEVICE_OK = DEV_OK;							//检测到网络设备，标记
				NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);					//设置为数据收发模式
			}
			else
				printf( "NET Device :Error\r\n");
		}
		LOS_TaskDelay(5);													//挂起任务25ms
	
	}

}
/*
************************************************************
*	函数名称：	CLOCK_Task
*
*	函数功能：	网络校时、时间显示
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
//void CLOCK_Task(void *pdata)
//{
//	
//	unsigned int second = 0, secTemp = 0;
//	struct tm *time;
//	char timeStr[24];

//	while(1)
//	{
//	
//		if(second == 0)														//当second为0时
//		{
//			while(!second)
//				second = NET_Clock();										//则进行自动校时

//			RTC_SetTime(second + 4);										//设置RTC时间，加4是补上大概的时间差
//		}
//		else																//正常运行
//		{
//			secTemp = RTC_GetCounter();										//获取秒值
//			if(second != secTemp)											//当秒改变时，更新显示
//			{
//				second = secTemp;
//				
//				time = localtime((const time_t *)&second);					//将秒值转为tm结构所表示的时间
//				snprintf(timeStr, sizeof(timeStr), "%d-%d-%d %d:%d:%d   ", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
//				SPILCD_DisString(0, 115, 12, RED, 1, "%s", timeStr);		//显示
//				
//				if(time->tm_hour == 0)										//每天0点时，更新一次时间
//					second = 0;
//			}
//		}
//		
//		RTOS_TimeDly(100);													//挂起任务500ms
//	
//	}

//}

/*
************************************************************
*	函数名称：	
*
*	函数功能：	任务的创建
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void LOS_task(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;
		//LOS_TaskLock();
//		/*********IWDG_Task**********/
//    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
//    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)IWDG_Task;
//    stTaskInitParam.uwStackSize = 0x200;
//    stTaskInitParam.pcName = "IWDG_Task";
//    stTaskInitParam.usTaskPrio = IWDG_TASK_PRIO;
//    uwRet = LOS_TaskCreate(&IWDG_TaskID, &stTaskInitParam);

//    if (uwRet != LOS_OK)
//    {
//        printf("IWDG_Task test task create failed\n");
//        return;
//    }
		/*********USRAT_Task**********/		
//		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
//    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)USART_Task;
//    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
//    stTaskInitParam.pcName = "USART_Task";
//    stTaskInitParam.usTaskPrio = USART_TASK_PRIO;
//    uwRet = LOS_TaskCreate(&USART_TaskID, &stTaskInitParam);

//    if (uwRet != LOS_OK)
//    {
//        printf("USART_Task test task create failed\n");
//        return;
//    }
		/*********FAULT_Task**********/		
		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)FAULT_Task;
    stTaskInitParam.uwStackSize = 0x200;
    stTaskInitParam.pcName = "FAULT_Task";
    stTaskInitParam.usTaskPrio = FAULT_TASK_PRIO;
    uwRet = LOS_TaskCreate(&FAULT_TaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
        printf("FAULT_Task test task create failed\n");
        return;
    }
		/*********SENSOR_Task**********/		
//		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
//    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)SENSOR_Task;
//    stTaskInitParam.uwStackSize = 0x400;
//    stTaskInitParam.pcName = "SENSOR_Task";
//    stTaskInitParam.usTaskPrio = SENSOR_TASK_PRIO;
//    uwRet = LOS_TaskCreate(&SENSOR_TaskID, &stTaskInitParam);

//    if (uwRet != LOS_OK)
//    {
//        printf("SENSOR_Task test task create failed\n");
//        return;
//    }
//		/*********SEND_Task**********/		
		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)SEND_Task;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "SEND_Task";
    stTaskInitParam.usTaskPrio = SEND_TASK_PRIO;
    uwRet = LOS_TaskCreate(&SEND_TaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
        printf("SEND_Task test task create failed\n");
        return;
    }
//		/*********KEY_Task**********/		
//		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
//    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)KEY_Task;
//    stTaskInitParam.uwStackSize = 0x200;
//    stTaskInitParam.pcName = "KEY_Task";
//    stTaskInitParam.usTaskPrio = KEY_TASK_PRIO;
//    uwRet = LOS_TaskCreate(&KEY_TaskID, &stTaskInitParam);

//    if (uwRet != LOS_OK)
//    {
//        printf("KEY_Task test task create failed\n");
//        return;
//    }
		/*********NET_Task**********/		
		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)NET_Task;
    stTaskInitParam.uwStackSize = 0x500;
    stTaskInitParam.pcName = "NET_Task";
    stTaskInitParam.usTaskPrio = NET_TASK_PRIO;
    uwRet = LOS_TaskCreate(&NET_TaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
        printf("NET_Task test task create failed\n");
        return;
    }
		/*********DATA_Task**********/		
//		(VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
//    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DATA_Task;
//    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
//    stTaskInitParam.pcName = "DATA_Task";
//    stTaskInitParam.usTaskPrio = DATA_TASK_PRIO;
//    uwRet = LOS_TaskCreate(&DATA_TaskID, &stTaskInitParam);

//    if (uwRet != LOS_OK)
//    {
//        printf("DATA_Task test task create failed\n");
//        return;
//    }
		printf("ok\n");
		//LOS_TaskUnlock();
    return;
}
