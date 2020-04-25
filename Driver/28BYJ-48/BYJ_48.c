#include "byj_48.h"
#include "stm32f4xx.h"
#include "systick_delay.h"
void byj_48_init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;//����һ��GPIO��ṹ��
	//����SPI��GPIOʱ��  PB6 PA8 PD6 PD7 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;//8������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;//������������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//����Ƭѡ��Ϊ���
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;//������������
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;//������������
	GPIO_Init(GPIOD,&GPIO_InitStruct);


}
//������ ��ת
void  Forward_byj_48(void )
{

 while(1)
	{

	DOUBLEA;
	delay_ms(5);
	DOUBLEB;
	delay_ms(5);
	DOUBLEC;
	delay_ms(5);
	DOUBLED;
	delay_ms(5);
	
	}



}
	