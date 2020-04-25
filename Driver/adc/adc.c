#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
void adc_init(void )
{
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	 GPIO_InitTypeDef GPIO_InitStruct;//定义一个GPIO大结构体
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//使能adc时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//84M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AN;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;//0号引脚
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//无上拉无下拉
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//复位ADC
	ADC_DeInit();
	//设置ADC1的分频系数
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//不使用DMA
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;//4分频 84/4 =21 不超过30M
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//采样周期5ms
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//初始化ADC1(分辨率,规则序列,工作模式)
	//5.ADC初始化(分辨率 规则序列 工作模式)
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//不连续转换
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//不外部触发，用软件触发
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//不使用多通道
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12位分辨率
	ADC_InitStruct.ADC_NbrOfConversion = 2;//规则组数目
	ADC_Init(ADC1, &ADC_InitStruct);
	
		//7.配置规则通道(ADC1的通道5,第一个规则,采样时间为3个时钟周期)
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles);
	
	//6.使能ADC1
	ADC_Cmd(ADC1,ENABLE);
}
u16 get_adc1value(void)
{
	u16 value=0;
	//7.配置规则通道(ADC1的通道5,第一个规则,采样时间为3个时钟周期)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	//开启ADC转换
	ADC_SoftwareStartConv(ADC1);
	//等待转换结束
   while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)!=SET);
  //读取转换结果
    value=ADC_GetConversionValue(ADC1);
	
	return value;

}

u16 get_adc2value(void)
{
	u16 value=0;
	uint8_t i=0;
	//7.配置规则通道(ADC1的通道5,第一个规则,采样时间为3个时钟周期)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
	//开启ADC转换
	ADC_SoftwareStartConv(ADC1);
	//等待转换结束
   while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)!=SET)
	 {
			
		   i++;
			 DELAY_MS(100);
		   if(  i== 30 ) //超时3S则判
			 {
				 printf("ADC_GetFlagStatus  Timeout\r\n");
				 //上报至8266				
				 break;
			 }
			 
	 }
  //读取转换结果
    value=ADC_GetConversionValue(ADC1);
	
	return value;

}

