/* ========================================================================== *
 *                                                                            *
 *        Copyright (C) 2014-2016 Huawei Technologies Co., Ltd.               *
 *                              All rights reserved.                          *
 * THIS SOFTWARE IS PROVIDE BY Huawei Technologies Co., Ltd. `` AS IS"" AND   *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE     *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    *
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT *
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY  *
 * OUT OF THE USE OF THIS SOFTWARE , EVEN IF ADVISED OF THE POSSIBILITY OF    * 
 * SUCH DAMAGE.                                                               *
 *                                                                            *
 * ========================================================================== */
 
 #include "ocean_porting.h"
// #include "main.h"//--------
 #include "ocean_sample.h"
 #include "los_bsp_led.h"
// #include "max7219_driver_func.h"
 
static unsigned char flashdata[256] = {0};
static COAP_LED_STAT led_status = C_LED_ON;//0 means off, 1 means on


int get_cmd = 0;
int uset_value = 0;

/* load verify data from flash, now we just test, so read it in ram */
unsigned char * user_dev_load_data(unsigned int uiAddr, unsigned int uiLen)
{
	return (unsigned char *)flashdata;
}

/* save verify data to flash, now we just test, so keep it in ram */
void user_dev_save_data(unsigned int uiAddr, unsigned char *pucData, unsigned int uiLen)
{
	int tmp = 0;
	if(0 == uiAddr || NULL == pucData || 0 == uiLen)
	{
		return ;
	}
	tmp = sizeof(flashdata);
	if (uiLen > tmp)
	{
		uiLen = tmp;
	}
	memcpy(flashdata, pucData, uiLen);
	return ;
}

/*--1由设备自定义，需要更改!!屏蔽了 sprintf((char *)pucData,*/
void user_dev_get_sn(unsigned char *pucData, unsigned char * uiLen)
{
	
	if(NULL == pucData || NULL == uiLen)
	{
		return ;
	}
	//pucData[0] = MAC_ADDR0;
	//pucData[1] = MAC_ADDR1;
	//pucData[2] = MAC_ADDR2;
	//pucData[3] = MAC_ADDR3;
	//pucData[4] = MAC_ADDR4;
	//pucData[5] = MAC_ADDR5;
	
	memcpy(pucData, "xy123567", 8);
	//sprintf((char *)pucData, "%02x%02x%02x", MAC_ADDR0, MAC_ADDR1, MAC_ADDR2);//----
	//sprintf((char *)pucData+6, "%02x%02x%02x", MAC_ADDR3, MAC_ADDR4, MAC_ADDR5);
	*uiLen = 8;
	return ;
}

//--下发命令
void user_recv_cmd(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen)
{
	char showd;
	//here we should get the cmd and control the led_status
	led_status = C_LED_ON;
	if (NULL == pucVal || 0 == nLen)
	{
		return ;
	}
	if (3 == nLen && pucVal[0] == 0xaa && pucVal[1] == 0x72)
	{
		if (pucVal[2] > 35)
		{
			get_cmd = 0;
			uset_value = 0;
		}
		else
		{
			get_cmd = 1;
			uset_value = pucVal[2];
			showd = max7219_get_show_data(pucVal[2]);
			LOS_TaskLock();
//			LOS_MAX7219_show(showd);
			LOS_TaskUnlock();
		}
		/*
		if (pucVal[2] != 0)//set led on
		{
			led_status = C_LED_ON;
			if(0 == led_get_status())
			{
				led_set_status();
			}
		}
		else
		{
			led_status = C_LED_OFF;
			if(1 == led_get_status())
			{
				led_set_status();
			}
		}*/
	}
	else
	{
		return ;
	}
	
	switch(led_status)
	{
		case C_LED_ON:
//			LOS_EvbLedControl(LOS_LED3, LED_ON);
			break;
		case C_LED_OFF:
		default:
//			LOS_EvbLedControl(LOS_LED3, LED_OFF);
			break;
	}
	return ;

}
