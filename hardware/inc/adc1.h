/**
  ******************************************************************************
  * @file    adc1.c
  * @author  wx����
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
 							   
void Adc1_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc1(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  

#endif 















