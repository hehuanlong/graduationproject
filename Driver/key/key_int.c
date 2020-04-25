#include "stm32f4xx.h"
#include "key_int.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
uint8_t cardRegisterFlag = 0 ;
//中断初始化
extern uint16_t CloseFLAMEFLAG;
void key_int_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//**************************中断源***************************
	//1.开始SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//2.初始化GPIO为输入模式
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//PA0
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;//PE2 PE3 PE4
	//	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_2| GPIO_Pin_3|GPIO_Pin_4;//PE2 PE3 PE4
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//3.设置IO口和中断线的映射关系
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

	//4.初始化外部中断源
	//EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Line = EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&EXTI_InitStruct);
	
	//****************************NVIC部分******************************
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
//	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;//外部中断4
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
}

//中断处理函数
//void EXTI0_IRQHandler(void)
//{
//	//点亮LED1
//	led1_onoff(1,1);
//	delay(2000);
//	led1_onoff(1,0);
//	//清除中断标志
//	EXTI_ClearITPendingBit(EXTI_Line0);
//}

//void EXTI2_IRQHandler(void)
//{
//	//点亮LED1
//	led1_onoff(2,1);
//	delay(2000);
//	led1_onoff(2,0);
//	//清除中断标志
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
	//点亮LED1 
	
	//清除中断标志
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
