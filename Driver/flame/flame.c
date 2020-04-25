#include "flame.h"
#include "stm32f4xx.h"
extern uint16_t FLAMEFLAG;//�����־
void flame_init(void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//**************************�ж�Դ***************************
	//1.��ʼSYSCFGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.��ʼ��GPIOΪ����ģʽ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PA0
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
//	//3.����IO�ں��ж��ߵ�ӳ���ϵ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);


	//4.��ʼ���ⲿ�ж�Դ
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//�½��ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);
	
	//****************************NVIC����******************************
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�10-15
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);

}
void EXTI9_5_IRQHandler(void) //led2��
{
	//TIM_Cmd(TIM3,ENABLE);
	// TIM_SetCounter(TIM3,10000);
	if(EXTI_GetITStatus(EXTI_Line6)==SET)
	FLAMEFLAG=1;
	EXTI_ClearITPendingBit(EXTI_Line6);

}

