#include "stm32f4xx.h"
#include "key_int.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
uint8_t cardRegisterFlag = 0 ;
//�жϳ�ʼ��
extern uint16_t CloseFLAMEFLAG;
void key_int_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//**************************�ж�Դ***************************
	//1.��ʼSYSCFGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.��ʼ��GPIOΪ����ģʽ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//PA0
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;//PE2 PE3 PE4
	//	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_2| GPIO_Pin_3|GPIO_Pin_4;//PE2 PE3 PE4
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//3.����IO�ں��ж��ߵ�ӳ���ϵ
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

	//4.��ʼ���ⲿ�ж�Դ
	//EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Line = EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);
	
	//****************************NVIC����******************************
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
//	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�4
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
}

//�жϴ�����
//void EXTI0_IRQHandler(void)
//{
//	//����LED1
//	led1_onoff(1,1);
//	delay(2000);
//	led1_onoff(1,0);
//	//����жϱ�־
//	EXTI_ClearITPendingBit(EXTI_Line0);
//}

//void EXTI2_IRQHandler(void)
//{
//	//����LED1
//	led1_onoff(2,1);
//	delay(2000);
//	led1_onoff(2,0);
//	//����жϱ�־
//	EXTI_ClearITPendingBit(EXTI_Line2);
//}


void EXTI3_IRQHandler(void)
{
	if(PEin(3) == 0)
	{
		DWT_Delay_us(10000);
		if(PEin(3) == 0)
		{
		  cardRegisterFlag=1;
		
		}
	
	
	
	}	
	//����LED1 
	
	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	if(PEin(4) == 0)
	{
		DWT_Delay_us(10000);
		if(PEin(4) == 0)
		{
		 CloseFLAMEFLAG=1;
		
		}
	}	
	EXTI_ClearITPendingBit(EXTI_Line4);
}
