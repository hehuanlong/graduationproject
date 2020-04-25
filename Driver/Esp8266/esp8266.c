#include "esp8266.h"
#include "usart.h"
#include "delay.h"
extern uint8_t  USART3_RX_BUFF[512];
extern uint32_t USART3_RX_CNT;
extern uint32_t USART3_RX_FLAG;
uint8_t connect_status;
	
/**********************************************************
�������ܣ� STAģʽ+TCP�ͻ���ģʽ��ʼ��
���������
���ز���˵����
AT+CIPSEND\r\n    ��ʼ͸��ģʽ

**********************************************************/
void ESP8266_STATCPClient_MODE_Init()
{
		DELAY_MS(200);
		if(ESP8266_Check_Exist())//�鿴�豸�Ƿ����
			printf("ESP8266 couldn't found!\n");	
		if(ESP8266_CloseEcho())//�������
			printf("ESP8266 close echo fail!\n");	
		if(ESP8266_SetWorkMode(3))
//			printf("ESP8266 set work mode fail!\n");
//		if(ESP8266_ResetSys())
//			printf("ESP8266 reset fail!\n");	
//		if(ESP8266_CloseEcho())
//			printf("ESP8266 close echo fail!\n");	
		DELAY_MS(200);
		if(ESP8266_STATCPClient_SsidPasswords("306","30six888"))
			printf("ESP8266 connect ssid&passwords fail!\n");	
		DELAY_MS(1000);
		if(ESP8266_ConnectNum(0))
			printf("ESP8266 set connect setting fail!\n");
		if(ESP8266_STAClient_Get_IP_MAC())
			printf("ESP8266 get ip fail!\n");
		if(ESP8266_STATCPClient_ConnectServer("183.230.40.33",80))
			printf("ESP8266 connect server fail!\n");
		if(ESP8266_STATCPClient_SetWorkMode(1))//����͸��ģʽ
			printf("ESP8266 set mode fail!\n");
		if((connect_status =GetLinkStatus())!=3)//��ȡ״̬
				printf("ESP8266 link fail!\n");
		
		if(ESP8266_STAClient_SendData())//͸������
			printf("ESP8266 build connect fail!\n");


	//	usart3_puts("setID:1234");
}
/**********************************************************
�������ܣ����ͻ�ִOK������
�������������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
***********************************************************/
uint8_t ESP8266_SendCmd(char *cmd)
{
	int i,j;
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	if(strcmp(cmd,"AT\r\n")==0)
	{
		ESP8266_STAClient_ExitMode();//�˳�͸��ģʽ
		ESP8266_STATCPClient_SetWorkMode(0);//���ù���ģʽ
	}
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART3_RX_FLAG=0;//��־λ��λ
		USART3_RX_CNT=0;//��������
		
		usart3_puts(cmd);
		printf("cmd :%s",cmd);
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				printf("USART3 resive :%s \n",USART3_RX_BUFF);
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//��������
				if(strstr((char *)USART3_RX_BUFF,"OK")!=NULL)
				{
				
					if(strstr(cmd,"CWMODE")!=NULL)//�鿴��������ģʽ
						printf("ESP8266 set work mode success!\n");
					else
						if(strstr(cmd,"CIPMUX")!=NULL)//��ѯ��ģ���Ƿ���������
							printf("ESP8266 connect setting set success��\n");
						else
							if(strstr(cmd,"E0")!=NULL)
								printf("ESP8266 close echo success��\n");
							else
								if(strstr(cmd,"CIPMODE")!=NULL)//ѡ��TCPIPӦ��ģʽ
									printf("ESP8266 STA CIPmode set success!\n");
								else
									printf("ESP8266 exist\n");
					return 0;
				}
				else		
					break;
			}
		
		}

	}
	return 1;
}
/**********************************************************
�������ܣ�����豸�Ƿ����
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
***********************************************************/
uint8_t ESP8266_Check_Exist(void)
{
	return(ESP8266_SendCmd("AT\r\n"));
}
/**********************************************************
�������ܣ�����ģʽ
���������1��2��3����Ӧ������ģʽ 1��station 2��ap 3:1&2
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
**********************************************************/
uint8_t ESP8266_SetWorkMode(int mode) 
{
	char mode_cmd[20];
	switch(mode)
	{
		case 1 :strcpy(mode_cmd,"AT+CWMODE=1\r\n");break;
		case 2 :strcpy(mode_cmd,"AT+CWMODE=2\r\n");break;
		case 3 :strcpy(mode_cmd,"AT+CWMODE=3\r\n");break;
	}
	return(ESP8266_SendCmd(mode_cmd));
}
/**********************************************************
�������ܣ�APģʽ+TCP������ģʽ�����ȵ������������ȵ�����
���������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
˵��:����ָ��:AT+CWSAP="WBYQ_ESP8266","12345678",1,4
							OK
**********************************************************/
uint8_t ESP8266_APTCPServer_SetSsidPasswords(char *ssid,char *passwords)
{
	int i,j;
	char str[50];
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	sprintf(str,"AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n",ssid,passwords,1,4);
	for(i=0;i<RETRY_TIMES;i++)
	{
		usart3_puts(str);
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART3_RX_BUFF);
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//��������
				if(strstr((char *)USART3_RX_BUFF,"OK")!=NULL)
				{
					printf("ESP8266 set ssid:%s,PSW:%s success!\n",ssid,passwords);
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
�������ܣ��������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
����˵��:����ָ��:AT
									OK
***********************************************************/
uint8_t ESP8266_CloseEcho(void)
{
	return(ESP8266_SendCmd("ATE0\r\n"));
}
/**********************************************************
�������ܣ����ÿ����ӷ�ʽ
���������1��������ģʽ 0:������ģʽ
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
����˵��������ָ��:AT+CIPMUX=1
									 OK
***********************************************************/
uint8_t ESP8266_ConnectNum(uint8_t num)
{
	char cmd[15];
	sprintf(cmd,"AT+CIPMUX=%d\r\n",num);
	return(ESP8266_SendCmd(cmd));
}
/**********************************************************
�������ܣ���λ
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
���ڸ�λ���յ������ݣ���ʱ�رմ���ʹ�ܣ����Դ���ʹ�ܲ��رջᵼ�±���
***********************************************************/	
uint8_t ESP8266_ResetSys(void)
{
	usart3_puts("AT+RST\r\n");
	printf("ESP8266 reset success!\n");
	USART3->CR1&=~(1<<13);//��������ʹ��
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	USART3->CR1|=1<<13;//��������ʹ��
	return 0;
}

/**********************************************************
�������ܣ�STAģʽ+TCP�ͻ���ģʽ�����ȵ������������ȵ�����
���������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
AT+CWJAP="SSID","PassWord" �������ӵ���WIFI��������/���ܷ�ʽ/����
                 ���ӳɹ��᷵��  WIFI GOT IP  �ַ���
**********************************************************/
uint8_t ESP8266_STATCPClient_SsidPasswords(char *ssid,char *passwords)
{
	int i,j;
	char str[50];
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	sprintf(str,"AT+CWJAP_DEF=\"%s\",\"%s\"\r\n",ssid,passwords);
	for(i=0;i<RETRY_TIMES;i++)
	{
		usart3_puts(str);
		printf("str: %s\n",str);
		for(j=0;j<4;j++)
		{
			DELAY_MS(4000);
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//�������� 
				if(strstr((char *)USART3_RX_BUFF,"WIFI GOT IP")!=NULL)
				{
					printf("ESP8266 connect ssid:%s,PSW:%s success!\n",ssid,passwords);
					return 0;
				}
			}
		}
	}
	return 1;
}
/**********************************************************
�������ܣ���ѯ����IP��MAC
����������˿ں�
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
����ָ��:AT+CIFSR
				+CIFSR:STAIP,"192.168.43.177"
				+CIFSR:STAMAC,"84:f3:eb:17:ea:8b"
					OK
***********************************************************/
uint8_t ESP8266_STAClient_Get_IP_MAC(void)
{
	int i,j;
	char *temp1;
	char *temp2;
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART3_RX_FLAG=0;//��־λ��λ
		USART3_RX_CNT=0;//��������
		usart3_puts("AT+CIFSR\r\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART3_RX_BUFF);
				if(strstr((char *)USART3_RX_BUFF,"OK")!=NULL)
				{
					temp1=strstr((char *)USART3_RX_BUFF,"TAIP,");
					temp2=strstr(temp1+6,"\"");
					temp1=strstr((char *)USART3_RX_BUFF,"AMAC,");
					temp2=strstr(temp1+6,"\"");
					USART3_RX_FLAG=0;//��־λ��λ
					USART3_RX_CNT=0;//��������
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
�������ܣ�STAģʽ+TCP�ͻ���ģʽ���ӷ�����
���������������IP���˿ں�
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
AT+CIPSTART="TCP","119.29.140.191",8080
CONNECT
OK
**********************************************************/
uint8_t ESP8266_STATCPClient_ConnectServer(char *server_ip,u32 port)
{
	int i,j;
	char str[50];
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	sprintf(str,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server_ip,port);
	for(i=0;i<RETRY_TIMES;i++)
	{
		usart3_puts(str);
		for(j=0;j<4;j++)
		{
			DELAY_MS(1000);
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART3_RX_BUFF);
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//��������
				if(strstr((char *)USART3_RX_BUFF,"CONNECT")!=NULL)
				{
					printf("ESP8266 coonnect ip:%s,port:%d success!\n",server_ip,port);
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
�������ܣ�STAģʽ����ģʽ
���������0��1��Ӧ������ģʽ 0����ͨģʽ 1��͸��ģʽ
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
AT+CIPMODE=1
OK
**********************************************************/
uint8_t ESP8266_STATCPClient_SetWorkMode(int mode) 
{
	char mode_cmd[20];
	switch(mode)
	{
		case 0 :strcpy(mode_cmd,"AT+CIPMODE=0\r\n");break;
		case 1 :strcpy(mode_cmd,"AT+CIPMODE=1\r\n");break;
	}
	return(ESP8266_SendCmd(mode_cmd));
}
/**********************************************
�������ܣ���STAģʽ�½���͸����������
���������ID������
AT+CIPSEND
OK

> 
**********************************************/
uint8_t ESP8266_STAClient_SendData()
{
	int i,j;
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART3_RX_FLAG=0;//��־λ��λ
		USART3_RX_CNT=0;//��������
		usart3_puts("AT+CIPSEND\r\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(40);
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART3_RX_BUFF);
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//��������
				if(strstr((char *)USART3_RX_BUFF,">")!=NULL)//û���жϴ��ڷ������
				{
					printf("ESP8266 build connect success!\n");
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/******************************************************************
�������ܣ�STAģʽ�˳�͸��ģʽ
******************************************************************/
void ESP8266_STAClient_ExitMode()
{
	usart3_puts("+++");
	DELAY_MS(1000);
}


/******************************************************************
��������:��ȡ�豸״̬
*����ֵ�� 0 ʧ�� 
*					2 ��ȡIP
*					3:��������
*					4:ʧȥ����
*

*****************************************************************/
uint8_t GetLinkStatus()
{
	uint8_t status;
	USART3_RX_FLAG=0;
	USART3_RX_CNT=0;
	int i,j;
	printf("get status\r\n");
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART3_RX_FLAG=0;//��־λ��λ
		USART3_RX_CNT=0;//��������
		
		usart3_puts("AT+CIPSTATUS\r\n");
		printf("00000\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(40);
			if(USART3_RX_FLAG)//�յ�һ֡����������
			{
				USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART3_RX_BUFF);
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//��������
				if(strstr((char *)USART3_RX_BUFF,"STATUS:2\r\n")!=NULL)//û���жϴ��ڷ������
				{
					status=2;
					//printf("ESP8266 get status success!\n");
					printf("8266 link status : %d\r\n",status);
					return status;
				}
				else if(strstr((char *)USART3_RX_BUFF,"STATUS:3\r\n")!=NULL)	
				{
					status=3;
					//printf("ESP8266 get status success!\n");
					printf("8266 link status : %d\r\n",status);
					return status;
				
				}
				else if(strstr((char *)USART3_RX_BUFF,"STATUS:4\r\n")!=NULL)	
				{
					status=4;
					printf("8266 link status : %d\r\n",status);
					return status;
				}
				else
					break;
			}
		}
	}
	
	return 0;

}
/******************************************************************
��������:�������ݵ�������
*ID���豸id
api_key:��Կ
*dataname:����������
*data������
*len�����ݳ���
******************************************************************/
uint8_t sendDataTOONENET(char *ID,char *api_key,char *dataname,uint8_t data,float data1)
{
	
	char postcmd[300];
	char getlen[20];
	char temp[100]={0};
	char *post_p1;
	char *post_p2;
	char check[2];
	uint8_t i,j;
	printf("dataname :%s\n",dataname);
	if(data!=0)
	sprintf(temp,"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]}]}	",dataname,data);
	else
	sprintf(temp,"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%f}]}]}	",dataname,data1);	
	uint8_t len =strlen(temp);
	//sprintf(postcmd,"POST/devices/%s/datapoints?type=3 HTTP/1.1\r\napi-key:BZywwSFfJCTMNrl3rOoqOBuhmv4=\r\nHost:api.heclouds.com Content-Length:%d\r\n\n%s",ID,len,getlen); 																			
	sprintf(postcmd,"POST /devices/%s/datapoints HTTP/1.1\r\napi-key:%s\r\nHost:api.heclouds.com\r\nConnection:close\r\nContent-Length:%d\r\n\n%s",\
																			ID,api_key,len,temp);	
	if((connect_status)==3)
	{	
			printf("1\n");
			for(i=0;i<RETRY_TIMES;i++)
			{
				printf("2\n");
				USART3_RX_FLAG=0;//��־λ��λ
				USART3_RX_CNT=0;//��������
				usart3_puts(postcmd);
				for(j=0;j<4;j++)
				{
					DELAY_MS(40);
					printf("3\n");
					if(USART3_RX_FLAG)//�յ�һ֡����������
					{
						printf("4\n");
						USART3_RX_BUFF[USART3_RX_CNT]='\0';//��ӽ�����
						printf("USART1 resive :%s \n",USART3_RX_BUFF);
						USART3_RX_FLAG=0;//��־λ��λ
						USART3_RX_CNT=0;//��������
						if((post_p1=strstr((char *)USART3_RX_BUFF,"errno"))!=NULL)//û���жϴ��ڷ������
						{
							if((post_p2=strstr(post_p1,"}"))!=NULL)
							{
								post_p2=strtok((post_p1),":");
								post_p2=strtok((NULL),";");
								sprintf(check,"%s",post_p2);
								if(check[0]='0')
								{
									printf("send data to server succeed\r\n");
									return 0;
								}
							
							}
							//ESP8266_STAClient_ExitMode();//���ͳɹ����˳�͸��
							//ESP8266_STATCPClient_SetWorkMode(0);//�ر�͸��ģʽ
						
							
						}
						else		
							break;
						
						
					}
				}
			}
	}
	else
	{
		printf("link error code :%d\r\n",connect_status);
		//�˳�͸��
		ESP8266_STAClient_ExitMode();//���ͳɹ����˳�͸��
	  if(ESP8266_STATCPClient_ConnectServer("183.230.40.33",80))
			printf("ESP8266 connect server fail!\n");
		//�ر�͸��ģʽ
		//��������
	
	}
	return 0;

}

