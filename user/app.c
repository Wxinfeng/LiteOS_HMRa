



/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	app.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��ɵ�Ƭ����ʼ�������IC��ʼ��������Ĵ���������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Э��
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

//�����豸
#include "net_device.h"

//Ӳ������
#include "bsp_led.h"
#include "key.h"

//C��
#include <string.h>
#include <time.h>
#include <stdio.h>


#include "app.h"



//���Ź�����
#define IWDG_TASK_PRIO		6
#define IWDG_STK_SIZE		64
//OS_STK IWDG_TASK_STK[IWDG_STK_SIZE];
UINT32 IWDG_Task(VOID);

//��������
#define USART_TASK_PRIO		7
#define USART_STK_SIZE		1024
//OS_STK USART_TASK_STK[USART_STK_SIZE]; //
UINT32 USART_Task(VOID);

//��������
#define HEART_TASK_PRIO		8
#define HEART_STK_SIZE		512
//OS_STK HEART_TASK_STK[HEART_STK_SIZE]; //
UINT32 HEART_Task(VOID);

//���ϴ�������
#define FAULT_TASK_PRIO		9 //
#define FAULT_STK_SIZE		512
//OS_STK FAULT_TASK_STK[FAULT_STK_SIZE]; //
UINT32 FAULT_Task(VOID);

//����������
#define SENSOR_TASK_PRIO	10
#define SENSOR_STK_SIZE		512
//__align(8) OS_STK SENSOR_TASK_STK[SENSOR_STK_SIZE]; //UCOSʹ�ø���������printf��sprintfʱһ��Ҫ8�ֽڶ���
UINT32 SENSOR_Task(VOID);

//���ݷ�������
#define SEND_TASK_PRIO		11
#define SEND_STK_SIZE		64
//OS_STK SEND_TASK_STK[SEND_STK_SIZE];
UINT32 SEND_Task(VOID);

//��������
#define KEY_TASK_PRIO		12
#define KEY_STK_SIZE		256
//__align(8) OS_STK KEY_TASK_STK[KEY_STK_SIZE]; //UCOSʹ�ø���������printf��sprintfʱһ��Ҫ8�ֽڶ���
UINT32 KEY_Task(VOID);

//�����ʼ������
#define NET_TASK_PRIO		13 //
#define NET_STK_SIZE		256
//OS_STK NET_TASK_STK[NET_STK_SIZE]; //
UINT32 NET_Task(VOID);

//���ݷ�������
#define DATA_TASK_PRIO		14 //
#define DATA_STK_SIZE		512
//__align(8) OS_STK DATA_TASK_STK[DATA_STK_SIZE]; //UCOSʹ�ø���������printf��sprintfʱһ��Ҫ8�ֽڶ���
UINT32 DATA_Task(VOID);

////��Ϣ��������
//#define ALTER_TASK_PRIO		15 //
//#define ALTER_STK_SIZE		512
//OS_STK ALTER_TASK_STK[ALTER_STK_SIZE]; //
//void ALTER_Task(void *pdata);

//ʱ������
#define CLOCK_TASK_PRIO		16 //
#define CLOCK_STK_SIZE		256
//OS_STK CLOCK_TASK_STK[CLOCK_STK_SIZE]; //
UINT32 CLOCK_Task(VOID);



#define NET_TIME	60			//�趨ʱ��--��λ��
unsigned short timerCount = 0;	//ʱ�����--��λ��

char rank;

//����ID
static UINT32 IWDG_TaskID;
static UINT32 USART_TaskID;
static UINT32 HEART_TaskID;
static UINT32 FAULT_TaskID;
static UINT32 SENSOR_TaskID;
static UINT32 SEND_TaskID;
static UINT32 KEY_TaskID;
static UINT32 NET_TaskID;
static UINT32 DATA_TaskID;

//OS_TMR *tmr;					//�����ʱ�����

