#include "umini.h"
#include "umini_prv.h"

#include "stdlib.h"
#include "string.h"
#include "coap.h"
#define    nodata   0x0
//��1��==================================AgentTiny�ӿ������ݽṹ==================================
//=======(1)=======�ӿڼ�飺4��api��4���ص�����������豸��tcp/ipЭ��ջ����mcuֱ��ʹ��GSMģ����������������2��sockSend/sockRecv�Ļص�������coap���ͺͽ���
PFN_ONRECV    g_pfnUminiOnRecv = UM_NULL;


//��2��==================================Tiny����ҵ���߼���α��==================================
//==============AgentTiny.c==============
board_callback_t g_board_func = {0};
coap_callback_t  g_coap_func={0};
unsigned char *g_flash = 0;
uint8_t g_Token[TOKLENMAX] = {0xf1};
uint8_t g_Token_len = 8;
ST_UMINI_MGR  g_stUminiMgr = {0};
unsigned char g_DeviceSN[DEVICESN] = {'\0'};
uint8_t   g_DeviceSNLen=0;
uint8_t   UM_outbuff[256] = {0};
uint8_t   g_outbuf[256] = {0};
uint8_t   lifetime=0;
uint16_t  msgid = 0x0101;

static unsigned char   UM_RES[3]={'t'};
static unsigned char   UM_RES1[3]={'r'};
static unsigned char   UM_RES2[16]="ep=";

UM_API_FUNC  UM_RET UMini_Initial(board_callback_t boardfunc,coap_callback_t coapfunc)
{
	g_board_func.pfnGetSN = boardfunc.pfnGetSN;
	g_board_func.pfnDataWrite = boardfunc.pfnDataWrite;
	g_board_func.pfnDataRead = boardfunc.pfnDataRead;
	g_board_func.pfnRecvCmd = boardfunc.pfnRecvCmd;
	g_board_func.pfnSendData=boardfunc.pfnSendData;
	g_board_func.flashAddr = boardfunc.flashAddr;
  g_board_func.uiLength=boardfunc.uiLength;  
  g_board_func.iSocket=boardfunc.iSocket;	
  g_flash = g_board_func.pfnDataRead(g_board_func.flashAddr,g_board_func.uiLength);
	
	
	g_coap_func.pfnCoapPktMalloc=coapfunc.pfnCoapPktMalloc;
	g_coap_func.pfnCoapPktfree=coapfunc.pfnCoapPktfree;
	g_coap_func.pfnGetRandom=coapfunc.pfnGetRandom;
	g_coap_func.pfnGetToken=coapfunc.pfnGetToken;
	g_coap_func.pfnSetCoapHeader=coapfunc.pfnSetCoapHeader;
	g_coap_func.pfnSetCoapOption=coapfunc.pfnSetCoapOption;
	g_coap_func.pfnSetCoapPayload=coapfunc.pfnSetCoapPayload;
	g_coap_func.pfnSetCoapToken=coapfunc.pfnSetCoapToken;
	g_coap_func.pfnCoapBuild=coapfunc.pfnCoapBuild;
    if(NULL!= g_flash[0])
    {    //u8BindFlag ȫ�ֱ�����Ϊflash����  ��ΪNB�豸һ�ΰ� �ϵ�����֮�����ֱ�ӱ�����  ?        if(/*����g_flash������IotIp port DeviceSN u8BindFlag*/)
		  if(nodata!=g_flash[0]) 
			{
			 //�����Ҫ���ú����õ���
			 g_board_func.pfnGetSN (g_DeviceSN,&g_DeviceSNLen);
			 g_stUminiMgr.u8BindFlag = 1;
			 return UM_OK;
			}
			else
      {
        g_stUminiMgr.u8BindFlag = 0;
				return UM_OK;
      }
			//todo  �������������ô�ж�
			/*else if{
			//�����������
			g_board_func.pfnDataWrite(g_board_func.flashAddr, 0, 512);
			g_stUminiMgr.u8BindFlag = 0;
			}*/
		}
		else if(NULL== g_flash[0])
		{
		  return UM_ERR;
		}
		return UM_ERR;
}

