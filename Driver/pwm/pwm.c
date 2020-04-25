#include "stm32f4xx.h"
#include "pwm.h"
void pwm_init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//��ʱ���ṹ��
	TIM_OCInitTypeDef TIM_OCInitStruct;//��ʱ���ȽϽṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	//ʹ�ܶ�ʱ��TIM14   GPIOFʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//��ʼ��GPIOF 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//��PF9����ӳ�䵽TIM14
	 GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
  //��ʼ����ʱ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ������
	TIM_TimeBaseInitStruct.TIM_Period=1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=84-1;//1000000   
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//��ʼ����ʱ���Ƚϲ���
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse=500;
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
  
	//ʹ��Ԥװ�ڼĴ���
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	//ʹ���Զ���װ������λ
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	TIM_Cmd(TIM14,ENABLE);

}
/*************TIM1_CH3************************/
void pwm_led3init(void)//������ pwm����
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//��ʱ��ʱ��ʹ�ܺ�GPIOEʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//GPIO��ʼ��
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//��PE13����ӳ�䵽TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	
	//��ʱ���ĳ�ʼ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ������
	TIM_TimeBaseInitStruct.TIM_Period=10000;//1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=168-1;//168000000  /168 =1000000
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//��ʼ����ʱ���Ƚϲ���
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//����Ϊpwm1ģʽ
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//���״̬ʹ�� 
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//���Ե�ƽ
	TIM_OCInitStruct.TIM_Pulse=500;//��ʼ�Ƚϼ���ֵ
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
	
	//ʹ��Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//ʹ����װ�ؼĴ�������λ
	TIM_ARRPreloadConfig(TIM1, ENABLE);
 //������ʱ��1
 TIM_Cmd(TIM1,ENABLE);
 TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void pwm_led4init(void)//led4 pwm
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//��ʱ��ʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//GPIO��ʼ��
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	//��ʱ����ʼ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ������
	TIM_TimeBaseInitStruct.TIM_Period=1000;     //1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=168-1;//168000000  /168 =1000000
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//��PE14ӳ�䵽TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	//��ʼ����ʱ���Ƚϲ���
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//����Ϊpwm1ģʽ
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//���״̬ʹ�� 
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;//���Ե�ƽ
	TIM_OCInitStruct.TIM_Pulse=500;//��ʼ�Ƚϼ���ֵ
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	//��ʼ��Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//ʹ����װ�ؼĴ�������λ
	TIM_ARRPreloadConfig(TIM1, ENABLE);
 //������ʱ��1
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}



void pwm_beepinit(void)//������ pwm����
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//ʹ�ܶ�ʱ��TIM14   GPIOFʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//��ʼ��GPIOF 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//��PF9����ӳ�䵽TIM13
	 GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
  //��ʼ����ʱ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ������
	TIM_TimeBaseInitStruct.TIM_Period=1000;//1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler=84-1;//84000000   
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStruct);
	
	//��ʼ����ʱ���Ƚϲ���
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_Pulse=500;
	TIM_OC1Init(TIM13,&TIM_OCInitStruct);
  
	//ʹ��Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
	//ʹ���Զ���װ������λ
	TIM_ARRPreloadConfig(TIM13, ENABLE);
	//������ʱ��
	TIM_Cmd(TIM13,ENABLE);
}
