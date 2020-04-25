#include "stm32f4xx.h"
#include "pwm.h"
void pwm_init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//定时器结构体
	TIM_OCInitTypeDef TIM_OCInitStruct;//定时器比较结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	//使能定时器TIM14   GPIOF时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//初始化GPIOF 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//将PF9复用映射到TIM14
	 GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
  //初始化定时器
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟因子
	TIM_TimeBaseInitStruct.TIM_Period=1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=84-1;//1000000   
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//初始化定时器比较参数
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse=500;
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
  
	//使能预装在寄存器
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	//使能自动重装载允许位
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	TIM_Cmd(TIM14,ENABLE);

}
/*************TIM1_CH3************************/
void pwm_led3init(void)//蜂鸣器 pwm调制
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//定时器时钟使能和GPIOE时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//GPIO初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//将PE13复用映射到TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	
	//定时器的初始化
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//设置为向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟因子
	TIM_TimeBaseInitStruct.TIM_Period=10000;//1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=168-1;//168000000  /168 =1000000
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//初始化定时器比较参数
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//设置为pwm1模式
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//输出状态使能 
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//极性电平
	TIM_OCInitStruct.TIM_Pulse=500;//初始比较计数值
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
	
	//使能预装载寄存器
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//使能重装载寄存器允许位
	TIM_ARRPreloadConfig(TIM1, ENABLE);
 //开启定时器1
 TIM_Cmd(TIM1,ENABLE);
 TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void pwm_led4init(void)//led4 pwm
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//定时器时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//GPIO初始化
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//定时器初始化
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//设置为向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟因子
	TIM_TimeBaseInitStruct.TIM_Period=1000;     //1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=168-1;//168000000  /168 =1000000
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//将PE14映射到TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	//初始化定时器比较参数
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//设置为pwm1模式
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//输出状态使能 
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;//极性电平
	TIM_OCInitStruct.TIM_Pulse=500;//初始比较计数值
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	//初始化预装载寄存器
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//使能重装载寄存器允许位
	TIM_ARRPreloadConfig(TIM1, ENABLE);
 //开启定时器1
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}



void pwm_beepinit(void)//蜂鸣器 pwm调制
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//使能定时器TIM14   GPIOF时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//初始化GPIOF 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//将PF9复用映射到TIM13
	 GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
  //初始化定时器
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟因子
	TIM_TimeBaseInitStruct.TIM_Period=1000;//1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=84-1;//84000000   
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStruct);
	
	//初始化定时器比较参数
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_Pulse=500;
	TIM_OC1Init(TIM13,&TIM_OCInitStruct);
  
	//使能预装载寄存器
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
	//使能自动重装载允许位
	TIM_ARRPreloadConfig(TIM13, ENABLE);
	//开启定时器
	TIM_Cmd(TIM13,ENABLE);
}
