#include "ocean_sample.h"
#include "los_bsp_led.h"
#include "los_task.h"
#include "coap.h" 
#include "coap_liteos_port.h"
#include "ocean_porting.h"

//#include "max7219_driver_func.h"


static board_callback_t agentfunc;
static coap_callback_t coapfunc;
static int socket = -1;

static int test_led_status = 0;//0 led off, 1 led on

extern char rank;
extern int get_cmd;
extern int uset_value;
void led_set_status(void)
{
	test_led_status = (test_led_status + 1)%2;
	if(test_led_status)
	{
		//gd_eval_led_on(LED3);
//		LOS_EvbLedControl(LOS_LED3, LED_ON);
	}
	else
	{
		//gd_eval_led_off(LED3);
//		LOS_EvbLedControl(LOS_LED3, LED_OFF);
	}
}
int led_get_status(void)
{
	return test_led_status;
}

int LOS_SetIoTGW_AddrInfo(const char *ipaddr, const char *port)
{
	return coap_set_iotaddr_info(ipaddr, port);
}

char max7219_get_show_data(int idx)
{
	if (idx < 0  || idx > 38)
	{
		return '0';
	}
	if (0<=idx && 9>= idx)
	{
		return 0x30+idx;
	}
	if (10<=idx || 35>=idx)
	{
		return 0x41 + idx - 10;
	}
	return '0';
}


int LOS_OceanCon_Init(void)
{
	coap_resource_init();
	//int coap_alloc_source(int *socket, int kind)
	if (coap_alloc_source(&socket, 0))
	{
		//failed create socket
		while(1);
	}
	
	agentfunc.pfnGetSN = user_dev_get_sn;
	agentfunc.pfnDataWrite = user_dev_save_data;
	agentfunc.pfnDataRead = user_dev_load_data;
	agentfunc.pfnRecvCmd = user_recv_cmd;
	agentfunc.pfnSendData = coap_lowlevel_send;
	
	coapfunc.pfnCoapBuild = coap_all_build;
	coapfunc.pfnCoapPktfree = coap_free_pkt;
	coapfunc.pfnCoapPktMalloc = coap_malloc_pkt;
	coapfunc.pfnGetRandom = coap_get_random;
	coapfunc.pfnGetToken = coap_get_token;
	coapfunc.pfnSetCoapHeader = coap_set_header;
	coapfunc.pfnSetCoapOption = coap_set_option;
	coapfunc.pfnSetCoapPayload = coap_set_payload;
	coapfunc.pfnSetCoapToken = coap_set_token;
	//todo
	agentfunc.flashAddr = 0x00ff0000;
	agentfunc.uiLength=256;
	agentfunc.iSocket = socket;
	
	UMini_Initial(agentfunc, coapfunc);

	return 0;
}

void oceancon_task(void * pvParameters)
{
	unsigned char data[8] = {0};
	int leds;
	int testd = 0;
	char showd;
	/* Set the IoT GateWay ip and port */
//	LOS_SetIoTGW_AddrInfo("0.0.0.0", "5683");

	/* Init coap stack and huawei ocean connect agent */
	LOS_OceanCon_Init();
	/* start ocean connect bind and login process */
	LOS_OceanCon_Login();
	
	/* Init led matrix hardware driver */
	LOS_TaskLock();
//	LOS_MAX7219_Init();
	LOS_TaskUnlock();
	//LOS_MAX7219_show('3');

	
	//wait socket ok
	osDelay(5000);

	while(1)
	{
		//0x100 x00
		//todo 4
		/*
		leds = led_get_status();
		if (leds)
		{
			data[0] = 0xAA;
			data[1] = 0x72;
			data[2] = 0x00;//00 means device data up
			data[3] = 0x00;
			data[4] = 0x00;//led on
			data[5] = 0x10;//led brightness
		}
		else
		{
			data[0] = 0xAA;
			data[1] = 0x72;
			data[2] = 0x00;//00 means device data up
			data[3] = 0x00;
			data[4] = 0x01;//led off
			data[5] = 0x10;//led brightness
		}
		*/
		data[0] = 0xAA;
		data[1] = 0x72;
		data[2] = 0x00;//00 means device data up
		data[3] = 0x00;
//		data[4] = 0x03;//led on
//		data[5] = 0x06;//led brightness
//		if (!get_cmd)
//		{
//			testd = (testd + 1)%36;
//			data[5] = testd;
//			showd = max7219_get_show_data(data[5]);
//			LOS_TaskLock();
////			LOS_MAX7219_show(showd);
//			LOS_TaskUnlock();
//		}
//		else
//		{
//			data[5] = uset_value;
//			testd = uset_value;
//		}
		if (rank>=3)
		{
			data[4] = 3;
			LOS_EvbLedControl(LOS_LED2, LED_ON);	
//			Beep_Set(BEEP_ON);
		}
		else
		{
			data[4] = 0;
		}
		UMini_Report(data, 5);
		printf("UMini_Report\r\n");
		osDelay(5000);
	}

}


void LOS_OceanCon_Sample(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "OceanSample";
	thread_def.stacksize = 0x400;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)oceancon_task;
	osThreadCreate(&thread_def, NULL);
}
