#ifndef _FAULT_H_
#define _FAULT_H_


#define DEV_OK		1
#define DEV_ERR		0


typedef struct
{
	unsigned short NET_DEVICE_OK : 1;	//网络设备正常标志位

} CHECK_INFO;

typedef enum
{

	FAULT_NONE = 0,		//无错误
	FAULT_REBOOT,		//死机重启错误
	FAULT_EDP,			//EDP协议错误
	FAULT_NODEVICE,		//硬件掉线错误，比如8266或者6311接触不良、sim卡接触不良等硬件原因引起的错误

} FAULT_TYPE;

extern unsigned char faultType;

extern unsigned char faultTypeReport;




void Fault_Process(void);
extern CHECK_INFO checkInfo;


#endif
