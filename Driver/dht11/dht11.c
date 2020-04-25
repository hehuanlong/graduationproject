#include "dht11.h"
#include "sys.h"
#include "systick_delay.h"
#include "stdio.h"
#include "delay.h"
	
;
void Set_DQ_inout(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//串口时钟使能和GPIOA 使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	//初始化GPIO
	GPIO_InitStruct.GPIO_Mode=mode;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}
u8 dht11_init(void)
{
	u8 retry=0;//记录等待时间
	//PG9设置为输出
	Set_DQ_inout(GPIO_Mode_OUT);
	//拉低>18ms
	DQ_OUT=0;
	DWT_Delay_us(20000);
	DQ_OUT=1;
	DWT_Delay_us(30);
	
	//等待响应 将PG9设置为输入模式
	Set_DQ_inout(GPIO_Mode_IN);		
	//拉低80us
	while(DQ_IN && retry<100)//等待总线拉低
	{
		retry++;
		DWT_Delay_us(1); 
	}
	if(retry>=100)//没有拉低 返回错误
		return 1;
	
	//DWT_Delay_us(80);
	retry=0;
	//拉高80us
	while(!DQ_IN && retry<100)//等待总线拉低
	{
		retry++;
		DWT_Delay_us(1); 
	}
	if(retry>=100)//没有拉高 返回错误
		return 1;
	//DWT_Delay_us(80);
	return 0;
}
//传送数据  读取一位
u8 dht11_read_bit(void)
{
	//等待变为低电平
	while(DQ_IN);
	//等待变为高电平
	while(!DQ_IN);
	//延时40us  
  DWT_Delay_us(40);  

	return DQ_IN;

}
//读取一个字节
u8 dht11_read_byte(void)
{
	u8 i,data=0;
	for(i=0;i<8;i++)
	{
		data <<=1 ;
		data |=dht11_read_bit();
	
	}

	return data;

}

//读取温湿度
u8 dht11_read_data(u8 *temp,u8 *humi)
{
	u8 buf[5] = {0},i;
	
	if(dht11_init()==0){
		//读取5个字节
		for(i=0;i<5;i++)
			buf[i] = dht11_read_byte();
		//验证校验和
		if(buf[0]+buf[1]+buf[2]+buf[3]==buf[4]){
			*temp = buf[2];
			*humi = buf[0];
			
			return 0;
		}
	}
	
	return 1;
}
void temp_humi_res(u8 temp,u8 humi)
{
	if(temp>30)
		PFout(8) = 1;
	else
		PFout(8) = 0;
	
	if(humi>50){
		PFout(9) = 0;
		PFout(10) = 1;
	}
	else if(humi<20){
		PFout(10) = 0;
		PFout(9) = 1;
	}
	else{
		PFout(9) = 1;
		PFout(10) = 1;
	}
}