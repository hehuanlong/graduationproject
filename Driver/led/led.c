#include "stm32f4xx.h"
#include "led.h"
//初始化 PF9 PF10   PE13 PE14
void led_init(void)
{
	
	//使能GPIOE GPIOF时钟
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN|RCC_AHB1ENR_GPIOFEN;
	
	//输出模式
	GPIOF->MODER &= ~(GPIO_MODER_MODER9|GPIO_MODER_MODER10);
	GPIOF->MODER |= GPIO_MODER_MODER9_0|GPIO_MODER_MODER10_0;//0101
	
	//推挽输出
	GPIOF->OTYPER &= ~(GPIO_OTYPER_OT_9|GPIO_OTYPER_OT_10);
	
	//输出速度50M
	GPIOF->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR9|GPIO_OSPEEDER_OSPEEDR10);
	GPIOF->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1|GPIO_OSPEEDER_OSPEEDR10_1;//1010
	
	//无上下拉
	GPIOF->PUPDR &= ~(GPIO_PUPDR_PUPDR9|GPIO_PUPDR_PUPDR10);
	
	//关闭LED
	GPIOF->BSRRL |= GPIO_BSRR_BS_9|GPIO_BSRR_BS_10;
}

//控制灯的亮灭
/*
n = 1,2,3,4 表示哪盏灯
onoff = 0,1 表示灭亮
*/
void led_onoff(int n,int onoff)
{

	
	if(n==1){
		if(onoff)
			GPIOF->BSRRH |= GPIO_BSRR_BS_9;
		else
			GPIOF->BSRRL |= GPIO_BSRR_BS_9;
	}
	else if(n==2){
		if(onoff)
			GPIOF->BSRRH |= GPIO_BSRR_BS_10;
		else
			GPIOF->BSRRL |= GPIO_BSRR_BS_10;
	}
	
}

void delay(unsigned ms)
{
	int i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<10000;j++);
}
