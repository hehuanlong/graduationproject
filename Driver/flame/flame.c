#include "flame.h"
#include "stm32f4xx.h"
extern uint16_t FLAMEFLAG;//火焰标志
void flame_init(void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//**************************中断源***************************
	//1.开始SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.初始化GPIO为输入模式
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PA0
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
//	//3.设置IO口和中断线的映射关系
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);


	//4.初始化外部中断源
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//下降沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&EXTI_InitStruct);
	
	//****************************NVIC部分******************************
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断10-15
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);

}
void EXTI9_5_IRQHandler(void) //led2亮
{
	//TIM_Cmd(TIM3,ENABLE);
	// TIM_SetCounter(TIM3,10000);
	if(EXTI_GetITStatus(EXTI_Line6)==SET)
	FLAMEFLAG=1;
	EXTI_ClearITPendingBit(EXTI_Line6);

}