/***
****1.��ʼ��ʱ��flash�ж�ȡƽ̨ip port���豸sn��bindflag
****2.��ip port  SN ƴװ��ip+port+ POST+CON +/t/r?ep=id ��CoAP���İ�
****3.��CoAP ���İ�ת�����ܷ��͵��ַ���pucBuf
****4.����ע��ķ��ͻص����������ݷ���ȥ
***/
UM_UINT8 umini_bind()
{
    //���� g_flash��������Ҫ��IP ��port��sn����װbind����
	  //��װ�󶨱���             ������װ��ʱ����Ҫд�ض���Э���ַ�  /t/r

	 // unsigned char UM_buf[100];
   /* size_t option_len,option1_len,option2_len;
    uint16_t delta = 0;			 			 
    coap_packet_t *pkt; 
    coap_buffer_t tok; */	
		unsigned int rantok;
	

		unsigned char  devicesnLen;
    //todo
	  size_t buflen; 
		UM_COAPPKT pkt;
  	pkt = g_coap_func.pfnCoapPktMalloc();
		
	//  pkt = (coap_packet_t *)malloc(sizeof(coap_packet_t));

  //  memset(pkt,0,sizeof(coap_packet_t));
  /*	pkt->hdr.ver = 0x01;
    pkt->hdr.t = COAP_TYPE_CON;//COAP_TYPE_ACK;
    pkt->hdr.tkl = 0;
    pkt->hdr.code = COAP_METHOD_POST;//COAP_METHOD_PUT;
    pkt->hdr.id[0] = (msgid>>8)&0x00ff;
    pkt->hdr.id[1] = msgid&0x00ff;
		msgid++;
	  pkt->numopts = 3;*/
		//g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_CON,0,COAP_METHOD_POST);
    g_board_func.pfnGetSN(UM_RES2+3,&devicesnLen);
		rantok = g_coap_func.pfnGetRandom();
		g_Token[1] = (uint8_t)(rantok>>24);
		g_Token[2] = (uint8_t)(rantok>>16);
		g_Token[3] = (uint8_t)(rantok>>8);
		g_Token[4] = (uint8_t)(rantok);
		g_coap_func.pfnSetCoapToken(pkt, g_Token, g_Token_len);
		g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_CON, g_Token_len,COAP_METHOD_POST);
		
    // need token in response
    //if (UM_NULL==tok) {
		/*	rantok = coap_get_random();
			g_Token[1] = (uint8_t)(rantok>>24);
			g_Token[2] = (uint8_t)(rantok>>16);
			g_Token[3] = (uint8_t)(rantok>>8);
			g_Token[4] = (uint8_t)(rantok);
			tok.p = g_Token;
			tok.len = g_Token_len;
      pkt->hdr.tkl = tok.len;
      memcpy(&pkt->tok, &tok, sizeof(coap_buffer_t));
    //}*/
		
	    //option ת��Ϊpkt->opt[]	
	    //option ת��ΪUM_outbuff
			
		/*  option_len = coap_opt_encode((coap_opt_t *)UM_outbuff, sizeof(UM_outbuff),COAP_OPTION_URI_PATH, (unsigned char *)res, 1);
			p = UM_outbuff;
		 //UM_outbuff ת��Ϊpkt->opt[]
		  coap_parseOption(&pkt->opts[0], &delta, &p,option_len);
				//option	
		  option1_len = coap_opt_encode((coap_opt_t *)UM_outbuff+option_len, sizeof(UM_outbuff),COAP_OPTION_URI_PATH-delta,(unsigned char *)res1, 1);
			p = UM_outbuff+option_len;
		  coap_parseOption(&pkt->opts[1], &delta, &p,option1_len);
			
			option2_len = coap_opt_encode((coap_opt_t *)UM_outbuff+option_len+option1_len, sizeof(UM_outbuff),COAP_OPTION_URI_QUERY-delta,(unsigned char *)res2,15);
			p = UM_outbuff+option_len+option1_len;
		  coap_parseOption(&pkt->opts[2], &delta, &p ,option2_len);*/
			
	    buflen = (size_t)sizeof(g_outbuf); 
			//typedef UM_VOID (*PFN_SET_COAP_OPTION)(UM_COAPPKT pkt,UM_UINT8 optsIndex,UM_UINT8 *delta,UM_UINT8 *p,UM_UINT8 plen);//Tiny���յ���������󴫸��豸Ӧ�õĻص�
			g_coap_func.pfnSetCoapOption(pkt,0,COAP_OPTION_URI_PATH, (unsigned char *)UM_RES,1);
			g_coap_func.pfnSetCoapOption(pkt,1,COAP_OPTION_URI_PATH, (unsigned char *)UM_RES1,1);
			g_coap_func.pfnSetCoapOption(pkt,2,COAP_OPTION_URI_QUERY, (unsigned char *)UM_RES2,devicesnLen+3); 
			//coap pktת��Ϊ�ɷ��͵��ַ���
      //coap_build(g_outbuf, &buflen, pkt);
		  g_coap_func.pfnCoapBuild(g_outbuf, &buflen, pkt); 
			g_coap_func.pfnCoapPktfree(pkt);
			//free(pkt);
	    //����bind����    g_IotIp,g_IotPort,g_DeviceSN;
	    g_board_func.pfnSendData(g_board_func.iSocket,g_outbuf,buflen);
	    return UM_OK;
}
/***
****1.��ʼ��ʱ��flash�ж�ȡƽ̨ip port
****2.��ip port  ����data  ƴװ��ip+port+205content+data  ��CoAP���İ�
****3.��CoAP ���İ�ת�����ܷ��͵��ַ���pucBuf
****4.����ע��ķ��ͻص����������ݷ���ȥ
***/
#if 0
UM_UINT8 DataReport(const coap_buffer_t* tok,unsigned char *data, unsigned int dlen)
{
   
    //todo
	  //uint8_t    UM_outbuff[256];		 
    coap_packet_t *pkt;
	  size_t option_len;
	  const uint8_t *p;
	  uint16_t delta = 0;		
    //todo
	  size_t buflen ;
  	pkt = (coap_packet_t *)malloc(sizeof(coap_packet_t));
	  //inject  header
  	memset(pkt,0,sizeof(coap_packet_t));
  	pkt->hdr.ver = 0x01;
    pkt->hdr.t = COAP_TYPE_NONCON;
    pkt->hdr.tkl = 0;
    pkt->hdr.code = COAP_RSPCODE_CONTENT;
    //pkt->hdr.id[0] = 0x01;
    //pkt->hdr.id[1] = 0x01;
    pkt->hdr.id[0] = (msgid>>8)&0x00ff;
    pkt->hdr.id[1] = msgid&0x00ff;
		msgid++;
	  pkt->numopts = 1;
	
    //inject token
    if (tok) {
         
   			pkt->hdr.tkl = tok->len;
        memcpy(&pkt->tok, tok, sizeof(coap_buffer_t));
    }
	  buflen = (size_t)sizeof(UM_outbuff);
		//option	
	    //option->UM_outbuff
		  option_len = coap_opt_encode((coap_opt_t *)UM_outbuff, sizeof(UM_outbuff),COAP_OPTION_OBSERVE, &lifetime, 1);
			p = UM_outbuff;
		  lifetime++;
		  coap_parseOption(&pkt->opts[0], &delta, &p,option_len);
		//inject payload
    if(data){
		pkt->payload.len=dlen;//strlen((const char *)data);
		pkt->payload.p =data;
		}
		//translate pkt to Outbuf
    coap_build(g_outbuf,&buflen,pkt);
		free(pkt);
	  g_board_func.pfnSendData(g_board_func.iSocket, g_outbuf,buflen);
		return UM_OK;
}
#endif
UM_UINT8 dataReport(const coap_buffer_t* tok,unsigned char *data, unsigned int dlen)
{
   
    //todo
	  //uint8_t    UM_outbuff[256];		 
	  /*size_t option_len;
	  const uint8_t *p;
	  uint16_t delta = 0;		*/
    //todo
	 // size_t buflen ;
	size_t buflen;	
	UM_COAPPKT pkt;
	pkt = g_coap_func.pfnCoapPktMalloc();
	  //inject  header
  	//memset(pkt,0,sizeof(coap_packet_t));
  	/*pkt->hdr.ver = 0x01;
    pkt->hdr.t = COAP_TYPE_NONCON;
    pkt->hdr.tkl = 0;
    pkt->hdr.code = COAP_RSPCODE_CONTENT;*/
	  
    //pkt->hdr.id[0] = 0x01;
    //pkt->hdr.id[1] = 0x01;
   /* pkt->hdr.id[0] = (msgid>>8)&0x00ff;
    pkt->hdr.id[1] = msgid&0x00ff;
		msgid++;*/
	  //pkt->numopts = 1;
	
    //inject token
   /* if (tok) {
         
   			pkt->hdr.tkl = tok->len;
        memcpy(&pkt->tok, tok, sizeof(coap_buffer_t));
    }*/
		
		g_coap_func.pfnSetCoapToken(pkt,g_Token,g_Token_len);
		g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_NONCON,g_Token_len,COAP_RSPCODE_CONTENT);
		lifetime++;
		g_coap_func.pfnSetCoapOption(pkt, 0x00, COAP_OPTION_OBSERVE, &lifetime, 1);
	 /* buflen = (size_t)sizeof(UM_outbuff);
		//option	
	    //option->UM_outbuff
		  option_len = coap_opt_encode((coap_opt_t *)UM_outbuff, sizeof(UM_outbuff),COAP_OPTION_OBSERVE, &lifetime, 1);
			p = UM_outbuff;
		  lifetime++;
		  coap_parseOption(&pkt->opts[0], &delta, &p,option_len);
		//inject payload
    if(data){
		pkt->payload.len=dlen;//strlen((const char *)data);
		pkt->payload.p =data;
		}
		//translate pkt to Outbuf
    coap_build(g_outbuf,&buflen,pkt);
		free(pkt);*/
		buflen = (size_t)sizeof(g_outbuf);
		g_coap_func.pfnSetCoapPayload(pkt,data,dlen);
	  g_coap_func.pfnCoapBuild(g_outbuf, &buflen, pkt);
		g_coap_func.pfnCoapPktfree(pkt);
	  g_board_func.pfnSendData(g_board_func.iSocket, g_outbuf,buflen);
		for (int i=0;i<=buflen;i++)
		{
			printf("g_outbuf : 0x%02x\r\n",g_outbuf[i]);
		}
		return UM_OK;
}

