/**
  ******************************************************************************
  * @file    delay.h
  * @author  wx·çóÝ
  * @version V1.0
  * @date    2017-02-04
  * @brief        
  * @attention
  ******************************************************************************
  */
	
#ifndef __DELAY_H
#define __DELAY_H 	

#include "sys.h"

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
