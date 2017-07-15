/**
  ******************************************************************************
  * @file    mq_5.c
  * @author  wx风筝
  * @version V1.0
  * @date    2017-02-04
  * @brief        
  * @attention
  ******************************************************************************
  */
	
#include "mq_5.h"

//烟雾传感器初始化
void MQ_5_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    //PA1作为模拟通道输入引脚
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;               //PA1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           //模拟输入
		GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    Adc1_Init();//初始化ADC1
}

/**************************************************************************
函数功能：获取AD转换后的值(电压值)
入口参数：无
返回  值：转换后的浓度值
说    明：获取的电压范围0~3.3V得到的数值为：0~4096
          浓度变化范围，               300~10000
**************************************************************************/
char Get_MQ_5_Value(void)
{      
    u32 temp_val = 0;
	u8 t;
	for(t = 0;t < MQ_5_READ_TIMES;t++)
	{
		temp_val += Get_Adc1(ADC_Channel_1);	//读取ADC值,通道1
		delay_ms(5);
	}
	temp_val /=MQ_5_READ_TIMES;              //得到平均值

		if (temp_val<MQ_5_01)
			return 1;
		if (temp_val<MQ_5_02)
			return 2;
		if (temp_val<MQ_5_03)
			return 3;
		if (temp_val<MQ_5_04)
			return 4;
		if (temp_val<MQ_5_05)
			return 5;
		if (temp_val<MQ_5_06)
			return 6;
		if (temp_val<MQ_5_07)
			return 7;
		if (temp_val<MQ_5_08)
			return 8;
		if (temp_val<MQ_5_09)
			return 9;
		if (temp_val<MQ_5_10)
			return 10;
//		if (temp_val>MQ_5_10)
			return 10;
//	printf("%d\n",temp_val);
//	if(temp_val > 4000) temp_val = 4000;
//	return (u8)(100 - (temp_val/40));
}
