/**
  ******************************************************************************
  * @file    sys.h
  * @author  wx风筝
  * @version V1.0
  * @date    2017-02-04
  * @brief        
  * @attention
  ******************************************************************************
  */

#ifndef __SYS_H
#define __SYS_H

#include "stm32f10x.h"

//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	
    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

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

/**************全局变量声明*****************/
extern u8 tempture_max;                 //温度上限值
extern u8 tempture_min;                 //温度下限值
extern u8 humidity_max;                 //湿度上限值
extern u8 humidity_min;                 //湿度下限值
extern u8 smoke_max;                    //烟雾浓度上限值
extern u8 smoke_min;                    //烟雾浓度下限值

/****************实际测试值*******************/
extern u8 bed_room_tempture;            //卧室温度(DHT11测得)
extern short parlour_tempture;          //客厅温度(DS18B20测得)
extern u8 humidity;                     //湿度(DHT11测得)--针对整个房间测试
extern u32 kitchen_smoke;               //厨房烟雾浓度(MQ-2测得)
extern u8 light_strength;               //光照强度

/*步进电机运行状态控制变量，通过修改变量状态使不仅电机启动或者停止，以及正转和反转*/
extern volatile u8 Motor_Stop;
extern volatile u8 Motor_Forward;

//ZigBee接受数据标志变量
#define USART3_MAX_SIZE        20         //最大接收数据个数
extern u8 packerflag;                     //接收到一包完整数据包标志(1:完整，0：不完整)
extern u8 USART3_RX_BUF[USART3_MAX_SIZE]; //接收缓冲,最大64个字节.
extern u8 USART3_RX_STA;                  //接收状态标记
extern u8 USART3_COUNT;                   //接收到的数据格式

/******************手控/光控标志位**********************/
extern u8 flag_auto_control;             //自动控制

//extern u8 flag_leave_home;
extern u8 flag_go_home;

//void NVIC_Configuration(void);

void JTAG_Set(u8 mode);

#endif

