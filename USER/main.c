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
u8  sm05cmdbuf[15]={14,128,0,22,5,0,0,0,4,1,157,16,0,0,21};//写入的数据#
char api_key[50]={"xk08oGjrTpjHE6V0gzxXkUdnXlM="};
char ID[20]={"589803550"};
uint16_t FLAMEFLAG=0;//火焰标志
uint16_t CloseFLAMEFLAG=0;//火焰标志
/*==================SRAM静态内存========================*/
#if 1
/*定义线程控制块*/
static struct rt_thread event_thread;
/*定义一个线程栈*/
ALIGN(RT_ALIGN_SIZE)  
/*RT_ALIGN_SIZE 是rtconfig.h 里的一个宏 为4 
要求定义线程栈的时候还以RT_ALIGN_SIZE 个字节对齐*/
static rt_uint8_t rt_event_thread_stack[1024];


/*led2 线程控制块*/
static struct rt_thread mainevent_thread;/*线程控制块*/
static rt_uint8_t rt_mainevent_thread_stack[2048];/*线程栈的大小*/
static void mainevent_thread_entry(void *parameter);/*线程入口函数*/
/*=====================================================*/



/*刷卡 线程控制块*/
static struct rt_thread cardOpenDoor_thread;
static rt_uint8_t cardOpenDoor_thread_stack[1024];
static void cardOpenDoor_thread_entry(void *parameter);
/*=====================================================*/
#endif 

/*声明线程函数*/
static void event_thread_entry(void * parameter);
static void mainevent_thread_entry(void * parameter);
static void cardOpenDoor_thread_entry(void * parameter);



/*事件线程*/
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
		//在LOED上面显示
		//触发相对应的功能 打开风扇开关
		//串口发送到8266
//     printf("thread11 run \r\n");
		  distance=get_sro4_distance();
		  printf("distance: %d \r\n",distance);
		  sendDataTOONENET(ID,api_key,"distance",distance,0);
			if(distance<=30)
			{
				//报警蜂鸣器和显示
				led_onoff(1,0);
				rt_thread_delay(100);
				led_onoff(1,1);	
				rt_thread_delay(100);
			}
			if(humi>30)
			{
				//开启风扇
			
			}
			
			rt_thread_delay(200);
		}
			


}
/*主线程 火焰和可燃气体*/
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
				OLED_ShowCN(32,0,24);//警
				OLED_ShowCN(48,0,25);//报
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
			
			//触发火警
		  //后期增加打开花洒		
		}	
		mq2_value = get_adc2value()*3.3/0xfff;
		sprintf((char *)mq2_buf,"%.2f",mq2_value);
		printf("mq2_value: %f \n",mq2_value);
		sendDataTOONENET(ID,api_key,"combustiblegas",0,mq2_value);
		OLED_ShowStr(40,2,mq2_buf,2);
		if(  mq2_value >2.5 )//若是可燃气体则报警
		{
			led_onoff(2,1);
			rt_thread_delay(100);
			led_onoff(2,0);
			rt_thread_delay(100);
		}
		rt_thread_delay(200);
	}
}
	

//刷卡线程
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
			//设置一个按键中断
			//按键按下-----登记卡 //将卡号写进入flash和将数据写进卡里面加密
			//没有按键 //一直在读取卡
			//上报至8266
			if(cardRegisterFlag)//按下登记按键
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
				sendDataTOONENET(ID,api_key,"cardUID",cardUID[1],0	);//上传开门ID
			 while(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,
				{
					addrx+=4;	
				}
				write_flash(addrx,cardUID[1]);//将卡号写进去flah里面
					
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
				sendDataTOONENET(ID,api_key,"cardUID",cardUID[1],0	);//上传开门ID
				while(STMFLASH_ReadWord(addrx)!=cardUID[1])//有非0XFFFFFFFF的地方,要擦除这个扇区&& STMFLASH_ReadWord(addrx) !=0XFFFFFFFF
				{
					addrx+=4;
					if(addrx ==0x0801FFF)
					{
						printf("card open error \r\n");
						//从flash中遍历匹配卡序列号
				    //从flash中遍历匹配卡加密密码		
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
	
	//初始化GPIO
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	timer1_init();//初始化定时器 2 3
	usart1_init();
	usart3_init();
	DWT_Delay_Init();
	adc_init();//ADC初始化
	MQ2_init();//可燃气体传感器初始化
	dht11_init();//温湿度初始化	
	flame_init();//火焰传感器初始化
	supersonic_init();//超声波初始化
	I2C_Configuration();
	MFRC522_Initializtion();			//初始化MFRC522 
	OLED_Init();
	show_mq2_dht11();
	key_int_init();
	flash_init();
  printf("strart   run \r\n ");
	ESP8266_STATCPClient_MODE_Init(); //初始化8266
	/******************创建两个线程——SRAM静态内存******************************/
	//初始化线程
#if 1
	//事件线程
	rt_thread_init(&event_thread,         /*线程控制块*/
									"event",				       /*线程名字*/
									event_thread_entry,   /*线程入口函数*/
									RT_NULL,             /*线程入口函数的参数*/
									&rt_event_thread_stack[0],/*线程栈的起始地址*/
									sizeof(rt_event_thread_stack),/*线程栈的大小*/
										3,                  /*线程 的优先级*/
									20                    /*线程的时间片*/
															
	);                                             
	//初始化主线程
	rt_thread_init(&mainevent_thread,         /*线程控制块*/
									"mainevent",				       /*线程名字*/
									mainevent_thread_entry,   /*线程入口函数*/
									RT_NULL,             /*线程入口函数的参数*/
									&rt_mainevent_thread_stack[0],/*线程栈的起始地址*/
									sizeof(rt_mainevent_thread_stack),/*线程栈的大小*/
										3,                  /*线程 的优先级*/
									20                    /*线程的时间片*/
															
	);
	rt_thread_init(&cardOpenDoor_thread,         /*线程控制块*/
									"cardOpenDoor",				       /*线程名字*/
									cardOpenDoor_thread_entry,   /*线程入口函数*/
									RT_NULL,             /*线程入口函数的参数*/
									&cardOpenDoor_thread_stack[0],/*线程栈的起始地址*/
									sizeof(cardOpenDoor_thread_stack),/*线程栈的大小*/
										2,                  /*线程 的优先级*/
									20                    /*线程的时间片*/
															
	);
	/*开线程*/
	rt_thread_startup(&event_thread);
	rt_thread_startup(&mainevent_thread);
	rt_thread_startup(&cardOpenDoor_thread);
#endif

									
}


