#include "key.h"
#include "stm32f4xx.h"
void key_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;//����һ��GPIO��ṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;//0������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//2 3 4������
	GPIO_Init(GPIOE,&GPIO_InitStruct);

}
