#include "stm32f4xx.h"
#include "timer.h"
#include "flame.h"
#include "sys.h"
/*
RCC_APB1PeriphClockCmd ：  42M --》预分频数不是为1    42*2 =84M
RCC_APB2PeriphClockCmd ：  84M --》预分频数不是为1    84*2 =168M   
*/
void timer1_init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.使能定时器2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//2.初始化定时器2
	TIM_TimeBaseInitStruct.TIM_Prescaler = 840-1;// 84MHz/840 = 100KHz
	TIM_TimeBaseInitStruct.TIM_Period = 10000;//周期100ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟因子，1分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//2.初始化定时器2
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;// 84MHz/840 = 100KHz
	TIM_TimeBaseInitStruct.TIM_Period = 10000;//周期100ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟因子，1分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	//3.使能定时器更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//3.使能定时器更新中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//4.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//定时器2中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);	
	
	//4.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;//定时器3中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);	
	//5.使能定时器
	TIM_Cmd(TIM2,ENABLE);
	//5.使能定时器
	TIM_Cmd(TIM3,DISABLE);

}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET)//更新中断发生
	{
		if(FLAME)
		{
			PEout(13)=0;
		
		}
		else 
		{
			PEout(13)=0;
		
		}
	}
	//清除中断标志
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)//更新中断发生
	{
	
	}
	//清除中断标志
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

