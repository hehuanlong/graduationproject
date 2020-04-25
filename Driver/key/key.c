#include "key.h"
#include "stm32f4xx.h"
void key_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;//定义一个GPIO大结构体
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;//0号引脚
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//无上拉无下拉
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//2 3 4号引脚
	GPIO_Init(GPIOE,&GPIO_InitStruct);

}
