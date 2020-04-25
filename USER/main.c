#include "stm32f4xx.h"
#include "board.h"
#include "rtthread.h"

#include "stdio.h"
#include "mfrc522.h"
#include "timer.h"
#include "led.h"
#include "rtc.h"
#include "supersonic.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "mq2.h"
#include "OLED_I2C.h"
#include "dht11.h"
#include "flash.h"
#include "key_int.h"
#include "esp8266.h"
#include "flame.h"
u8  sm05cmdbuf[15]={14,128,0,22,5,0,0,0,4,1,157,16,0,0,21};//д�������#
char api_key[50]={"xk08oGjrTpjHE6V0gzxXkUdnXlM="};
char ID[20]={"589803550"};
uint16_t FLAMEFLAG=0;//�����־
uint16_t CloseFLAMEFLAG=0;//�����־
/*==================SRAM��̬�ڴ�========================*/
#if 1
/*�����߳̿��ƿ�*/
static struct rt_thread event_thread;
/*����һ���߳�ջ*/
ALIGN(RT_ALIGN_SIZE)  
/*RT_ALIGN_SIZE ��rtconfig.h ���һ���� Ϊ4 
Ҫ�����߳�ջ��ʱ����RT_ALIGN_SIZE ���ֽڶ���*/
static rt_uint8_t rt_event_thread_stack[1024];


/*led2 �߳̿��ƿ�*/
static struct rt_thread mainevent_thread;/*�߳̿��ƿ�*/
static rt_uint8_t rt_mainevent_thread_stack[2048];/*�߳�ջ�Ĵ�С*/
static void mainevent_thread_entry(void *parameter);/*�߳���ں���*/
/*=====================================================*/



/*ˢ�� �߳̿��ƿ�*/
static struct rt_thread cardOpenDoor_thread;
static rt_uint8_t cardOpenDoor_thread_stack[1024];
static void cardOpenDoor_thread_entry(void *parameter);
/*=====================================================*/
#endif 

/*�����̺߳���*/
static void event_thread_entry(void * parameter);
static void mainevent_thread_entry(void * parameter);
static void cardOpenDoor_thread_entry(void * parameter);



/*�¼��߳�*/
void event_thread_entry(void * parameter)
{
		uint8_t temp;
		uint8_t humi ;
		int distance;
	while(1){
		printf("thread 1 run \r\n");
	  //dht11_read_data(&temp,&humi);
		
		 // printf("temp : %d,humi %d\r\n",temp,humi);
		 // sprintf((char *)temp_buf,"%d",temp);
	   // sprintf((char *)humi_buf,"%d",humi);	
		 //OLED_ShowStr(40,2,mq2_buf,2);
	   // OLED_ShowStr(40,4,temp_buf,2);
    //	OLED_ShowStr(40,6,humi_buf,2);
		//��LOED������ʾ
		//�������Ӧ�Ĺ��� �򿪷��ȿ���
		//���ڷ��͵�8266
//     printf("thread11 run \r\n");
		  distance=get_sro4_distance();
		  printf("distance: %d \r\n",distance);
		  sendDataTOONENET(ID,api_key,"distance",distance,0);
			if(distance<=30)
			{
				//��������������ʾ
				led_onoff(1,0);
				rt_thread_delay(100);
				led_onoff(1,1);	
				rt_thread_delay(100);
			}
			if(humi>30)
			{
				//��������
			
			}
			
			rt_thread_delay(200);
		}
			


}
/*���߳� ����Ϳ�ȼ����*/
void mainevent_thread_entry(void *parameter)
{
	float mq2_value=0.0;
	u8 mq2_buf[10]={0};
	while(1){
		printf("thread 2 run \r\n");
		if(FLAMEFLAG==1)
		{
			printf("flame*************************************\n");
			FLAMEFLAG=0;
			 sendDataTOONENET(ID,api_key,"flame",0xff,0);
			while(1)
			{
				OLED_ShowCN(32,0,24);//��
				OLED_ShowCN(48,0,25);//��
				led_onoff(2,1);
				rt_thread_delay(100);
				led_onoff(2,0);
				OLED_ShowStr(32,0,"  ",2);	
				OLED_ShowStr(48,0,"  ",2);	
				rt_thread_delay(100);
				if(CloseFLAMEFLAG==1)
				{
					CloseFLAMEFLAG=0;
					break;
				}
				rt_thread_delay(100);
					
			}
			
			//������
		  //�������Ӵ򿪻���		
		}	
		mq2_value = get_adc2value()*3.3/0xfff;
		sprintf((char *)mq2_buf,"%.2f",mq2_value);
		printf("mq2_value: %f \n",mq2_value);
		sendDataTOONENET(ID,api_key,"combustiblegas",0,mq2_value);
		OLED_ShowStr(40,2,mq2_buf,2);
		if(  mq2_value >2.5 )//���ǿ�ȼ�����򱨾�
		{
			led_onoff(2,1);
			rt_thread_delay(100);
			led_onoff(2,0);
			rt_thread_delay(100);
		}
		rt_thread_delay(200);
	}
}
	

