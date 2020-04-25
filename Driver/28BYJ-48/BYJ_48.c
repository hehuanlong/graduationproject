#include "byj_48.h"
#include "stm32f4xx.h"
#include "systick_delay.h"
void byj_48_init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;//定义一个GPIO大结构体
	//开启SPI和GPIO时钟  PB6 PA8 PD6 PD7 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;//8号引脚
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//设置片选脚为输出
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;//无上拉无下拉
	GPIO_Init(GPIOD,&GPIO_InitStruct);


}
//单四拍 正转
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
	