
#ifndef _MQ_5APP_H_
#define _MQ_5APP_H_


typedef struct packet{
	char packetLen;							        //信息长度   	可变长
	char type;							            //数据/应答
	char command;	                      //命令字							
	char errCode;                       //错误码
	char *data;								        //数据
}packet,*LPPACKET;

void LOS_task(void);

#endif
