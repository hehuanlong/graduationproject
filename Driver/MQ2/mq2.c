#include "mq2.h"
#include "stm32f4xx.h"
#include "systick_delay.h"
#include "usart.h"
#include "timer.h"
#include "sys.h"
void MQ2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//**************************中断源***************************
	//1.开始SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.初始化GPIO为输入模式
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//PA0
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//3.设置IO口和中断线的映射关系
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource11);


	//4.初始化外部中断源
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//下降沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&EXTI_InitStruct);
	
	//****************************NVIC部分******************************
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断10-15
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
		
}
void EXTI15_10_IRQHandler(void) //led2亮
{
		TIM_Cmd(TIM2,ENABLE);
	  TIM_SetCounter(TIM2,10000);

	EXTI_ClearITPendingBit(EXTI_Line11);

}