/*
************************************************************
*	�������ƣ�	OS_TimerCallBack
*
*	�������ܣ�	��ʱ�������״̬��־λ
*
*	��ڲ�����	ptmr�������ʱ�����
*				p_arg�������ʱ���ص���������
*
*	���ز�����	��
*
*	˵����		��ʱ�����񡣶�ʱ�������״̬�������������趨ʱ�����������ӣ������ƽ̨����
************************************************************
*/
//void OS_TimerCallBack(OS_TMR *ptmr, void *p_arg)
//{
//	
//	if(oneNetInfo.netWork == 0)											//�������Ͽ�
//	{
//		if(++timerCount >= NET_TIME) 									//�������Ͽ���ʱ
//		{
//			UsartPrintf(USART_DEBUG, "Tips:		Timer Check Err\r\n");
//			
//			checkInfo.NET_DEVICE_OK = 0;								//���豸δ����־
//			
//			NET_DEVICE_ReConfig(0);										//�豸��ʼ����������Ϊ��ʼ״̬
//			
//			oneNetInfo.netWork = 0;
//		}
//	}
//	else
//	{
//		timerCount = 0;													//�������
//	}
//	

//}

/*
************************************************************
*	�������ƣ�	IWDG_Task
*
*	�������ܣ�	������Ź�
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���Ź�����
************************************************************
*/
UINT32 IWDG_Task(VOID)
{

	while(1)
	{
		if(oneNetInfo.netWork == 0)
		{	
	//		Iwdg_Feed(); 		//ι��	
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
*	�������ƣ�	KEY_Task
*
*	�������ܣ�	ɨ�谴���Ƿ��£�����а��£����ж�Ӧ�Ĵ���
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		��������
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
*	�������ƣ�	HEART_Task
*
*	�������ܣ�	�������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		�������񡣷����������󲢵ȴ�������Ӧ�������趨ʱ����û����Ӧ������ƽ̨����
************************************************************
*/
UINT32 HEART_Task(VOID)
{

	while(1)
	{
		
		LOS_TaskDelay(20);	//��������
	
	}

}

/*
************************************************************
*	�������ƣ�	SEND_Task
*
*	�������ܣ�	�ϴ�����������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ݷ�������
************************************************************
*/
UINT32 SEND_Task(VOID)
{

	while(1)
	{
		
		oneNetInfo.sendData = 1;		//������ݷ���
		
		LOS_TaskDelay(15);	//��������
		
	}

}

/*
************************************************************
*	�������ƣ�	USART_Task
*
*	�������ܣ�	����ƽ̨�·�������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ڽ�������������ģʽ��ʱ���ȴ�ƽ̨�·����������������
************************************************************
*/
UINT32 USART_Task(VOID)
{
	
	unsigned char *dataPtr;														//����ָ��

//	while(1)
//	{
//		if(oneNetInfo.netWork && NET_DEVICE_Get_DataMode() == DEVICE_DATA_MODE)	//�������� �� ���������ģʽʱ
//		{
//			dataPtr = NET_DEVICE_GetIPD(0);										//���ȴ�����ȡƽ̨�·�������
//			if(dataPtr != NULL)													//�������ָ�벻Ϊ�գ�������յ�������
//			{
//				OneNet_App(dataPtr);											//���д���
//			}
//		}
//		
//		LOS_TaskDelay(2);														//��������10ms
//	
//	}

}

/*
************************************************************
*	�������ƣ�	SENSOR_Task
*
*	�������ܣ�	���������ݲɼ�����ʾ
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���������ݲɼ����񡣽�����Ӵ����������ݲɼ�����ȡ����ʾ
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
//		LOS_TaskDelay(100); 										//��������500ms
//			Get_rank(0);
			LOS_TaskDelay(500);
	}

}

/*
************************************************************
*	�������ƣ�	DATA_Task
*
*	�������ܣ�	ƽ̨�·���������ݷ���
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ݷ�����������ƽ̨�·�ָ���ķ���������͸��ģʽ��ʱ���յ�֮���������أ���͸��ģʽ��Ϊ��Ҫ����'>'���ţ�����ʹ������ķ�ʽ��������
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
*	�������ƣ�	FAULT_Task
*
*	�������ܣ�	����״̬������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ϴ������񡣵�������������豸����ʱ�����Ƕ�Ӧ��־λ��Ȼ���н��д���
************************************************************
*/
UINT32 FAULT_Task(VOID)
{

	while(1)
	{
		
		if(faultType != FAULT_NONE)									//��������־������
		{
			printf( "WARN:	Fault Process\r\n");
			Fault_Process();										//�����������
		}
		
		LOS_TaskDelay(10);											//��������50ms
	
	}

}

/*
************************************************************
*	�������ƣ�	NET_Task
*
*	�������ܣ�	�������ӡ�ƽ̨����
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���������������񡣻������������߼����������״̬������д�����״̬��Ȼ���������������
************************************************************
*/
UINT32 NET_Task(VOID)
{
	
	NET_DEVICE_IO_Init();													//�����豸IO��ʼ��
	NET_DEVICE_Reset();														//�����豸��λ
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);								//����Ϊ�����շ�ģʽ(����ESP8266Ҫ����AT�ķ��ػ���ƽ̨�·����ݵķ���)

	while(1)
	{
		
		if(!oneNetInfo.netWork && (checkInfo.NET_DEVICE_OK == DEV_OK))		//��û������ �� ����ģ���⵽ʱ
		{
			
			NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);						//����Ϊ�����շ�ģʽ
			
			if(!NET_DEVICE_Init(oneNetInfo.ip, oneNetInfo.port))			//��ʼ�������豸������������
			{
//				OneNet_DevLink(oneNetInfo.devID, oneNetInfo.apiKey);		//����ƽ̨
				oneNetInfo.netWork = 1;
				
				LOS_OceanCon_Sample();
				printf("NET_OK\r\n");
				
				if(oneNetInfo.netWork)
				{
//					Beep_Set(BEEP_ON);										//�̽���ʾ�ɹ�
//					LOS_TaskDelay(40);
//					Beep_Set(BEEP_OFF);
							
				}
				else
				{
////					Beep_Set(BEEP_ON);										//������ʾʧ��
////					LOS_TaskDelay(100);
////					Beep_Set(BEEP_OFF);
					
				}
			}
		}
		
		if(checkInfo.NET_DEVICE_OK == DEV_ERR) 								//�������豸δ�����
		{
			NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);						//����Ϊ�����շ�ģʽ
			
			if(timerCount >= NET_TIME) 										//����������ӳ�ʱ
			{
				NET_DEVICE_Reset();											//��λ�����豸
				timerCount = 0;												//�������ӳ�ʱ����
				faultType = FAULT_NONE;										//��������־
			}
			
			if(!NET_DEVICE_Exist())											//�����豸���
			{
				printf( "NET Device :Ok\r\n");
				checkInfo.NET_DEVICE_OK = DEV_OK;							//��⵽�����豸�����
				NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);					//����Ϊ�����շ�ģʽ
			}
			else
				printf( "NET Device :Error\r\n");
		}
		LOS_TaskDelay(5);													//��������25ms
	
	}

}
/*
************************************************************
*	�������ƣ�	CLOCK_Task
*
*	�������ܣ�	����Уʱ��ʱ����ʾ
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		
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
//		if(second == 0)														//��secondΪ0ʱ
//		{
//			while(!second)
//				second = NET_Clock();										//������Զ�Уʱ

//			RTC_SetTime(second + 4);										//����RTCʱ�䣬��4�ǲ��ϴ�ŵ�ʱ���
//		}
//		else																//��������
//		{
//			secTemp = RTC_GetCounter();										//��ȡ��ֵ
//			if(second != secTemp)											//����ı�ʱ��������ʾ
//			{
//				second = secTemp;
//				
//				time = localtime((const time_t *)&second);					//����ֵתΪtm�ṹ����ʾ��ʱ��
//				snprintf(timeStr, sizeof(timeStr), "%d-%d-%d %d:%d:%d   ", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
//				SPILCD_DisString(0, 115, 12, RED, 1, "%s", timeStr);		//��ʾ
//				
//				if(time->tm_hour == 0)										//ÿ��0��ʱ������һ��ʱ��
//					second = 0;
//			}
//		}
//		
//		RTOS_TimeDly(100);													//��������500ms
//	
//	}

//}

/*
************************************************************
*	�������ƣ�	
*
*	�������ܣ�	����Ĵ���
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		
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
