#include "stm32f4xx.h"
#include "timer.h"
#include "flame.h"
#include "sys.h"
/*
RCC_APB1PeriphClockCmd ��  42M --��Ԥ��Ƶ������Ϊ1    42*2 =84M
RCC_APB2PeriphClockCmd ��  84M --��Ԥ��Ƶ������Ϊ1    84*2 =168M   
*/
void timer1_init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//2.��ʼ����ʱ��2
	TIM_TimeBaseInitStruct.TIM_Prescaler = 840-1;// 84MHz/840 = 100KHz
	TIM_TimeBaseInitStruct.TIM_Period = 10000;//����100ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�����ӣ�1��Ƶ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//2.��ʼ����ʱ��2
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;// 84MHz/840 = 100KHz
	TIM_TimeBaseInitStruct.TIM_Period = 10000;//����100ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�����ӣ�1��Ƶ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	//3.ʹ�ܶ�ʱ�������ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//3.ʹ�ܶ�ʱ�������ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//4.��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//��ʱ��2�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);	
	
	//4.��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;//��ʱ��3�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);	
	//5.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM2,ENABLE);
	//5.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3,DISABLE);

}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET)//�����жϷ���
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
	//����жϱ�־
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)//�����жϷ���
	{
	
	}
	//����жϱ�־
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

