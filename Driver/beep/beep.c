#include "beep.h"
#include "stm32f4xx.h"
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//����һ��GPIO��ṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;//0������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
}

