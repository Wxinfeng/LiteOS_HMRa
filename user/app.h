
#ifndef _MQ_5APP_H_
#define _MQ_5APP_H_


typedef struct packet{
	char packetLen;							        //��Ϣ����   	�ɱ䳤
	char type;							            //����/Ӧ��
	char command;	                      //������							
	char errCode;                       //������
	char *data;								        //����
}packet,*LPPACKET;

void LOS_task(void);

#endif