/*
*��������»ᷢ��4������ı���:
��������:
1.��ʱ�ı���:CON post ip:port+/t/r?ep=SN
2.�ϱ�������������:     ip:port+2.05content+data
*/
//��Ϊcase����ifelse �ֱ���Ȼ��ȥ��UMini_Drive
UM_API_FUNC UM_RET UMini_Report(unsigned char *pucData, unsigned int uiLen)
//unsigned int UMini_Report(unsigned char *pucData, unsigned int uiLen)
{
	//��1����pucData����g_stUminiMgr.stReportBuf��
	//��2��
	 coap_buffer_t  tok;
	 tok.p = (const uint8_t*)g_Token;
	 tok.len = g_Token_len;
	if(0 == g_stUminiMgr.u8BindFlag)
	{
		umini_bind();//��� g_flash ����Ϊ�գ�����AppID��DeviceID��DeviceSecret�ȣ�˵����û�󶨡�
	}
	else//�豸�Ѿ���ƽ̨����ֱ���ϱ�����
	{
		dataReport((const coap_buffer_t *)&tok, pucData, uiLen);//forѭ����ÿ�ζ���g_stUminiMgr.stReportBuf�л�����豸���ݣ�ͨ�����緢�͸�������
	}
	return 0;
}

//��os������os������������������жϺ����NetRecv
//��os�Ҷ�����ģ�����������̵߳���NetRecv
/*
*����������յ���������ı���:
1.�󶨳ɹ���ƽ̨��Ӧ ACK 204 Changed/201Created?
2.�󶨳ɹ���ƽ̨����GET /t/d  observe=0
3.�����·�ʱ�� POST /t/d  data
*/
//UM_API_FUNC UM_RET  NetRecv(coap_packet_t *pucData, unsigned int uiLen)
//�����������Coap  endpoint ����NetRecvֻ�ǶԽ��յ��ı���pkt����ȷ����֤��
UM_API_FUNC UM_RET UMini_NetRecv(uint8_t t, uint8_t code,uint8_t  delta, const uint8_t *pToken,uint8_t pTokenLen)
{
    if(COAP_TYPE_CON == t && COAP_METHOD_GET==code && COAP_OPTION_OBSERVE==delta)//bind success
    {
		  memcpy(g_Token,pToken,pTokenLen);
			g_stUminiMgr.u8BindFlag = 1;
			g_Token_len = pTokenLen; 
			return UM_CON;
		}
    if(COAP_TYPE_RESET == t)//bind failed
    {
		  memset(g_Token,0xf1,sizeof(g_Token));
			g_stUminiMgr.u8BindFlag = 0; 
			g_Token_len = 8;
			return UM_CON;
		}
		if(COAP_TYPE_ACK == t && COAP_METHOD_EMPTY == code)//encode/decode success&&commond
		{
			return UM_NON;
		}

		return UM_CON;//defualt no ack

}
#if 0
UM_API_FUNC UM_RET  NetRecv(uint8_t *buff, unsigned int uiLen)
{
	  
	
  #if 0 
	//(1)coap��pucData���н����õ�****��UMini_CoapDecode
	 if(COAP_TYPE_ACK==pkt->hdr.t)
   {
    g_stUminiMgr.u8BindFlag = EN_UMINI_STA_BIND;
		 //todo
	  //g_board_func.pfnDataWrite(g_board_func.flashAddr, 0, 512);
		return UM_OK;
	 }
	else if(COAP_RSPCODE_CREATED==pkt->hdr.code||COAP_RSPCODE_CHANGED==pkt->hdr.code)//1.ƽ̨���ĺ���Ӧ:      ip:port+2.05content
	{  /*�� coap Token ���ͱ�����Ӧ1*/
		 if(UM_NULL!=pkt->tok.p)
		 {
			 //todo
		  g_board_func.pfnDataWrite(g_board_func.flashAddr,pkt->tok.p ,pkt->tok.len);
		  return UM_OK;
		 }
		 else{
		   return UM_ERR;
		 } 
	}
	else if(COAP_RSPCODE_CREATED==pkt->hdr.code)  //2.ƽ̨���������Ӧ:     ip+port+2.04changed
	{
    //����CoAP Token��
	  /*�� coap Token ���ͱ�����Ӧ2*/
	  g_board_func.pfnRecvCmd(/*(****)*/, /*(****�ĳ���)*/);
	}
	else{
     return UM_ERR;
	}
	#endif
	          //size_t rsplen = sizeof(buff)+1;   
    int rc;
    coap_packet_t pkt; 
    coap_packet_t rsppkt;
	    //todo
	  //uint8_t scratch_raw[1024]; 
		uint8_t scratch_raw[128];  
    coap_rw_buffer_t scratch_buf; 
    scratch_buf.len=sizeof(scratch_raw);
    scratch_buf.p=scratch_raw;
		rc = coap_parse(&pkt,(const uint8_t*)buff,uiLen/*strlen((const char*)buff)*/);
	  if(0 != rc)        //prase the received "buff" to "pkt"
    {
      //printf("Bad packet rc=%d\r\n", rc);
		  return UM_ERR;
    }
    else if(COAP_TYPE_CON==pkt.hdr.t && COAP_METHOD_GET==pkt.hdr.code && COAP_OPTION_OBSERVE==pkt.opts[0].num)//bind success
    {
		  memcpy(g_Token,pkt.tok.p,pkt.tok.len);
			g_stUminiMgr.u8BindFlag=1;
			g_Token_len = pkt.tok.len; 
		}
		else if(COAP_TYPE_CON==pkt.hdr.t&&COAP_METHOD_POST==pkt.hdr.code&&'t'==pkt.opts[0].buf.p[0])//encode/decode success&&commond
		{
		}
    else if(COAP_TYPE_RESET==pkt.hdr.t)//bind failed
    {
		  memset(g_Token,0xf1,sizeof(g_Token));
			g_stUminiMgr.u8BindFlag=0; 
			g_Token_len = 8;
		}
		//else if(COAP_TYPE_ACK==pkt.hdr.t&&COAP_METHOD_EMPTY==pkt.hdr.code)//encode/decode success&&commond
		//{
		//	return   UM_OK;
		//}
	  size_t rsplen = uiLen+1;//sizeof(buff)+1;                                         
    coap_handle_req(&scratch_buf, &pkt, &rsppkt);                  //handle the prased  pkt then make response pkt
		rc = coap_build((uint8_t*)buff, &rsplen, &rsppkt);
    if (0 != rc) //
    {
       return  UM_ERR;
    }
    else
    {  
				 g_board_func.pfnSendData(g_board_func.iSocket, buff,rsplen);
				 return   UM_OK;
    }
}
#endif

