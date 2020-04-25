#include "systick_delay.h"
void delay_init(void)
{
		//SysTick->CTRL &=(0x1<<2);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择HCLK/8作为 参考时钟
	SysTick->CTRL &=~SysTick_CTRL_TICKINT_Msk;//不产生中断
}

void delay_us (u32 us)
{
	//设置重装载值
	SysTick->LOAD = 21*us-1;//21M 1us 就是计数到21  0-20
	//设置当前值为0
	SysTick->VAL=0;
	
	//启动SysTicck
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;//0
	//等待计数时间到
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));//SysTick_CTRL_COUNTFLAG_Msk 1<<16

	//关闭SysTicck
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0;
	
}
void delay_ms (u32 ms)
{
		//设置重装载值
	SysTick->LOAD = 21000*ms-1;//21M 1us 就是计数到21  0-20
	//设置当前值为0
	SysTick->VAL=0;
	
	//启动SysTicck
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;//0
	//等待计数时间到
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));

	//关闭SysTicck
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
