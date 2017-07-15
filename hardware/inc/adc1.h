/**
  ******************************************************************************
  * @file    adc1.c
  * @author  wx风筝
  * @version V1.0
  * @date    2017-02-04
  * @brief        
  * @attention
  ******************************************************************************
  */

#ifndef __ADC1_H
#define __ADC1_H	

#include "sys.h" 
#include "delay.h"	
 							   
void Adc1_Init(void); 				//ADC通道初始化
u16  Get_Adc1(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  

#endif 















