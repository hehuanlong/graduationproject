#include "beep.h"
#include "stm32f4xx.h"
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//定义一个GPIO大结构体
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;//0号引脚
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//无上拉无下拉
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
}

