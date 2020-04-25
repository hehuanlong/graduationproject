#include "systick_delay.h"
void delay_init(void)
{
		//SysTick->CTRL &=(0x1<<2);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//ѡ��HCLK/8��Ϊ �ο�ʱ��
	SysTick->CTRL &=~SysTick_CTRL_TICKINT_Msk;//�������ж�
}

void delay_us (u32 us)
{
	//������װ��ֵ
	SysTick->LOAD = 21*us-1;//21M 1us ���Ǽ�����21  0-20
	//���õ�ǰֵΪ0
	SysTick->VAL=0;
	
	//����SysTicck
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;//0
	//�ȴ�����ʱ�䵽
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));//SysTick_CTRL_COUNTFLAG_Msk 1<<16

	//�ر�SysTicck
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0;
	
}
void delay_ms (u32 ms)
{
		//������װ��ֵ
	SysTick->LOAD = 21000*ms-1;//21M 1us ���Ǽ�����21  0-20
	//���õ�ǰֵΪ0
	SysTick->VAL=0;
	
	//����SysTicck
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;//0
	//�ȴ�����ʱ�䵽
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));

	//�ر�SysTicck
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0;

}

void delay_s (u32 s)
{
	while(s--){
		delay_ms(500);
		delay_ms(500);
	
	}
	 
}
