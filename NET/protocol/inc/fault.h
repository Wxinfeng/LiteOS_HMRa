#ifndef _FAULT_H_
#define _FAULT_H_


#define DEV_OK		1
#define DEV_ERR		0


typedef struct
{
	unsigned short NET_DEVICE_OK : 1;	//�����豸������־λ

} CHECK_INFO;

typedef enum
{

	FAULT_NONE = 0,		//�޴���
	FAULT_REBOOT,		//������������
	FAULT_EDP,			//EDPЭ�����
	FAULT_NODEVICE,		//Ӳ�����ߴ��󣬱���8266����6311�Ӵ�������sim���Ӵ�������Ӳ��ԭ������Ĵ���

} FAULT_TYPE;

extern unsigned char faultType;

extern unsigned char faultTypeReport;




void Fault_Process(void);
extern CHECK_INFO checkInfo;


#endif