//10�붨ʱ��
void TimerPro(UM_VOID)
{
	if(0 == g_stUminiMgr.u8BindFlag)
	{
		umini_bind();//��� g_flash ����Ϊ�գ�����AppID��DeviceID��DeviceSecret�ȣ�˵����û�󶨡�
	}
}

UM_API_FUNC UM_VOID UMnini_Reset(UM_VOID)
{
	size_t buflen;	
	UM_COAPPKT pkt;
	pkt = g_coap_func.pfnCoapPktMalloc();
	g_coap_func.pfnSetCoapHeader(pkt,0x01,COAP_TYPE_RESET,0,COAP_RSPCODE_RESET);
	buflen = (size_t)sizeof(g_outbuf);

	g_coap_func.pfnCoapBuild(g_outbuf, &buflen, pkt);
	g_coap_func.pfnCoapPktfree(pkt);
	g_board_func.pfnSendData(g_board_func.iSocket, g_outbuf,buflen);
}

//��3��==================================�ؼ�����==================================
//1����ok����һ������Ӳ�����ޣ��޷�����־���Ǵ�����Ϣ��ζ�λ������>����һ��AgentTiny�ṩpringLog�ӿڣ��������or������Ϣ�������豸������Һ��������ܡ�led����զ�����Լ�ʵ�֣�
//
//1�[��ok,���������̣����°󶨡��cig���ɵ�coap token��NB�豸Ҫ�־û��洢������>��ΪNB�豸��һ���ϵ�Ҫbind���õ�coap token��Ȼ���µ����ϵ�Ͳ�����bind����ʱreportʱ�Ƿ�����֮ǰ��coap tokenȥ��

//2����ok��д��flash��ʹ�ô��ڿ����á�NB�豸�϶���server ip/port��λ�ȡ��
//
