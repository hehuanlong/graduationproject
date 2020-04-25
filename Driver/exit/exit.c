#include "exit.h"
#include "stm32f4xx.h"
#include "warterfall_led.h"
void exit_init(void)
{
	  NVIC_InitTypeDef NVIC_InitStruct;
		EXTI_InitTypeDef EXTI_InitStruct;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0) ; //设置IO口和中断线的映射关系
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2); //设置IO口和中断线的映射关系
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3); //设置IO口和中断线的映射关系
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4); //设置IO口和中断线的映射关系
		//SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,,EXTI_PinSource2|,EXTI_PinSource3|EXTI_PinSource4); 不可以这样写
	
/***********************l流水灯**************************************/			
		EXTI_InitStruct.EXTI_Line=EXTI_Line0;//中断号
	  EXTI_InitStruct.EXTI_LineCmd=ENABLE;//中断使能
		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//设置为中断模式
		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStruct);
		
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;//中断号
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x03;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x03;
		NVIC_Init(&NVIC_InitStruct);
		
/***********************led2 亮**************************************/		
		
		EXTI_InitStruct.EXTI_Line=EXTI_Line2;//中断号
	  EXTI_InitStruct.EXTI_LineCmd=ENABLE;//中断使能
		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//设置为中断模式
		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStruct);
		
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;//中断号
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x02;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x02;
		NVIC_Init(&NVIC_InitStruct);

/***********************led3 亮**************************************/		
		
		EXTI_InitStruct.EXTI_Line=EXTI_Line3;//中断号
	  EXTI_InitStruct.EXTI_LineCmd=ENABLE;//中断使能
		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//设置为中断模式
		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStruct);
		
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;//中断号
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x01;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;
		NVIC_Init(&NVIC_InitStruct);
		
/***********************led4 亮**************************************/		
		
		EXTI_InitStruct.EXTI_Line=EXTI_Line4;//中断号
	  EXTI_InitStruct.EXTI_LineCmd=ENABLE;//中断使能
		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//设置为中断模式
		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStruct);
		
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;//中断号
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x00;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x00;
		NVIC_Init(&NVIC_InitStruct);
		
		
}
void EXTI0_IRQHandler(void)  //流水灯
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		while(1)
		warterfall_led();
	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);

}

void EXTI2_IRQHandler(void) //led2亮
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
	{
		while(1)
		led_onoff(2,1);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line2);

}
void EXTI3_IRQHandler(void) //led2亮
{
	if(EXTI_GetITStatus(EXTI_Line3)==SET)
	{
		while(1)
		led_onoff(3,1);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line3);

}
void EXTI4_IRQHandler(void) //led4亮
{
	if(EXTI_GetITStatus(EXTI_Line4)==SET)
	{
		while(1)
		led_onoff(4,1);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line4);

}



	
