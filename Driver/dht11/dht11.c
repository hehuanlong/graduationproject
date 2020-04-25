#include "dht11.h"
#include "sys.h"
#include "systick_delay.h"
#include "stdio.h"
#include "delay.h"
	
;
void Set_DQ_inout(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//����ʱ��ʹ�ܺ�GPIOA ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	//��ʼ��GPIO
	GPIO_InitStruct.GPIO_Mode=mode;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}
u8 dht11_init(void)
{
	u8 retry=0;//��¼�ȴ�ʱ��
	//PG9����Ϊ���
	Set_DQ_inout(GPIO_Mode_OUT);
	//����>18ms
	DQ_OUT=0;
	DWT_Delay_us(20000);
	DQ_OUT=1;
	DWT_Delay_us(30);
	
	//�ȴ���Ӧ ��PG9����Ϊ����ģʽ
	Set_DQ_inout(GPIO_Mode_IN);		
	//����80us
	while(DQ_IN && retry<100)//�ȴ���������
	{
		retry++;
		DWT_Delay_us(1); 
	}
	if(retry>=100)//û������ ���ش���
		return 1;
	
	//DWT_Delay_us(80);
	retry=0;
	//����80us
	while(!DQ_IN && retry<100)//�ȴ���������
	{
		retry++;
		DWT_Delay_us(1); 
	}
	if(retry>=100)//û������ ���ش���
		return 1;
	//DWT_Delay_us(80);
	return 0;
}
//��������  ��ȡһλ
u8 dht11_read_bit(void)
{
	//�ȴ���Ϊ�͵�ƽ
	while(DQ_IN);
	//�ȴ���Ϊ�ߵ�ƽ
	while(!DQ_IN);
	//��ʱ40us  
  DWT_Delay_us(40);  

	return DQ_IN;

}
//��ȡһ���ֽ�
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

//��ȡ��ʪ��
u8 dht11_read_data(u8 *temp,u8 *humi)
{
	u8 buf[5] = {0},i;
	
	if(dht11_init()==0){
		//��ȡ5���ֽ�
		for(i=0;i<5;i++)
			buf[i] = dht11_read_byte();
		//��֤У���
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