//ˢ���߳�
void cardOpenDoor_thread_entry(void * parameter)
{
	u32 cardUID[2]={0};
	u32 keybuf[4]={0};
	u32 addrx ;
	uint8_t passwddata[16]={0};
		while(1)
		{
			 printf("thread 3 run \r\n");
			//break;
			//����һ�������ж�
			//��������-----�Ǽǿ� //������д����flash�ͽ�����д�����������
			//û�а��� //һֱ�ڶ�ȡ��
			//�ϱ���8266
			if(cardRegisterFlag)//���µǼǰ���
			{	
				printf("cardRegisterFlag %d\n",cardRegisterFlag);
				cardRegisterFlag =0 ;
				addrx =ADDR_FLASH_SECTOR_4;
				while(rwCardData(1,sm05cmdbuf,16) != 16)
				{
					
					printf("data write error  1\r\n");
					 rt_thread_delay(2000);					
				}
//				for(int i=0 ;i<16 ;i++)
//				{
//					if(passwddata[i] == sm05cmdbuf[i] )
//						printf(" %d",passwddata[i]);
//				}
//					printf("\n");
			
				cardUID[1] = cardAllInfo_IDA[0] << 24 | cardAllInfo_IDA[1]<< 16|cardAllInfo_IDA[2]<< 8| cardAllInfo_IDA[3];
				sendDataTOONENET(ID,api_key,"cardUID",cardUID[1],0	);//�ϴ�����ID
			 while(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,
				{
					addrx+=4;	
				}
				write_flash(addrx,cardUID[1]);//������д��ȥflah����
					
			}
			else if(rwCardData(0,passwddata,16)==16)
			{
				for(int i=0 ;i<16 ;i++)
				{
				 	printf(" %d",passwddata[i]);
					if(passwddata[i] != sm05cmdbuf[i] )
						printf("card data error\r\n");
				
				}
				printf("\n");
				addrx =  ADDR_FLASH_SECTOR_4;
				cardUID[1] = cardAllInfo_IDA[0] << 24 | cardAllInfo_IDA[1]<< 16|cardAllInfo_IDA[2]<< 8| cardAllInfo_IDA[3];
				sendDataTOONENET(ID,api_key,"cardUID",cardUID[1],0	);//�ϴ�����ID
				while(STMFLASH_ReadWord(addrx)!=cardUID[1])//�з�0XFFFFFFFF�ĵط�,Ҫ�����������&& STMFLASH_ReadWord(addrx) !=0XFFFFFFFF
				{
					addrx+=4;
					if(addrx ==0x0801FFF)
					{
						printf("card open error \r\n");
						//��flash�б���ƥ�俨���к�
				    //��flash�б���ƥ�俨��������		
						break;
					
					}
					
				}	
				printf("STMFLASH_ReadWord(addrx): %02x\n",STMFLASH_ReadWord(addrx));
				printf("this is a right card\r\n");						
			}
		   rt_thread_delay(1000);

	  }

}
int main()
{
	
	//��ʼ��GPIO
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	timer1_init();//��ʼ����ʱ�� 2 3
	usart1_init();
	usart3_init();
	DWT_Delay_Init();
	adc_init();//ADC��ʼ��
	MQ2_init();//��ȼ���崫������ʼ��
	dht11_init();//��ʪ�ȳ�ʼ��	
	flame_init();//���洫������ʼ��
	supersonic_init();//��������ʼ��
	I2C_Configuration();
	MFRC522_Initializtion();			//��ʼ��MFRC522 
	OLED_Init();
	show_mq2_dht11();
	key_int_init();
	flash_init();
  printf("strart   run \r\n ");
	ESP8266_STATCPClient_MODE_Init(); //��ʼ��8266
	/******************���������̡߳���SRAM��̬�ڴ�******************************/
	//��ʼ���߳�
#if 1
	//�¼��߳�
	rt_thread_init(&event_thread,         /*�߳̿��ƿ�*/
									"event",				       /*�߳�����*/
									event_thread_entry,   /*�߳���ں���*/
									RT_NULL,             /*�߳���ں����Ĳ���*/
									&rt_event_thread_stack[0],/*�߳�ջ����ʼ��ַ*/
									sizeof(rt_event_thread_stack),/*�߳�ջ�Ĵ�С*/
										3,                  /*�߳� �����ȼ�*/
									20                    /*�̵߳�ʱ��Ƭ*/
															
	);                                             
	//��ʼ�����߳�
	rt_thread_init(&mainevent_thread,         /*�߳̿��ƿ�*/
									"mainevent",				       /*�߳�����*/
									mainevent_thread_entry,   /*�߳���ں���*/
									RT_NULL,             /*�߳���ں����Ĳ���*/
									&rt_mainevent_thread_stack[0],/*�߳�ջ����ʼ��ַ*/
									sizeof(rt_mainevent_thread_stack),/*�߳�ջ�Ĵ�С*/
										3,                  /*�߳� �����ȼ�*/
									20                    /*�̵߳�ʱ��Ƭ*/
															
	);
	rt_thread_init(&cardOpenDoor_thread,         /*�߳̿��ƿ�*/
									"cardOpenDoor",				       /*�߳�����*/
									cardOpenDoor_thread_entry,   /*�߳���ں���*/
									RT_NULL,             /*�߳���ں����Ĳ���*/
									&cardOpenDoor_thread_stack[0],/*�߳�ջ����ʼ��ַ*/
									sizeof(cardOpenDoor_thread_stack),/*�߳�ջ�Ĵ�С*/
										2,                  /*�߳� �����ȼ�*/
									20                    /*�̵߳�ʱ��Ƭ*/
															
	);
	/*���߳�*/
	rt_thread_startup(&event_thread);
	rt_thread_startup(&mainevent_thread);
	rt_thread_startup(&cardOpenDoor_thread);
#endif

									
}


