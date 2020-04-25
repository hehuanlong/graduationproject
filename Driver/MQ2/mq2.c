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
	
	//**************************�ж�Դ***************************
	//1.��ʼSYSCFGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.��ʼ��GPIOΪ����ģʽ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//PA0
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//3.����IO�ں��ж��ߵ�ӳ���ϵ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource11);


	//4.��ʼ���ⲿ�ж�Դ
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);
	
	//****************************NVIC����******************************
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�10-15
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
		
}
void EXTI15_10_IRQHandler(void) //led2��
{
		TIM_Cmd(TIM2,ENABLE);
	  TIM_SetCounter(TIM2,10000);

	EXTI_ClearITPendingBit(EXTI_Line11);

}
