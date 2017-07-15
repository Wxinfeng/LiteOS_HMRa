/**
  ******************************************************************************
  * @file    sys.h
  * @author  wx����
  * @version V1.0
  * @date    2017-02-04
  * @brief        
  * @attention
  ******************************************************************************
  */

#ifndef __SYS_H
#define __SYS_H

#include "stm32f10x.h"

//JTAGģʽ���ö���
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	
    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum{
    HAS_ONE = 0,
    NO_ONE = !HAS_ONE
}HCSR501;

typedef enum{
	E_smoke = 0,
	E_tempture,
	E_humidity
}SENSOR;

/**************ȫ�ֱ�������*****************/
extern u8 tempture_max;                 //�¶�����ֵ
extern u8 tempture_min;                 //�¶�����ֵ
extern u8 humidity_max;                 //ʪ������ֵ
extern u8 humidity_min;                 //ʪ������ֵ
extern u8 smoke_max;                    //����Ũ������ֵ
extern u8 smoke_min;                    //����Ũ������ֵ

/****************ʵ�ʲ���ֵ*******************/
extern u8 bed_room_tempture;            //�����¶�(DHT11���)
extern short parlour_tempture;          //�����¶�(DS18B20���)
extern u8 humidity;                     //ʪ��(DHT11���)--��������������
extern u32 kitchen_smoke;               //��������Ũ��(MQ-2���)
extern u8 light_strength;               //����ǿ��

/*�����������״̬���Ʊ�����ͨ���޸ı���״̬ʹ���������������ֹͣ���Լ���ת�ͷ�ת*/
extern volatile u8 Motor_Stop;
extern volatile u8 Motor_Forward;

//ZigBee�������ݱ�־����
#define USART3_MAX_SIZE        20         //���������ݸ���
extern u8 packerflag;                     //���յ�һ���������ݰ���־(1:������0��������)
extern u8 USART3_RX_BUF[USART3_MAX_SIZE]; //���ջ���,���64���ֽ�.
extern u8 USART3_RX_STA;                  //����״̬���
extern u8 USART3_COUNT;                   //���յ������ݸ�ʽ

/******************�ֿ�/��ر�־λ**********************/
extern u8 flag_auto_control;             //�Զ�����

//extern u8 flag_leave_home;
extern u8 flag_go_home;

//void NVIC_Configuration(void);

void JTAG_Set(u8 mode);

#endif

