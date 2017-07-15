/**
  ******************************************************************************
  * @file    mq_5.h
  * @author  wx风筝
  * @version V1.0
  * @date    2017-02-04
  * @brief        
  * @attention
  ******************************************************************************
  */
	
#ifndef __MQ_5_H
#define __MQ_5_H

#include "sys.h"
#include "adc1.h"
#include "usart_1.h"


#define MQ_5_READ_TIMES	10		//定义烟雾传感器读取次数,读这么多次,然后取平均值

#define MQ_5_01    500
#define MQ_5_02    1500
#define MQ_5_03    1800
#define MQ_5_04    2000
#define MQ_5_05    2200
#define MQ_5_06    2400
#define MQ_5_07    2600
#define MQ_5_08    2800
#define MQ_5_09    3200
#define MQ_5_10    4000




void MQ_5_Configuration(void);


char Get_MQ_5_Value(void);        //返回烟雾浓度值

#endif
